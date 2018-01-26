#include "settingswidget.h"


SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent, Qt::Window)
{
    //address line edit
    addressLine = new QLineEdit();

    //port spinbox
    portSpin = new QSpinBox();
    portSpin->setRange(1, 65535);


    //login line edit
    loginLine = new QLineEdit();

    //password line edit
    pswrdLine = new QLineEdit();
    pswrdLine->setEchoMode(QLineEdit::Password);

    //locale directory line edit
    locDir = new QLineEdit();

//buttons
    regBut = new QPushButton("&Registration");
    connect(regBut, &QPushButton::clicked, this, &SettingsWidget::on_regBut);
    applyBut = new QPushButton("&Apply");
    connect(applyBut, &QPushButton::clicked, this, &SettingsWidget::on_applyBut);
    cancelBut = new QPushButton("&Cancel");
    connect(cancelBut, &QPushButton::clicked, this, &SettingsWidget::on_cancelBut);
    setDefaultBut = new QPushButton("Set by &default");
    connect(setDefaultBut, &QPushButton::clicked, this, &SettingsWidget::on_setDefaultBut);


    //locale directory enabled checkbox
    useLocDir = new QCheckBox("Use locale directory");
    connect(useLocDir, &QCheckBox::toggled, this, &SettingsWidget::on_locDirEnabled);

    //read and setting params for fields
    this->setParamsFromSettings();

    //server address Vlayout
    QVBoxLayout *vServAdrLay = new QVBoxLayout();
    vServAdrLay->addWidget(new QLabel("Server address:"));
    vServAdrLay->addWidget(addressLine);

    //server port Vlayout
    QVBoxLayout *vServPortLay = new QVBoxLayout();
    vServPortLay->addWidget(new QLabel("Server port:"));
    vServPortLay->addWidget(portSpin);

    //address Hlayout
    QHBoxLayout *hAddressLay = new QHBoxLayout();
    hAddressLay->addLayout(vServAdrLay);
    hAddressLay->addLayout(vServPortLay);

    //button Hlayout
    QHBoxLayout *hButLay = new QHBoxLayout();
    hButLay->addWidget(regBut);
    hButLay->addSpacing(50);
    hButLay->addWidget(applyBut);
    hButLay->addWidget(cancelBut);
    hButLay->addWidget(setDefaultBut);

//main Vlayout
    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addLayout(hAddressLay);
    vlay->addWidget(new QLabel("Login:"));
    vlay->addWidget(loginLine);
    vlay->addWidget(new QLabel("Password:"));
    vlay->addWidget(pswrdLine);
    vlay->addWidget(useLocDir);
    vlay->addWidget(locDir);
    vlay->addLayout(hButLay);

    this->setLayout(vlay);

    QRect r = QDesktopWidget().geometry();
    int x = (r.width() - sizeHint().width()) / 2;
    int y = (r.height() - sizeHint().height()) / 2;

    this->setGeometry(x, y, width(), height());
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(sizeHint());
    this->setWindowTitle("Black Hole settings");
    this->show();
}


void SettingsWidget::setParamsFromSettings()
{
    QSettings settings(CONFILE, QSettings::IniFormat);

    addressLine->setText(settings.value( SERVERADDRESS, "127.0.0.1").toString());
    addressLine->setDisabled(settings.value(LOCDIRENABLE, true).toBool());

    portSpin->setValue(settings.value(SERVERPORT, 56565).toInt());
    portSpin->setDisabled(settings.value(LOCDIRENABLE, true).toBool());

    loginLine->setText(settings.value( LOGIN, "").toString());
    loginLine->setDisabled(settings.value(LOCDIRENABLE, true).toBool());

    pswrdLine->setText(settings.value( PASSWORD, "").toString());
    pswrdLine->setDisabled(settings.value(LOCDIRENABLE, true).toBool());

    locDir->setText(settings.value( LOCDIR, QDir::homePath()+"/Black Hole").toString());
    locDir->setEnabled(settings.value(LOCDIRENABLE, true).toBool());

    useLocDir->setChecked(settings.value( LOCDIRENABLE, true).toBool());
}


void SettingsWidget::on_regBut()
{
    new RegWidget(this);
}


void SettingsWidget::on_applyBut()
{
    QSettings settings(CONFILE, QSettings::IniFormat);
    settings.setValue(SERVERADDRESS, addressLine->text());
    settings.setValue(SERVERPORT, portSpin->value());
    settings.setValue(LOGIN, loginLine->text());
    settings.setValue(PASSWORD, pswrdLine->text());
    settings.setValue(LOCDIRENABLE, useLocDir->isChecked());
    settings.setValue(LOCDIR, locDir->text());

    settings.sync();

    this->close();
}


void SettingsWidget::on_cancelBut()
{
    this->close();
}


void SettingsWidget::on_setDefaultBut()
{
    int val = QMessageBox::question(this, "", "Are you sure?");

    if (val == QMessageBox::Yes)
    {
        QSettings settings(CONFILE, QSettings::IniFormat);
        settings.setValue(SERVERADDRESS, "127.0.0.1");
        settings.setValue(SERVERPORT, 56565);
        settings.setValue(LOGIN, "");
        settings.setValue(PASSWORD, "");
        settings.setValue(LOCDIRENABLE, true);
        settings.setValue(LOCDIR, QDir::homePath()+"/Black Hole");

        settings.sync();

        setParamsFromSettings();
    }
}


void SettingsWidget::on_locDirEnabled(bool checked)
{
    if (checked)
    {
        addressLine->setDisabled(true);
        portSpin->setDisabled(true);
        loginLine->setDisabled(true);
        pswrdLine->setDisabled(true);
        locDir->setEnabled(true);
    }
    else
    {
        addressLine->setEnabled(true);
        portSpin->setEnabled(true);
        loginLine->setEnabled(true);
        pswrdLine->setEnabled(true);
        locDir->setDisabled(true);
    }
}
