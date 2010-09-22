#include "DTFile.h"
#include "DTXor.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QTextCodec>
#include <QtCore/QDebug>
#include <QtCore/QFile>

using namespace DT;

DTFile::DTFile()
{
  m_dt = new DataTable();
  m_xorKey = XorKeys::NONE;
}

int DTFile::load(QString fileName)
{
  if(!QFile::exists(fileName)) {
    return Errors::FILE_NOT_FOUND;
  }

  m_file.setFileName(fileName);
  if(m_file.open(QIODevice::ReadOnly)) {
    m_str.setDevice(&m_file);
    m_str.setByteOrder(QDataStream::LittleEndian);

    int err = loadHeader();
    if(err != Errors::SUCCESS) {
      m_file.close();
      return err;
    }
    //qDebug() << "Header loaded";

    err = loadDescriptor();
    if(err != Errors::SUCCESS) {
      m_file.close();
      return err;
    }
    //qDebug() << "Columns loaded";

    err = loadRows();
    if(err != Errors::SUCCESS) {
      m_file.close();
      return err;
    }
    //qDebug() << "Rows loaded";

    m_file.close();
  } else {
    return Errors::FILE_ERROR;
  }

  return Errors::SUCCESS;
}

int DTFile::loadHeader()
{
  QString sig = m_str.device()->read(5);
  if(sig == "DTBIN") {
    qint8 verMaj;
    m_str >> verMaj;
    if((char)verMaj == '3') {
      qint8 verMin;
      m_str >> verMin;

      m_dt->setVersion(verMaj, verMin);
      //qDebug() << "ver: " << (char)verMaj << "." << (char)verMin;

      int fileFlag;
      m_str >> fileFlag;
      m_dt->setFileFlag(fileFlag);
      //qDebug() << "fileFlag: " << fileFlag;

      int rowCount;
      m_str >> rowCount;
      m_dt->setFileRowCount(rowCount);
      //qDebug() << "rowCount: " << rowCount;

      int dataSize;
      m_str >> dataSize;
      //qDebug() << "dataSize: " << dataSize;

      if(dataSize > 0) {
        int dataRead = 4;

        int dataFlag;
        m_str >> dataFlag;
        //qDebug() << "dataFlag: " << dataFlag;

        if(dataFlag & DataFlags::LAST_ID) {
          int lastId;
          m_str >> lastId;
          m_dt->setLastId(lastId);
          //qDebug() << "  lastId: " << lastId;

          dataRead += 4;
        }

        if(dataFlag & DataFlags::PASSWORD) {
          QByteArray hash = m_str.device()->read(16);
          m_dt->setPassHash(hash);
          //qDebug() << "  passHash: " << hash.toHex();

          dataRead += 16;
        }

        if(dataFlag & DataFlags::DB_ID) {
          int dbId;
          m_str >> dbId;
          Q_UNUSED(dbId);
          //qDebug() << "  dbId: " << dbId;

          dataRead += 4;
        }

				if(dataFlag & DataFlags::PARAMS) {
					int paramCount;
					m_str >> paramCount;
					//qDebug() << "  paramCount: " << paramCount;

					for (int i = 0; i < paramCount; i++) {
						/*String key = readString(&dataLeft);
						String value = readString(&dataLeft);
						if (_table->paramExists(key) == false) {
							_table->setParam(key, value);
						}*/
					}

					dataRead += 4;
				}

				if(dataFlag & DataFlags::CREATED) {
					qint64 created;
					m_str >> created;
					//qDebug() << "  created: " << created;
					//TODO

					dataRead += 4;
				}

				if(dataFlag & DataFlags::MODIFIED) {
					qint64 modified;
					m_str >> modified;
					//qDebug() << "  modified: " << modified;
					//TODO

					dataRead += 4;
				}

				if(dataFlag & DataFlags::LAST_BACKUP) {
					qint64 lastBackup;
					m_str >> lastBackup;
					//qDebug() << "  lastBackup: " << lastBackup;
					//TODO

					dataRead += 4;
				}

				if(dataFlag & DataFlags::PASS_SALT) {
					int passSalt;
					m_str >> passSalt;
					m_dt->setPassSalt(passSalt);
					//qDebug() << "  passSalt: " << passSalt;

					dataRead += 4;
				}

				if(dataFlag & DataFlags::XOR_SALT) {
					int xorSalt;
					m_str >> xorSalt;
					m_dt->setXorSalt(xorSalt);
					//qDebug() << "  xorSalt: " << xorSalt;

					dataRead += 4;
				}

				if(dataRead < dataSize) {
					//qDebug() << " dataLeft: " << (dataSize - dataRead);
					m_str.device()->read(dataSize - dataRead);
				}
			}
		} else {
			return Errors::BAD_VERSION;
		}
	} else {
		return Errors::BAD_FORMAT;
	}

	return Errors::SUCCESS;
}

