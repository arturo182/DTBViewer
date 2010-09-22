#include "wndMain.h"

#include "DTFile.h"
#include "DTXor.h"

#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QDebug>

using namespace DT;

int main(int argc, char *argv[])
{
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QApplication a(argc, argv);

  wndMain wnd;
  wnd.show();

  return a.exec();
}
