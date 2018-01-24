#ifndef RUN_H
#define RUN_H

#include <QTcpSocket>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDir>

#include <QCryptographicHash>

#include <QException>

#include "../COMMON/socket.h"
#include "../COMMON/commands.h"

class ClientThread : public QObject
{
    Q_OBJECT

protected:
    QTcpSocket *tcpSock;
    Socket *socket;

    QSqlDatabase *db;

    QString login,
            password;

    QString path;

    bool authorized = false;


    bool isLoginFree(const QString &login);
    bool checkAuthorization(const QString &login, const QString &password);
    bool registrateUser(const QString &login, const QString &password);

    QByteArray commandToBytes(int code);

    QByteArray getDirList(QDir &dir, const QString &path);

    bool appendToFile(const QByteArray &data);


public:
    ClientThread(QTcpSocket *sock, QSqlDatabase *db);

public slots:
    void run();

    void parseData(const QByteArray &bytes);


};

#endif // RUN_H
