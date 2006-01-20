#ifndef CONTIG_PICKER_HH__
#define CONTIG_PICKER_HH__ 1

#include <string>
#include <qmainwindow.h>
#include <qlistview.h>
#include <qpopupmenu.h>

using std::string;

class DataStore;

class ContigPicker : public QMainWindow
{
  Q_OBJECT

public:
  ContigPicker(DataStore * datastore, QWidget * parent, const char * name);

public slots:
  void itemSelected(QListViewItem * item);
  void selectiid(const QString & iid);
  void selecteid(const QString & eid);
  void acceptSelected();
  void toggleShowReads();
  void refreshTable();

signals:
  void contigSelected(int);
  void setGindex(int);

private:
  QListView * m_table;

  void loadTable(bool jumpToCurrent);
  bool m_showReads;
  int m_showreadsid;
  QPopupMenu * m_options;
  DataStore * m_datastore;

};


#endif
