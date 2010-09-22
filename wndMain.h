#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QtGui/QMainWindow>
#include <QtGui/QLabel>

namespace Ui
{
  class wndMain;
}

class wndMain: public QMainWindow
{
  Q_OBJECT

  public:
    explicit wndMain(QWidget *parent = 0);
    ~wndMain();

  private slots:
    void on_actLoad_triggered();
    void on_actConfigKey_triggered();
    void on_actHistoryKey_triggered();

  private:
    Ui::wndMain *m_ui;
    QLabel *m_lbXor;
    QLabel *m_lbCols;
    QLabel *m_lbRows;
};

#endif // WNDMAIN_H
