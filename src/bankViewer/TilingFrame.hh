#ifndef TILINGFRAME_HH_
#define TILINGFRAME_HH_ 1

#include <qframe.h>
#include <string>
#include "RenderSeq.hh"
#include "TilingField.hh"
#include "ConsensusField.hh"
#include <qscrollview.h>

#include "DataStore.hh"



class TilingFrame: public QFrame
{
  Q_OBJECT

public:
  TilingFrame::TilingFrame(DataStore * datastore,
                           QWidget * parent = 0,
                           const char * name = 0,  
                           WFlags f = 0);
public slots:
    void setGindex(int gindex);
    void setContigId(int contigId);
    void setFontSize(int);
    void toggleStable(bool);
    void toggleShowNumbers(bool);
    void toggleHighlightDiscrepancy(bool);
    void toggleDisplayQV(bool);
    void toggleDisplayAllChromo(bool);
    void toggleLowQualityLowerCase(bool);
    void toggleBaseColors(bool);
    void toggleShowFullRange(bool);
    void toggleShowIndicator(bool);
    void advanceNextDiscrepancy();
    void advancePrevDiscrepancy();
    void highlightRead(int);

signals:
    void gindexChanged( int );
    void setGindexRange( int, int );
    void contigLoaded(int);
    void setStatus(const QString &);
    void fontSizeChanged(int);
    void setShowNumbers(bool);
    void setHighlightDiscrepancy(bool);
    void setDisplayQV(bool);
    void setTilingVisibleRange(int, int);

protected:
    void paintEvent(QPaintEvent *);

private:

    void loadContigRange(int gindex);

    int m_fontsize;
    int m_gindex;
    int m_displaywidth;

    bool m_displayAllChromo;

    int m_loadedStart;
    int m_loadedEnd;
    int m_loadedWidth;

    QScrollView * m_sv;

    std::vector<AMOS::Tile_t> m_tiling;
    std::string m_consensus;
    std::string m_cstatus;

    DataStore * m_datastore;
    std::vector<RenderSeq_t> m_renderedSeqs;

    TilingField * m_tilingfield;
    ConsensusField * m_consfield;
};


#endif
