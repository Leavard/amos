#include "TilingFrame.hh"
#include "TilingField.hh"
#include <qscrollview.h>
#include <qlayout.h>
#include "ConsensusField.hh"
#include "InsertWindow.hh"
#include "RenderSeq.hh"

using namespace std;
using namespace AMOS;

int min (int a, int b)
{
  return a < b ? a : b;
}

TilingFrame::TilingFrame(QWidget * parent, const char * name, WFlags f)
  :QFrame(parent, name, f),
   read_bank(Read_t::NCODE),
   contig_bank(Contig_t::NCODE)
{
  m_contigId = 0;
  m_gindex = 0;
  m_fontsize = 12;
  m_displayAllChromo = true;
  m_loadedStart = -1;
  m_loadedEnd = -1;

  toggleDisplayAllChromo(false);


  resize(500, 500);
  m_db = "DMG";
  m_sv = new QScrollView(this, "tilingscroll");
  m_sv->setHScrollBarMode(QScrollView::AlwaysOff);
  
  m_tilingfield = new TilingField(m_renderedSeqs,
                                  m_consensus,
                                  m_cstatus,
                                  m_db,
                                  m_gindex,
                                  m_fontsize,
                                  m_sv->viewport(),
                                  "tiling" );

  m_sv->addChild(m_tilingfield);
  m_sv->setPaletteBackgroundColor(QColor(170,170,170));

  m_consfield = new ConsensusField(m_consensus, m_cstatus,
                                   m_gindex, this, "cons");

  QGridLayout * layout = new QGridLayout(this, 2, 1);
  layout->addWidget(m_consfield, 0,0);
  layout->addWidget(m_sv,1,0);
  layout->setRowStretch(1,10);

  connect(this,        SIGNAL(fontSizeChanged(int)),
          m_consfield, SLOT(setFontSize(int)));

  connect(this,        SIGNAL(setShowNumbers(bool)),
          m_consfield, SLOT(setShowNumbers(bool)));

  connect(this,          SIGNAL(setHighlightDiscrepancy(bool)),
          m_consfield,   SLOT(toggleHighlightDiscrepancy(bool)));

  connect(this,          SIGNAL(setHighlightDiscrepancy(bool)),
          m_tilingfield, SLOT(toggleHighlightDiscrepancy(bool)));
}

void TilingFrame::toggleHighlightDiscrepancy(bool show)
{
  emit setHighlightDiscrepancy(show);
}

void TilingFrame::toggleShowNumbers(bool doShow)
{
  emit setShowNumbers(doShow);
}

void TilingFrame::paintEvent(QPaintEvent * event)
{
  QFrame::paintEvent(event);
  m_consfield->repaint();

  //m_tilingfield->setSize(m_sv->visibleWidth(), m_sv->visibleHeight());
  m_tilingfield->setSize(width(), m_sv->visibleHeight());
  m_tilingfield->repaint();
}

void TilingFrame::setContigId(int contigId)
{
  if (contigId == m_contigId) { return; }

  if (m_bankname != "")
  {
    try
    {
      m_contigId = contigId;

      read_bank.open(m_bankname);
      contig_bank.open(m_bankname);

      Contig_t contig;
      contig_bank.fetch(m_contigId, contig);

      m_tiling = contig.getReadTiling();
      m_consensus = contig.getSeqString();

      m_cstatus.clear();
      m_cstatus.resize(m_consensus.size(), ' ');
      m_renderedSeqs.clear();

      sort(m_tiling.begin(), m_tiling.end(), RenderSeq_t::TilingOrderCmp());
      m_loaded = 1;
      m_loadedStart = -1;
      m_loadedEnd = -1;

      setGindex(0);

      emit setGindexRange(0, (int)m_consensus.size()-1);
      emit contigLoaded(m_contigId);

      QString s = "Viewing ";
      s += m_bankname.c_str();
      s += " with ";
      s += QString::number(contig_bank.getSize());
      s += " contigs";

      s += " Contig Id:";
      s += QString::number(m_contigId);
      s += " Size: ";
      s += QString::number(m_consensus.size());
      s += " Reads: ";
      s += QString::number(m_tiling.size());

      emit setStatus(s);
      repaint();
    }
    catch (Exception_t & e)
    {
      cerr << "ERROR: -- Fatal AMOS Exception --\n" << e;
    }
  }
}


void TilingFrame::setBankname(string bankname)
{
  if (bankname != "")
  {
    try
    {
      m_bankname = bankname;
      contig_bank.open(m_bankname);
    }
    catch (Exception_t & e)
    {
      cerr << "ERROR: -- Fatal AMOS Exception --\n" << e;
    }

    emit contigRange(1, contig_bank.getSize());
    setContigId(1);
  }
}

void TilingFrame::setFontSize(int fontsize )
{
  if (fontsize == m_fontsize) { return; }
  m_fontsize = fontsize;

  emit fontSizeChanged(m_fontsize);
  repaint();
}

void TilingFrame::setGindex( int gindex )
{
  //cerr << "TF:setgindex: " << gindex << endl;
  if (!m_loaded) { return; }

  int basespace = 5;
  int basewidth = m_fontsize + basespace;
  int tilehoffset = m_fontsize*10;
  m_displaywidth = (width()-tilehoffset)/basewidth;

  gindex = min(gindex, m_consensus.size()-m_displaywidth+1);
  m_gindex = gindex;

  int grangeStart = m_gindex;
  int grangeEnd   = min(m_gindex + m_displaywidth+200, (int)m_consensus.length());

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
        rendered.load(read_bank, &*vi);
        if (m_displayAllChromo) { rendered.loadTrace(m_db); rendered.m_displayTrace = true; }
        m_renderedSeqs.push_back(rendered);

        for (int gindex = rendered.m_loffset; gindex <= rendered.m_roffset; gindex++)
        {
          if      (m_cstatus[gindex] == ' ')                   
            { m_cstatus[gindex] = rendered.base(gindex); }

          else if (m_cstatus[gindex] != rendered.base(gindex)) 
            { m_cstatus[gindex] = 'X'; }
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

  repaint();
  emit gindexChanged(m_gindex);
}

void TilingFrame::setDB(const QString & db)
{
  m_db = db.ascii();
}

void TilingFrame::trackGindex(int gindex)
{
  //cerr << "tracking: " << gindex << endl;
  m_gindex = gindex;
  m_consfield->repaint();
  emit gindexChanged( m_gindex );
}

void TilingFrame::trackGindexDone()
{
  //cerr << "track done: " << m_gindex << endl;
  setGindex(m_gindex);
  repaint();
}

void TilingFrame::toggleStable(bool stable)
{
  //cerr << "frame::toggle " << (stable ? "true" : "false") << endl;
  m_tilingfield->toggleStable(stable);
}

void TilingFrame::toggleDisplayQV(bool show)
{
  m_tilingfield->toggleDisplayQV(show);
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
      ri->loadTrace(m_db);
      ri->m_displayTrace = true;
    }

    repaint();
  }
  else
  {
    m_loadedWidth = 5000;
  }
}

void TilingFrame::showInserts()
{
  InsertWindow * insertWindow = new InsertWindow(m_bankname, m_contigId, this, "insertWindow");
  insertWindow->show();

  connect(insertWindow, SIGNAL(setGindex(int)),
          this,         SLOT(setGindex(int)));

  connect(m_tilingfield, SIGNAL(setTilingVisibleRange(int, int)),
          insertWindow,  SIGNAL(setTilingVisibleRange(int, int)));

  m_tilingfield->repaint();
}


