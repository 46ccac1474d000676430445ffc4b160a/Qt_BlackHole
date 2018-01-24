#include "BHWdget/bhwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    BHWidget wgt(150, 7);
    wgt.show();


    return a.exec();
}
