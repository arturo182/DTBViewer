#ifndef DTROW_H
#define DTROW_H

#include "DT.h"

#include <QtCore/QVariant>


namespace DT
{
  class DataTable;

  class DTRow
  {
    public:
      DTRow(DataTable *owner);

      void setValue(int column, QVariant value) { m_values.insert(column, value); }
      void setInt(int column, int value) { m_values.insert(column, value); }
      void setInt64(int column, qint64 value) { m_values.insert(column, value); }
      void setDouble(int column, double value) { m_values.insert(column, value); }
      void setString(int column, QString value) { m_values.insert(column, value); }
      void setId(int id) { m_id = id; }
      void setFlag(int flag) { m_flag = (m_flag | flag); }
      void unsetFlag(int flag) { m_flag = (m_flag & (~flag)); }

      QVariant getValue(int column) { return m_values.value(column); }
      int getInt(int column) { return m_values.value(column).toInt(); }
      qint64 getInt64(int column) { return m_values.value(column).toLongLong(); }
      double getDouble(int column) { return m_values.value(column).toReal(); }
      QString getString(int column) { return m_values.value(column).toString(); }
      int getId() { return m_id; }
      int getFlags() { return m_flag; }
      DataTable *getOwner() { return m_dt; }
      bool hasFlag(int flag) const { return (m_flag & flag); }

    private:
      int m_id;
      int m_flag;
      DataTable *m_dt;
      QMap<int, QVariant> m_values;
  };
};

#endif // DTROW_H
