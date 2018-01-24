#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QThread>

class Socket : public QObject
{
    Q_OBJECT

protected:
    QTcpSocket *socket;

    int MSize = -1;

protected slots:
    void on_readyRead();

public:
    Socket(QObject *parent = nullptr);
    Socket(qintptr sockDescriptor, QObject *parent);
    Socket(QTcpSocket *sock, QObject *parent);

    QTcpSocket *tcpSocket();

    void connectToHost(const QHostAddress &host, quint16 port);
    void disconnectFromHost();

signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray &);

public slots:
    void sendData(const QByteArray &data);

};

#endif // SOCKET_H
