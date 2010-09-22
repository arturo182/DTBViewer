#include "wndMain.h"
#include "ui_wndMain.h"

#include "DTFile.h"

#include <QtCore/QDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

using namespace DT;

wndMain::wndMain(QWidget *parent): QMainWindow(parent), m_ui(new Ui::wndMain)
{
  m_ui->setupUi(this);

  m_lbXor = new QLabel(" XOR: Config ");
  statusBar()->addPermanentWidget(m_lbXor, 0);

  m_lbCols = new QLabel(" COLS: 0 ");
  statusBar()->addPermanentWidget(m_lbCols, 0);

  m_lbRows = new QLabel(" ROWS: 0 ");
  statusBar()->addPermanentWidget(m_lbRows, 1);
}

wndMain::~wndMain()
{
  delete m_ui;
}

void wndMain::on_actLoad_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(0, "Choose .dtb file.", "", "DataTable files (*.dtb);;All files(*.*)");

  if(!fileName.isEmpty()) {
    DTFile file;

    if(m_ui->actConfigKey->isChecked()) {
      file.setXorKey(XorKeys::CONFIG);
    } else {
      file.setXorKey(XorKeys::HISTORY);
    }

    int error = file.load(fileName);
    if(error != Errors::SUCCESS) {
      qDebug() << "ERROR! " << error;
    }
    //qDebug() << "File loaded";

    DataTable *table = file.getTable();

    int colCount = table->getColumnCount();
    int rowCount = table->getRowCount();

    m_lbCols->setText(" COLS: " + QString::number(colCount) + " ");
    m_lbRows->setText(" ROWS: " + QString::number(rowCount) + " ");

    m_ui->twMain->clear();
    m_ui->twMain->setColumnCount(colCount);
    m_ui->twMain->setRowCount(rowCount);

    QStringList columns;
    QStringList rows;

    for(int row = 0; row < rowCount; row++) {
      for(int col = 0; col < colCount; col++) {
        DTColumn *column = table->getColumn(col);

        QTableWidgetItem *item = new QTableWidgetItem;

        item->setText(table->getString(column->getId(), row));
        m_ui->twMain->setItem(row, col, item);

        if(columns.count() < colCount) {
          columns << DataTable::typeToString(column->getType()) + "\n\"" + column->getName() + "\"" + ((column->hasFlag(ColumnFlags::XOR))?"\nCRYPTED":"");
        }

        //qDebug() << "creating table %:" << (((double)(row) / (double)(table->getRowCount())) * 100);
      }

      rows << QString::number(table->getRow(row)->getId());
    }

    m_ui->twMain->setHorizontalHeaderLabels(columns);
    m_ui->twMain->setVerticalHeaderLabels(rows);

    m_ui->twMain->resizeColumnsToContents();
    m_ui->twMain->resizeRowsToContents();
  }
}

void wndMain::on_actConfigKey_triggered()
{
  m_ui->actConfigKey->setChecked(true);
  m_ui->actHistoryKey->setChecked(false);
  m_lbXor->setText(" XOR: Config ");
}

void wndMain::on_actHistoryKey_triggered()
{
  m_ui->actHistoryKey->setChecked(true);
  m_ui->actConfigKey->setChecked(false);
  m_lbXor->setText(" XOR: History ");
}
