#include "settingswidget.h"


SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent, Qt::Tool)
{
    QSettings settings(CONFILE, QSettings::IniFormat);

    //login line edit
    loginLine = new QLineEdit(settings.value( LOGIN, "").toString());
    loginLine->setDisabled(settings.value(LOCDIRENABLE, true).toBool());

    //password line edit
    pswrdLine = new QLineEdit(settings.value( PASSWORD, "").toString());
    pswrdLine->setEchoMode(QLineEdit::Password);
    pswrdLine->setDisabled(settings.value(LOCDIRENABLE, true).toBool());

    //locale directory line edit
    locDir = new QLineEdit(settings.value( LOCDIR, QDir::homePath()+"/Black Hole").toString());
    locDir->setEnabled(settings.value(LOCDIRENABLE, true).toBool());

//buttons
    applyBut = new QPushButton("&Apply");
    connect(applyBut, &QPushButton::clicked, this, &SettingsWidget::on_applyBut);
    cancelBut = new QPushButton("&Cancel");
    connect(cancelBut, &QPushButton::clicked, this, &SettingsWidget::on_cancelBut);
    regBut = new QPushButton("&Registration");
    connect(regBut, &QPushButton::clicked, this, &SettingsWidget::on_regBut);

    //locale directory enabled checkbox
    useLocDir = new QCheckBox("Use locale directory");
    connect(useLocDir, &QCheckBox::toggled, this, &SettingsWidget::on_locDirEnabled);
    useLocDir->setChecked(settings.value( LOCDIRENABLE, true).toBool());

    //button Hlayout
    QHBoxLayout *hlay = new QHBoxLayout();
    hlay->addWidget(regBut);
    hlay->addSpacing(50);
    hlay->addWidget(applyBut);
    hlay->addWidget(cancelBut);

//main Vlayout
    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addWidget(new QLabel("Login:"));
    vlay->addWidget(loginLine);
    vlay->addWidget(new QLabel("Password:"));
    vlay->addWidget(pswrdLine);
    vlay->addWidget(useLocDir);
    vlay->addWidget(locDir);
    vlay->addLayout(hlay);

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


void SettingsWidget::on_applyBut()
{
    QSettings settings(CONFILE, QSettings::IniFormat);
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


void SettingsWidget::on_regBut()
{
    new RegWidget(this);
}


void SettingsWidget::on_locDirEnabled(bool checked)
{
    if (checked)
    {
        loginLine->setDisabled(true);
        pswrdLine->setDisabled(true);
        locDir->setEnabled(true);
    }
    else
    {
        loginLine->setEnabled(true);
        pswrdLine->setEnabled(true);
        locDir->setDisabled(true);
    }
}
