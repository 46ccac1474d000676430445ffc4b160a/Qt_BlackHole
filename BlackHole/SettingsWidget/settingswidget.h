#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QMessageBox>

#include <QDesktopWidget>

#include <QSettings>

#include <QDir>

#include "../RegWidget/regwidget.h"
#include "skeys.h"

class SettingsWidget : public QWidget
{
    Q_OBJECT

    QLineEdit *addressLine,
              *loginLine,
              *pswrdLine,
              *locDir;

    QSpinBox *portSpin;

    QCheckBox *useLocDir;

    QPushButton *regBut,
                *applyBut,
                *cancelBut,
                *setDefaultBut;

public:

    SettingsWidget(QWidget *parent);

    void setParamsFromSettings();

public slots:
    void on_regBut();
    void on_applyBut();
    void on_cancelBut();
    void on_setDefaultBut();
    void on_locDirEnabled(bool checked);

};

#endif // SETTINGSWIDGET_H
