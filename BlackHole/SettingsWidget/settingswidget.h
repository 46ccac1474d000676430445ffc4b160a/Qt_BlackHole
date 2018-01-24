#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

#include <QDesktopWidget>

#include <QSettings>

#include <QDir>

#include "../RegWidget/regwidget.h"
#include "skeys.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT

//    const char *LOGIN = "login";
//    const char *PASSWORD = "impfield";
//    const char *LOCDIRENABLE = "localdirectoryenable";
//    const char *LOCDIR = "localdirectory";

    QLineEdit *loginLine,
              *pswrdLine,
              *locDir;

    QCheckBox *useLocDir;

    QPushButton *applyBut,
                *cancelBut,
                *regBut;

public:

    SettingsWidget(QWidget *parent);

public slots:
    void on_applyBut();
    void on_cancelBut();
    void on_regBut();
    void on_locDirEnabled(bool checked);

};

#endif // SETTINGSWIDGET_H
