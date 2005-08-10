#ifndef COVERAGE_CANVAS_ITEM_HH_
#define COVERAGE_CANVAS_ITEM_HH_ 1

#include <qcanvas.h>
#include <vector>

class CoverageCanvasItem : public QCanvasRectangle
{
public:
  CoverageCanvasItem(int x, int y, int width, int height, 
                     int libid, double baseLevel,
                     QPointArray & arr, 
                     std::vector<double> & raw,
                     bool copyRaw,
                     QCanvas * canvas,
                     const QColor & color);

  static const int RTTI;
  int rtti () const { return RTTI; }

  QPointArray         m_points;
  std::vector<double> m_raw;

  int                 m_libid;
  double              m_baseLevel;
  QColor              m_color;
  bool                m_copyraw;


protected:
  void drawShape (QPainter & p);

};


#endif
