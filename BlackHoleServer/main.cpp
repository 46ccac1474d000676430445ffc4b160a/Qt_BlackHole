#include <QCoreApplication>

#include "bhserver.h"
#include <QSqlError>
#include <QException>

int main(int argc, char *argv[])
{
    try
    {
        QCoreApplication a(argc, argv);

        BHServer server(56565);

        return a.exec();
    }
    catch (const QString &err)
    {
        qDebug() << err;
    }
    catch(QSqlError &err)
    {
        qDebug() << err.text();
    }
    catch (QException &err)
    {
        qDebug() << "QException catched";
    }
    catch (...)
    {
        qDebug() << "Some shit happenes...";
    }

    return -1;
}
