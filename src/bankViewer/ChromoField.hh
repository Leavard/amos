#ifndef CHROMOFIELD_HH_
#define CHROMOFIELD_HH_

#include <qwidget.h>
#include <qpixmap.h>
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

#include <Read.h>

#ifdef __cplusplus
}
#endif

class RenderSeq_t;

class ChromoField: public QWidget
{
  Q_OBJECT
public:
  ChromoField(RenderSeq_t * read, 
              const std::string & db,
              const std::string & cons,
              const std::string & cstatus,
              QWidget *parent=0, 
              const char *name=0);

  int getWindowPos(int gindex);

protected:
  void paintEvent( QPaintEvent * );

private:
  RenderSeq_t * m_read;

  QPixmap * m_pix;

  double m_hscale;
  int    m_hoffset;
};



#endif
