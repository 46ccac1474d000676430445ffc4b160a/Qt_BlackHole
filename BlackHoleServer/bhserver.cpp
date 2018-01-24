#include "bhserver.h"


BHServer::BHServer(quint16 port, QObject *parent) :
    QTcpServer(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("Admin");
    db.setDatabaseName("db");
    db.setUserName("Admin");
    db.setPassword("");
    if (!db.open()) throw db.lastError();

    QSqlQuery q;
    if (!q.exec("create table if not exists Users (Login TEXT PRIMARY KEY, PasswordHash BLOB)")) throw q.lastError();

    if (!listen(QHostAddress::Any, port)) throw errorString();

    connect(this, &BHServer::newConnection, this, &BHServer::on_newConnection);

    qDebug() << "Server started";
}


void BHServer::on_newConnection()
{
    qDebug() << "New connection";

    QThread *thread = new QThread(this);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    QTcpSocket *sock = nextPendingConnection();
    sock->setParent(0); //VERY IMPORTAINT
    sock->moveToThread(thread); //VERY IMPORTAINT

    ClientThread *ct = new ClientThread(sock, &db);
    ct->moveToThread(thread);
    connect(thread, &QThread::started, ct, &ClientThread::run);

    thread->start();
}
