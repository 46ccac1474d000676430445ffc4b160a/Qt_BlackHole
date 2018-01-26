#include "serverdirexplorer.h"

ServerDirExplorer::ServerDirExplorer(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    socket = new Socket(this);
    connect(socket, &Socket::dataReceived, this, &ServerDirExplorer::parseData);

//list widget
    listWgt = new QListWidget();
    listWgt->setStyleSheet(
                    "QListWidget {"
                    "background: #eee;"
                    "border: none;"
                    "}"

                    "QListWidget::item {"
                    "background: #eee;"
                    "border-bottom: 1px solid #009eeb;"
                    "padding: 20px;"
                    "}"

                    "QListWidget::item:selected {"
                    "background: #a2e8d6;"
                    "color: black;"
                    "}"
                    );
    connect(listWgt, &QListWidget::itemDoubleClicked, this, &ServerDirExplorer::on_itemClicked);

//tool buttons
    //back button
    backBut = new QToolButton();
    backBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowBack));
    connect(backBut, &QToolButton::clicked, this, &ServerDirExplorer::on_backButTriggered);
    //forward button
    forwardBut = new QToolButton();
    forwardBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowForward));
    connect(forwardBut, &QToolButton::clicked, this, &ServerDirExplorer::on_forwardButTriggered);
    //goto button
    gotoBut = new QToolButton();
    gotoBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    connect(gotoBut, &QToolButton::clicked, this, &ServerDirExplorer::on_gotoButTriggered);
    connect(gotoBut, &QToolButton::clicked, [=](){ gotoBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)); });

//line edit
    dirLineEdit = new QLineEdit();
    connect(dirLineEdit, &QLineEdit::textEdited, [=](){ gotoBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowForward)); });


//layout settings
    //navigation bar Hlayout
    QHBoxLayout *navBarLay = new QHBoxLayout();
    navBarLay->addWidget(backBut);
    navBarLay->addWidget(forwardBut);
    navBarLay->addWidget(dirLineEdit);
    navBarLay->addWidget(gotoBut);

    //main Vlayout
    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addLayout(navBarLay);
    vlay->addWidget(listWgt);

//directory settings
    //TODO
    listDir("/");

    this->setLayout(vlay);

    QRect r = QDesktopWidget().geometry();
    int x = r.width()/2 - 400;
    int y = r.height()/2 - 275;

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(x, y, 800, 550);
    this->setStyleSheet("* { outline: none; }");
    this->show();
}


void ServerDirExplorer::closeEvent(QCloseEvent *event)
{
    parentWidget()->show(); //restores parent widget
    QWidget::closeEvent(event);
}


void ServerDirExplorer::listDir(const QString &path)
{
    QSettings settings(CONFILE, QSettings::IniFormat);
    socket->connectToHost(QHostAddress(settings.value(SERVERADDRESS, "127.0.0.1").toString()), settings.value(SERVERPORT, 56565).toInt());

    QByteArray req;
    QDataStream s(&req, QIODevice::ReadWrite);

    if (!authorized) authorization(settings.value(LOGIN).toString(), settings.value(PASSWORD).toString());

    s << Authorized << ClientAskedDirList << path;

    socket->sendData(req);
}


void ServerDirExplorer::fillListWidget(const QStringList &list)
{
    listWgt->clear();

    for(QString dir : list)
    {
        listWgt->addItem(dir);
    }
}


void ServerDirExplorer::authorization(const QString &login, const QString &password)
{
    QByteArray req;
    QDataStream s(&req, QIODevice::ReadWrite);

    s << Authorization << login << password;

    QSettings settings(CONFILE, QSettings::IniFormat);

    if (socket->tcpSocket()->state() != QTcpSocket::ConnectingState
            ||
        socket->tcpSocket()->state() != QTcpSocket::ConnectedState)
            socket->connectToHost(QHostAddress(settings.value(SERVERADDRESS, "127.0.0.1").toString()), settings.value(SERVERPORT, 56565).toInt());

    socket->sendData(req);
}


void ServerDirExplorer::parseData(const QByteArray &bytes)
{
    QByteArray data = bytes;
    QDataStream stream(&data, QIODevice::ReadWrite);

    int code;
    stream >> code;

    switch(code)
    {
        case AuthorizationPassed:
        {
            authorized = true;
        }


        case AuthorizationFailed:
        {
            authorized = false;
        }


        case DirExists:
        {
            QStringList s_list;
            stream >> s_list;
            fillListWidget(s_list);
        }


    }

}


void ServerDirExplorer::on_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
}


void ServerDirExplorer::on_backButTriggered()
{

}


void ServerDirExplorer::on_forwardButTriggered()
{

}


void ServerDirExplorer::on_gotoButTriggered()
{

}
