#include "DataTable.h"
#include "DTColumn.h"

#include <QtCore/QtAlgorithms>

using namespace DT;

bool lessThenColumns(DTColumn *c1, DTColumn *c2)
{
  return c1->getId() < c2->getId();
}

DataTable::DataTable()
{
  m_verMaj = 0;
  m_verMin = 0;
  m_fileFlag = FileFlags::NONE;
  m_fileRowCount = 0;
  m_lastId = 0;
  m_passSalt = 0;
  m_xorSalt = 0;
}

void DataTable::insertColumn(DTColumn *col)
{
  m_columns.append(col);
}

void DataTable::sortColumns()
{
  qSort(m_columns.begin(), m_columns.end(), lessThenColumns);
}

void DataTable::insertRow(DTRow *row)
{
  m_rows.append(row);
}

int DataTable::getSaveColumnCount()
{
  int count = 0;
  for(int i = 0; i < m_columns.count(); i++) {
    if(!m_columns.at(i)->hasFlag(ColumnFlags::DONT_SAVE)) {
      count++;
    }
  }

  return count;
}

DTColumn *DataTable::getColumn(int i)
{
  if(m_columns.count() == 0) {
    return 0;
  }

  if(m_columns.count() <= i) {
    return 0;
  }

  return m_columns.at(i);
}

DTColumn *DataTable::getColumnById(int id)
{
  for(int i = 0; i < m_columns.count(); i++) {
    if(m_columns.at(i)->getId() == id) {
      return m_columns.at(i);
    }
  }

  return 0;
}

int DataTable::getColumnIdByName(QString name)
{
  for(int i = 0; i < m_columns.count(); i++) {
    if(m_columns.at(i)->getName() == name) {
      return m_columns.at(i)->getId();
    }
  }

  return -1;
}

DTRow *DataTable::getRow(int i)
{
  if(m_rows.count() == 0) {
    return 0;
  }

  if(m_rows.count() <= i) {
    return 0;
  }

  return m_rows.at(i);
}

DTRow *DataTable::getRowById(int id)
{
  for(int i = 0; i < m_rows.count(); i++) {
    if(m_rows.at(i)->getId() == id) {
      return m_rows.at(i);
    }
  }

  return 0;
}

QString DataTable::typeToString(int type)
{
  switch(type) {
    case ColumnTypes::INT:
      return "int";
    break;

    case ColumnTypes::INT64:
      return "int64";
    break;

    case ColumnTypes::DOUBLE:
      return "double";
    break;

    case ColumnTypes::STRING:
      return "string";
    break;

    case ColumnTypes::BIN:
      return "bin";
    break;

    case ColumnTypes::UNKNOWN:
      return "unknown";
    break;
  }

  return "wtf";
}
