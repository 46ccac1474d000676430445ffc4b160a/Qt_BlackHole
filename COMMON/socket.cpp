#include "socket.h"


Socket::Socket(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Socket::connected);
    connect(socket, &QTcpSocket::disconnected, this, &Socket::disconnected);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(socket, &QTcpSocket::readyRead, this, &Socket::on_readyRead);
}

Socket::Socket(qintptr sockDescriptor, QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(sockDescriptor)) throw socket->errorString();
}

Socket::Socket(QTcpSocket *socket, QObject *parent) :
    QObject(parent)
{
    this->socket = socket;

    connect(socket, &QTcpSocket::connected, this, &Socket::connected);
    connect(socket, &QTcpSocket::disconnected, this, &Socket::disconnected);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(socket, &QTcpSocket::readyRead, this, &Socket::on_readyRead);
}

QTcpSocket* Socket::tcpSocket()
{
    return socket;
}

void Socket::connectToHost(const QHostAddress &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void Socket::disconnectFromHost()
{
    socket->disconnectFromHost();
}


void Socket::sendData(const QByteArray &data)
{
    QByteArray bytes;
    QDataStream s(&bytes, QIODevice::ReadWrite);
    s << data.size() << data;
    socket->write(bytes);
}


void Socket::on_readyRead()
{
    QDataStream s(socket);

    while (1)
        if (MSize < 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(int)) return;
            s >> MSize;
        }
        else
        {
            if (socket->bytesAvailable() < MSize) return;
            QByteArray bytes;
            s >> bytes;
            emit dataReceived(bytes);
            MSize = -1;
        }

}
