#ifndef DTFILE_H
#define DTFILE_H

#include "DataTable.h"

#include <QtCore/QDataStream>
#include <QtCore/QFile>

namespace DT
{
  class DTFile
  {
    public:
      DTFile();

      DataTable *getTable() { return m_dt; }

      int load(QString fileName);

      int getXorKey() { return m_xorKey; }
      void setXorKey(int xorKey) { m_xorKey = xorKey; }

    private:
      int loadHeader();
      int loadDescriptor();
      int loadRows();

    private:
      DataTable *m_dt;
      QFile m_file;
      QDataStream m_str;
      int m_xorKey;
  };
};

#endif // DTFILE_H