int DTFile::loadDescriptor()
{
  int columnCount;
  m_str >> columnCount;
  //qDebug() << "columnCount: " << columnCount;

  if(columnCount > 0) {
    for(int i = 0; i < columnCount; i++) {
      DTColumn *col = new DTColumn(m_dt);

      int columnId;
      m_str >> columnId;
      col->setId(columnId);

      int columnType;
      m_str >> columnType;
      col->setType(columnType);

      qint8 nameLen;
      m_str >> nameLen;

      if(nameLen > 0) {
        QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
        QString columnName = codec->toUnicode(m_str.device()->read(nameLen));
        col->setName(columnName);
      }

      //qDebug() << columnId << col->getType() << columnName;

      int dataSize;
      m_str >> dataSize;

      if(dataSize > 0) {
        int dataFlag;
        m_str >> dataFlag;
      }

      m_dt->insertColumn(col);
      //qDebug() << "column " << i << " loaded";
    }

    //m_dt->sortColumns();
  }

  return Errors::SUCCESS;
}
int DTFile::loadRows()
{
  //qDebug() << "rowCount: " << m_dt->getFileRowCount();
  if(m_dt->getFileRowCount() > 0) {
    for(int i = 0; i < m_dt->getFileRowCount(); i++) {
      DTRow *row = new DTRow(m_dt);

      qint8 rowStart;
      m_str >> rowStart;

      if(rowStart == '\n') {
        int rowSize;
        m_str >> rowSize;
        //qDebug() << "  rowSize: " << rowSize;
        if(rowSize > m_str.device()->bytesAvailable()) {
          return Errors::BAD_FORMAT;
        }

        m_str.device()->seek(m_str.device()->pos() + rowSize);

        int rowSize2;
        m_str >> rowSize2;
        //qDebug() << "  rowSize2: " << rowSize2;
        if(rowSize != rowSize2) {
          return Errors::BAD_FORMAT;
        }

        m_str.device()->seek(m_str.device()->pos() - (rowSize + 4));
        int rowFlag;
        m_str >> rowFlag;
        //qDebug() << "  rowFlag: " << rowFlag;
        row->setFlag(rowFlag);

        int dataSize;
        m_str >> dataSize;
        //qDebug() << "  dataSize: " << dataSize;

        if(dataSize > rowSize - 8) {
          return Errors::BAD_FORMAT;
        }

        if(dataSize > 0) {
          int dataRead = 4;

          int dataFlag;
          m_str >> dataFlag;
          //qDebug() << "  dataFlag: " << dataFlag;

          if(dataFlag & RowFlags::ROW_ID) {
            int rowId;
            m_str >> rowId;
            //qDebug() << "    rowId: " << rowId;
            row->setId(rowId);

            dataRead += 4;
          }

          if(dataRead < dataSize) {
            //qDebug() << " dataLeft: " << (dataSize - dataRead);
            m_str.device()->read(dataSize - dataRead);
          }
        }

        for(int i = 0; i < m_dt->getColumnCount(); i++) {
          DTColumn *col = m_dt->getColumn(i);

          if(col->hasFlag(ColumnFlags::DONT_SAVE)) {
            //qDebug() << "skipping" << col->getName();
            continue;
          }

          bool crypt = col->hasFlag(ColumnFlags::XOR) || (m_dt->hasFileFlag(FileFlags::CRYPT_ALL));
          switch(col->getType()) {
            case ColumnTypes::INT:
            {
              int val;
              m_str >> val;

              if(crypt) {
                ///qDebug() << "crypted:";
                //val = xor_decrypt(val);
              }

              //qDebug() << col->getName() << " int: " << val;
              row->setInt(col->getId(), val);
            }
            break;

            case ColumnTypes::INT64:
            {
              qint64 val;
              m_str >> val;
              if(crypt) {
                ///qDebug() << "crypted:";
                //val = xorDecryptInt(val);
              }

              //qDebug() << col->getName() << " int64: " << val;
              row->setInt64(col->getId(), val);
            }
            break;

            case ColumnTypes::DOUBLE:
            {
              qreal val;
              m_str >> val;
              if(crypt) {
                //qDebug() << "crypted:";
                //val = xorDecryptDouble(val);
              }

              //qDebug() << col->getName() << "double: " << val;
              row->setDouble(col->getId(), val);
            }
            break;

            case ColumnTypes::STRING:
            {
              int strLen;
              m_str >> strLen;

              if(strLen > 0) {
                QByteArray data = m_str.device()->read(strLen);
                if(crypt) {
                  if(m_xorKey > XorKeys::NONE) {
                    data =  DTXor::decryptString(m_xorKey, data);
                  }
                }

                //qDebug() << col->getName() << "string[" << strLen << "]: " << val;
                QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
                row->setString(col->getId(), codec->toUnicode(data.data()));
              }
            }
            break;

            case ColumnTypes::BIN:
            {
              //qDebug() << col->getName() << "bin: ";
            }
            break;

            default:
              //qDebug() << col->getName() << "wtf: ";
            break;
          }

          quint8 tab;
          m_str >> tab;
          if(tab != '\t') {
            return Errors::BAD_FORMAT;
          }
        }

        m_dt->insertRow(row);

        m_str >> rowSize2;
        if(rowSize2 != rowSize) {
          return Errors::BAD_FORMAT;
        }
      } else {
        if(m_str.device()->bytesAvailable() == 0) {
          return Errors::NOTHING_TO_READ;
        }

        return Errors::BAD_FORMAT;
      }

      //qDebug() << "loaded rows %:" << (((double)i / (double)m_dt->getFileRowCount()) * 100);
    }
  }

  return Errors::SUCCESS;
}
