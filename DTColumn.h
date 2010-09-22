#ifndef DTCOLUMN_H
#define DTCOLUMN_H

#include "DT.h"

#include <QtCore/QString>

namespace DT
{
  class DataTable;

  class DTColumn
  {
    public:
      DTColumn(DataTable *owner);

      void setId(int id) { m_id = id; }
      void setType(int type) { m_type = type; }
      void setName(QString name) { m_name = name; }
      void setFlag(int flag);
      void unsetFlag(int flag);

      int getId() { return m_id; }
      int getType() { return (m_type & DT::TYPE_MASK); }
      int getFlags() { return m_type; }
      QString getName() { return m_name; }
      bool hasFlag(int flag) { return (m_type & flag); }

      bool isIdUnique() { return (m_id & DT::UNIQUE_ID); }

    private:
      int m_id;
      int m_type;
      QString m_name;
      DataTable *m_dt;
  };
};

#endif // DTCOLUMN_H
