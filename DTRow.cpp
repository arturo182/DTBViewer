#include "DataTable.h"
#include "DTRow.h"
#include "DT.h"

using namespace DT;

DTRow::DTRow(DataTable *owner)
{
  m_id = 0;
  m_flag = RowFlags::NONE;
  m_dt = owner;
}
