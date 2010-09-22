#include "DTColumn.h"
#include "DataTable.h"

#include <QtCore/QDebug>

using namespace DT;

DTColumn::DTColumn(DataTable *owner)
{
	m_id = 0;
	m_type = 0;
	m_dt = owner;
}

void DTColumn::setFlag(int flag)
{
	flag = flag & ~DT::TYPE_MASK;
	m_type = m_type | flag;
}

void DTColumn::unsetFlag(int flag)
{
  flag = flag & ~DT::TYPE_MASK;
  m_type = m_type & (~flag);
}
