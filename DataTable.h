#ifndef DATATABLE_H
#define DATATABLE_H

#include "DTColumn.h"
#include "DTRow.h"

namespace DT
{
  class DataTable
  {
    public:
      DataTable();

      void setVersion(char verMaj, char verMin) { m_verMaj = verMaj; m_verMin = verMin; }
      void setFileFlag(int fileFlag) { m_fileFlag = fileFlag; }
      void setFileRowCount(int rowCount) { m_fileRowCount = rowCount; }
      void setLastId(int lastId) { m_lastId = lastId; }
      void setPassSalt(int passSalt) { m_passSalt = passSalt; }
      void setPassHash(QString passHash) { m_passHash = passHash; }
      void setXorSalt(int xorSalt) { m_xorSalt = xorSalt; }

      void insertColumn(DTColumn *col);
      void sortColumns();

      void insertRow(DTRow *row);

      char getVersionMajor() { return m_verMaj; }
      char getVersionMinor() { return m_verMin; }
      int getFileFlag() { return m_fileFlag; }
      int getFileRowCount() { return m_fileRowCount; }
      int getLastId() { return m_lastId; }
      int getPassSalt() { return m_passSalt; }
      QString getPassHash() { return m_passHash; }
      int getXorSalt() { return m_xorSalt; }
      bool hasFileFlag(int flag) { return (m_fileFlag & flag); }
      int getColumnCount() { return m_columns.size(); }
      int getSaveColumnCount();
      int getRowCount() { return m_rows.size(); }

      DTColumn *getColumn(int i);
      DTColumn *getColumnById(int id);
      int getColumnIdByName(QString name);

      DTRow *getRow(int i);
      DTRow *getRowById(int id);

      QVariant getValue(int col, int row) { return m_rows.at(row)->getValue(col); }
      int getInt(int col, int row) { return m_rows.at(row)->getInt(col); }
      QString getString(int col, int row) { return m_rows.at(row)->getString(col); }

      static QString typeToString(int type);

    private:
      QList<DTColumn*> m_columns;
      QList<DTRow*> m_rows;

      char m_verMin;
      char m_verMaj;
      int m_fileFlag;
      int m_fileRowCount;
      int m_lastId;
      int m_passSalt;
      QString m_passHash;
      int m_xorSalt;
  };
};

#endif // DATATABLE_H
