#ifndef BHWIDGET_H
#define BHWIDGET_H

#include <QDebug>

#include <QWidget>
#include <QDesktopWidget>
#include <QPainter>

#include <QPaintEvent>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>

#include <QMimeData>

#include <QDesktopServices>

#include <QBitmap>
#include <QRadialGradient>
#include <QFile>

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>


#include "../DirExplorer/direxplorer.h"
#include "../ServerDirExplorer/serverdirexplorer.h"

#include "../SettingsWidget/settingswidget.h"
#include "skeys.h"

#include "../COMMON/socket.h"
#include "../COMMON/commands.h"


class BHWidget : public QWidget
{
    Q_OBJECT

 protected:

    Socket *socket;

    QSystemTrayIcon *sysTray;

    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

public:
    BHWidget(int w, int shift);

signals:
    void rightButtonClicked();

public slots:
    void on_showWgt();
    void on_showSettings();
    void on_userExit();
    void on_rightButtonClicked();

};

#endif // BHWIDGET_H
