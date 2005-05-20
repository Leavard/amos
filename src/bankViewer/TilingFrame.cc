#include "TilingFrame.hh"
#include "TilingField.hh"
#include <qscrollview.h>
#include <qlayout.h>
#include "ConsensusField.hh"
#include "RenderSeq.hh"
#include "UIElements.hh"
#include <qregexp.h>

using namespace std;
using namespace AMOS;

int min (int a, int b)
{
  return a < b ? a : b;
}



struct SNPTilingOrderCmp
{
  static int snpposition;
  bool operator() (const RenderSeq_t & a, const RenderSeq_t & b)
  {
    char abase = a.base(snpposition);
    char bbase = b.base(snpposition);

    return abase < bbase;
  }
};

int SNPTilingOrderCmp::snpposition(0);


TilingFrame::TilingFrame(DataStore * datastore, 
                         AlignmentInfo * ai,
                         QWidget * parent, 
                         const char * name, 
                         WFlags f)
  :QFrame(parent, name, f),
   m_datastore(datastore),
   m_alignment(ai)
{
  m_gindex = 0;
  m_fontsize = 10;
  m_displayAllChromo = true;

  m_loadedStart = m_loadedEnd = -1;

  toggleDisplayAllChromo(false);
  m_nextDiscrepancyBuffer = 10;

  resize(500, 100);
  m_sv = new QScrollView(this, "tilingscroll");
  m_sv->setHScrollBarMode(QScrollView::AlwaysOff);
  
  m_tilingfield = new TilingField(datastore,
                                  m_renderedSeqs,
                                  m_consensus,
                                  m_cstatus,
                                  m_alignment,
                                  m_gindex,
                                  m_fontsize,
                                  m_sv->viewport(),
                                  "tiling" );

  m_sv->addChild(m_tilingfield);
  m_sv->setPaletteBackgroundColor(UIElements::color_tiling);

  m_consfield = new ConsensusField(m_consensus, m_cstatus, m_alignment,
                                   m_gindex, this, "cons");

  QBoxLayout * layout = new QVBoxLayout(this);
  layout->addWidget(m_consfield);
  layout->addWidget(m_sv, 10);


  connect(this,        SIGNAL(fontSizeChanged(int)),
          m_consfield, SLOT(setFontSize(int)));

  connect(m_tilingfield, SIGNAL(setTilingVisibleRange(int, int, int)),
          this,          SIGNAL(setTilingVisibleRange(int, int, int)));

  connect(m_consfield,   SIGNAL(sortColumns(int)),
          this,          SLOT(sortColumns(int)));

  connect(this,        SIGNAL(toggleDisplayQV(bool)),
          m_tilingfield, SLOT(toggleDisplayQV(bool)));

  connect(this,        SIGNAL(toggleLowQualityLowerCase(bool)),
          m_tilingfield, SLOT(toggleLowQualityLowerCase(bool)));

  connect(this,        SIGNAL(toggleShowFullRange(bool)),
          m_tilingfield, SLOT(toggleShowFullRange(bool)));

  connect(this,        SIGNAL(toggleHighlightDiscrepancy(bool)),
          m_tilingfield, SLOT(toggleHighlightDiscrepancy(bool)));

  connect(this,        SIGNAL(toggleShowNumbers(bool)),
          m_consfield,   SLOT(toggleShowNumbers(bool)));

  connect(this,        SIGNAL(toggleBaseColors(bool)),
          m_consfield,   SLOT(toggleBaseColors(bool)));

  connect(this,        SIGNAL(toggleShowIndicator(bool)),
          m_consfield,   SLOT(toggleShowIndicator(bool)));

  connect(this,        SIGNAL(toggleSNPColoring(bool)),
          m_tilingfield, SLOT(toggleSNPColoring(bool)));

  connect(this,        SIGNAL(toggleBaseColors(bool)),
          m_tilingfield, SLOT(toggleBaseColors(bool)));
}

void TilingFrame::toggleDisplayAllChromo(bool display)
{
  m_displayAllChromo = display;

  if (m_displayAllChromo)
  {
    m_loadedWidth = 500;

    // load only the reads in this region
    setGindex(m_gindex);

    // force loading of chromatograms
    vector<RenderSeq_t>::iterator ri;
    for (ri =  m_renderedSeqs.begin();
         ri != m_renderedSeqs.end();
         ri++)
    {
      ri->loadTrace(m_datastore);
      ri->m_displayTrace = true;
    }

    repaint();
  }
  else
  {
    m_loadedWidth = 5000;
  }
}


