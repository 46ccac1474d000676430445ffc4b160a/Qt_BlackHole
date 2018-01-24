#ifndef BHSERVER_H
#define BHSERVER_H

#include <QTcpServer>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "clientthread.h"

class BHServer : public QTcpServer
{
    Q_OBJECT

protected:
    QSqlDatabase db;

public:
    BHServer(quint16 port, QObject *parent = nullptr);

public slots:
    void on_newConnection();

};

#endif // BHSERVER_H
