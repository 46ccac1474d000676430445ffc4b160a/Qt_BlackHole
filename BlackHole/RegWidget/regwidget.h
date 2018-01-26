#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

#include <QMessageBox>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDesktopWidget>

#include "../COMMON/socket.h"
#include "../COMMON/commands.h"

class RegWidget : public QWidget
{
    Q_OBJECT

protected:
    Socket *socket;

    QLineEdit *loginLine,
              *passwordLine;

    QPushButton *regBut,
                *exitBut;

    QCheckBox *showPasswordCheck;

protected slots:
    void on_dataReceived(const QByteArray &data);

    void on_regButClicked();

public:
	explicit RegWidget(QWidget *parent);

};

#endif // REGWIDGET_H
