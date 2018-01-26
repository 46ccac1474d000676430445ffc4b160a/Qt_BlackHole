#include "regwidget.h"

RegWidget::RegWidget(QWidget *parent) :
  QWidget(parent, Qt::Tool)
{
    socket = new Socket(this);
    connect(socket, &Socket::dataReceived, this, &RegWidget::on_dataReceived);

    loginLine = new QLineEdit("User");
    passwordLine = new QLineEdit("password");
    passwordLine->setEchoMode(QLineEdit::Password);

    regBut = new QPushButton("&Registration");
    connect(regBut, &QPushButton::clicked, this, &RegWidget::on_regButClicked);
    exitBut = new QPushButton("&Exit");
    connect(exitBut, &QPushButton::clicked, this, &RegWidget::close);

    showPasswordCheck = new QCheckBox("&Show password");
    connect(showPasswordCheck, &QCheckBox::toggled,
            [=](bool ch)
            {
                if (ch) passwordLine->setEchoMode(QLineEdit::Normal);
                else passwordLine->setEchoMode(QLineEdit::Password);
            });

    QHBoxLayout *hButLay = new QHBoxLayout();
    hButLay->addWidget(regBut);
    hButLay->addWidget(exitBut);

    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addWidget(new QLabel("Login:"));
    vlay->addWidget(loginLine);
    vlay->addWidget(new QLabel("Password:"));
    vlay->addWidget(passwordLine);
    vlay->addWidget(showPasswordCheck);
    vlay->addLayout(hButLay);


    this->setLayout(vlay);

    QRect r = QDesktopWidget().geometry();
    int x = (r.width() - sizeHint().width()) / 2;
    int y = (r.height() - sizeHint().height()) / 2;

    this->setGeometry(x, y, width(), height());
    this->setAttribute(Qt::WA_DeleteOnClose);
}


void RegWidget::on_dataReceived(const QByteArray &data)
{
    QByteArray bytes = data;
    QDataStream s(&bytes, QIODevice::ReadWrite);
    int code;
    s >> code;

    if (code == RegistrationPassed) QMessageBox::information(this, "Registration", "Registration passed!");
    if (code == RegistrationFailed) QMessageBox::warning(this, "Registration", QString("Registration failed! Probably the username called \"%1\" allready exists. Try another username.").arg(loginLine->text()) );

    passwordLine->clear();

    this->setEnabled(true);
}


void RegWidget::on_regButClicked()  //TEMP
{
    this->setDisabled(true);

    QByteArray req;
    QDataStream s(&req, QIODevice::ReadWrite);

    s << Registration << loginLine->text() << passwordLine->text();

    socket->connectToHost(QHostAddress::LocalHost, 1024);

    socket->sendData(req);
}
