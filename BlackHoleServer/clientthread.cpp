#include "clientthread.h"


ClientThread::ClientThread(QTcpSocket *sock, QSqlDatabase *db) :
    QObject()
{
    tcpSock = sock;
    this->db = db;
}


void ClientThread::run()
{
    socket = new Socket(tcpSock, this);

    connect(socket, &Socket::disconnected, this, &ClientThread::deleteLater);
    connect(socket, &Socket::dataReceived, this, &ClientThread::parseData);
}


bool ClientThread::isLoginFree(const QString &login)
{
    QSqlQuery q(*db);

    if (!q.exec(QString("select Login from Users where Login = '%1'").arg(login)  )) throw q.lastError();

    return !q.next();
}


bool ClientThread::checkAuthorization(const QString &login, const QString &password)
{
    QSqlQuery q(*db);

    if (!q.exec(
         QString("select PasswordHash from Users where Login = '%1'").arg(login)
        )) throw q.lastError();

    return q.next() && q.value(0).toByteArray() == QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5);;
}


bool ClientThread::registrateUser(const QString &login, const QString &password)
{
    QSqlQuery q(*db);

    q.prepare(QString("insert into Users (Login, PasswordHash) VALUES (:Login, :PasswordHash)"));
    q.bindValue(":Login", login);
    q.bindValue(":PasswordHash", QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5));

    QDir::current().mkdir(login);

    return q.exec();
}


QByteArray ClientThread::commandToBytes(int code)
{
    QByteArray bytes;
    QDataStream s(&bytes, QIODevice::WriteOnly);
    s << code;
    return bytes;
}


QByteArray ClientThread::getDirList(QDir &dir, const QString &path)
{
    QByteArray dirList;
    QDataStream s(&dirList, QIODevice::WriteOnly);

    if (dir.cd(path))
    {
        s << DirExists;
        s << dir.entryList();
    }
    else s << DirNotExists;

    return dirList;
}


bool ClientThread::appendToFile(const QByteArray &data)
{
    if (path.isEmpty() || login.isEmpty()) return false;
    QString fullPath = QDir::currentPath() + "/" + login + "/" + path;
    QFile f(fullPath);
    if (!f.open(QIODevice::ReadWrite | QIODevice::Append)) return false;
    bool success = f.write(data) == data.size();
    f.flush();
    f.close();
    return success;
}


void ClientThread::parseData(const QByteArray &bytes)
{
    QByteArray data = bytes;
    QDataStream stream(&data, QIODevice::ReadOnly);

    int code;
    QString _login, _password;
    stream >> code >> _login >> _password;

    switch (code)
    {
        case Authorization:
        {
            if (checkAuthorization(_login, _password))
            {
                this->login = _login;
                this->password = _password;
                authorized = true;

                socket->sendData(commandToBytes(AuthorizationPassed));
            }
            else
            {
                authorized = false;
                socket->sendData(commandToBytes(AuthorizationFailed));
            }

        } break;


        case Registration:
        {
            if (isLoginFree(_login) && registrateUser(_login, _password))
                socket->sendData(commandToBytes(RegistrationPassed));
            else socket->sendData(commandToBytes(RegistrationFailed));

        } break;


        case Authorized:
        {
            if (!authorized)
            {
                socket->sendData(commandToBytes(AuthorizationFailed));
                socket->tcpSocket()->waitForBytesWritten();
                socket->disconnectFromHost();
            }

            int t_code;
            stream >> t_code;

            switch(t_code)
            {

                case ClientSendPathName:
                {

                    stream >> path;
                    socket->sendData(commandToBytes(ServerAcceptedPath));

                } break;


                case ClientSendFile:
                {
                    QByteArray data;
                    stream >> data;
                    if (appendToFile(data))
                        socket->sendData(commandToBytes(PartOfFileAccepted));
                    else socket->sendData(commandToBytes(PartOfFileNotAccepted));

                } break;


                case ClientSendDir:
                {
                    //EMPTY
                } break;


                case ClientAskedDirList:
                {
                    QString _path;
                    stream >> _path;
                    QDir dir(QDir::currentPath()+"/"+login);
                    socket->sendData(getDirList(dir, _path));
                } break;


                case ClientAskedFile:
                {

                } break;


                case ClientAskedDir:
                {
                    //EMPTY
                } break;


                default:
                    break;

            }//switch(t_code)

        } break;


        default:
            break;

    }//switch(code)

}