void TilingFrame::paintEvent(QPaintEvent * event)
{
  QFrame::paintEvent(event);
  m_consfield->repaint();

  m_tilingfield->setSize(width(), m_sv->visibleHeight());
  m_tilingfield->repaint();
}

void TilingFrame::setContigId(int contigId)
{
  if (m_datastore->m_loaded)
  {
    try
    {
      m_tiling = m_datastore->m_contig.getReadTiling();
      m_consensus = m_datastore->m_contig.getSeqString();

      m_cstatus.erase();
      m_cstatus.resize(m_consensus.size(), ' ');
      m_renderedSeqs.clear();

      for (unsigned int i = 0; i < m_alignment->m_gaps.size(); i++)
      {
        m_consensus.insert(i+m_alignment->m_gaps[i], 1, '*');
        m_cstatus.insert  (i+m_alignment->m_gaps[i], 1, '*');
      }

      m_consensus.insert((unsigned) 0,(unsigned) m_alignment->m_startshift, '*');
      m_cstatus.insert  ((unsigned) 0,(unsigned) m_alignment->m_startshift, '*');

      m_consensus.append(m_alignment->m_endshift, '*');
      m_cstatus.append  (m_alignment->m_endshift, '*');

      sort(m_tiling.begin(), m_tiling.end(), RenderSeq_t::TilingOrderCmp());

      m_loadedStart = m_loadedEnd = -1;

      m_consfield->setHighlightRange(-1,-1);
      setGindex(0);
      emit setGindexRange(0, (int)m_consensus.size()-1);
    }
    catch (Exception_t & e)
    {
      cerr << "ERROR: -- Fatal AMOS Exception --\n" << e;
    }
  }
}

void TilingFrame::setFontSize(int fontsize )
{
  if (fontsize == m_fontsize) { return; }
  m_fontsize = fontsize;

  emit fontSizeChanged(m_fontsize);
  repaint();
}

void TilingFrame::loadContigRange(int gindex)
{
  //cerr << "TF:setgindex: " << gindex << endl;
  if (m_tiling.empty()) { return; }

  int basespace = 5;
  int basewidth = m_fontsize + basespace;
  int tilehoffset = m_fontsize*10;
  m_displaywidth = (width()-tilehoffset)/basewidth;

  gindex = min(gindex, m_consensus.size()-m_displaywidth+1);
  gindex = max(gindex, 0);
  m_gindex = gindex;

  int grangeStart = m_alignment->getContigPos(m_gindex);
  int grangeEnd   = m_alignment->getContigPos(min(m_gindex + m_displaywidth+200, (int)m_consensus.length()));

  if (grangeStart < m_loadedStart || grangeEnd > m_loadedEnd)
  {
    m_loadedStart = max(0, grangeStart-m_loadedWidth/2);
    m_loadedEnd   = min(m_consensus.length(), grangeEnd+m_loadedWidth/2);
      
    // Render the aligned sequences
    vector<Tile_t>::iterator vi;

    int orig = m_renderedSeqs.size();
    int kept = 0;
    vector<RenderSeq_t>::iterator ri;

    for (vi =  m_tiling.begin(); vi != m_tiling.end(); vi++)
    {
      int hasOverlap = RenderSeq_t::hasOverlap(m_loadedStart, m_loadedEnd,
                                               vi->offset, vi->range.getLength() + vi->gaps.size(),
                                               m_consensus.length());
      if (hasOverlap)
      {
        // see if this has already been rendered
        bool found = false;
        for (ri =  m_renderedSeqs.begin();
             ri != m_renderedSeqs.end();
             ri++)
        {
          if (ri->m_tile->source == vi->source)
          {
            kept++;
            found = true;
            break;
          }
        }

        if (found) { continue; }

        // hasn't been rendered before
        RenderSeq_t rendered;
        rendered.load(m_datastore, &*vi);
        if (m_displayAllChromo) { rendered.loadTrace(m_datastore); rendered.m_displayTrace = true; }
        m_renderedSeqs.push_back(rendered);

        for (int gindex = rendered.m_loffset; gindex <= rendered.m_roffset; gindex++)
        {
          int global = m_alignment->getGlobalPos(gindex);
          if (m_cstatus[global] == '*')
          {
            cerr << "err" << endl;
          }
          else if (m_cstatus[global] == ' ')                   
            { m_cstatus[global] = rendered.base(gindex); }

          else if (toupper(m_cstatus[global]) != toupper(rendered.base(gindex))) 
            { m_cstatus[global] = 'X'; }
        }
      }
    }

    // remove sequences that are no longer in the view window
    int vectorpos = 0;
    for (ri = m_renderedSeqs.begin(); ri != m_renderedSeqs.end();)
         
    {
      if (!RenderSeq_t::hasOverlap(m_loadedStart, m_loadedEnd,
                                   ri->m_tile->offset, ri->gappedLen(),
                                   m_consensus.length()))
      {
        m_renderedSeqs.erase(ri);
        ri = m_renderedSeqs.begin() + vectorpos;
      }
      else
      {
        vectorpos++;
        ri++;
      }
    }

    cerr << "Loaded [" << m_loadedStart << "," << m_loadedEnd << "]:" << m_renderedSeqs.size() << " kept:" << kept << " of: " << orig << endl;
  }
}

