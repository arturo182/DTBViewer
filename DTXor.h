#ifndef DTXOR_H
#define DTXOR_H

#include "DT.h"

#include <QtCore/QString>

namespace DT
{
  class DTXor
  {
    public:
      static QString encryptString(QByteArray key, QString text);
      static QByteArray decryptString(int keyType, QByteArray str);

      static char keyHistory[11];
      static char keyConfig[19];
  };
};

#endif // DTXOR_H