void TilingFrame::setGindex(int gindex)
{
  if (m_loadedStart != -1 && gindex == m_gindex) { return; }

  if      (gindex < 0)                          { gindex = 0; }
  else if (gindex > (int) m_consensus.length()) { gindex = m_consensus.length(); }

  loadContigRange(gindex);
  emit gindexChanged(gindex);
  repaint();
}

void TilingFrame::advanceNextDiscrepancy()
{

  int gindex = m_gindex+m_nextDiscrepancyBuffer+1;

  while (gindex < (int)m_consensus.length())
  {
    if (gindex > m_loadedEnd)
    {
      loadContigRange(gindex);
    }

    while (gindex <= m_loadedEnd)
    {
      if (m_cstatus[gindex] == 'X')
      {
        setGindex(gindex-m_nextDiscrepancyBuffer);
        return;
      }
      else
      {
        gindex++;
      }
    }
  }

  setGindex(gindex);
} 

void TilingFrame::sortColumns(int gindex)
{
  SNPTilingOrderCmp::snpposition = gindex;
  stable_sort(m_renderedSeqs.begin(), m_renderedSeqs.end(), SNPTilingOrderCmp());

  vector<RenderSeq_t>::iterator vi;
  for (vi =  m_renderedSeqs.begin();
       vi != m_renderedSeqs.end();
       vi++)
  {
    vi->bgcolor = vi->base(gindex);
  }
  repaint();
}

void TilingFrame::advancePrevDiscrepancy()
{
  int gindex = m_gindex+m_nextDiscrepancyBuffer-1;

  while (gindex >=0 )
  {
    if (gindex < m_loadedStart)
    {
      loadContigRange(gindex);
    }

    while (gindex >= m_loadedStart)
    {
      if (m_cstatus[gindex] == 'X')
      {
        setGindex(gindex-m_nextDiscrepancyBuffer);
        return;
      }

      gindex--;
    }
  }

  setGindex(gindex);
}


void TilingFrame::highlightRead(int iid)
{
  if ((ID_t)iid != NULL_ID)
  {
    vector<Tile_t>::iterator vi;
    for (vi =  m_tiling.begin(); vi != m_tiling.end(); vi++)
    {
      if (vi->source == (ID_t)iid)
      {
        setGindex(vi->offset);
      }
    }
  }
}


void TilingFrame::searchString(const QString & str, bool forward)
{
  if (!str.isEmpty())
  {
    int pos;
    QRegExp regex(str);
    QString qcons(m_consensus.c_str());

    int gindex = m_gindex + m_nextDiscrepancyBuffer;

    if (forward)
    {
      gindex +=  1;
      pos = regex.search(qcons, gindex);
    }
    else
    {
      gindex -= 1;
      pos = regex.searchRev(qcons, gindex - m_consensus.length());
    }

    if (pos != -1)
    {
      m_consfield->setHighlightRange(pos, pos+regex.matchedLength() - 1);
      setGindex(pos-m_nextDiscrepancyBuffer);
    }
  }
}
