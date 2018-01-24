#ifndef SERVERDIREXPLORER_H
#define SERVERDIREXPLORER_H

#include <QWidget>
#include <QListWidget>
#include <QToolButton>
#include <QLineEdit>

#include <QDesktopWidget>

#include <QMessageBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QListWidgetItem>

#include <QApplication>

#include <QThread>

#include "../COMMON/socket.h"
#include "../COMMON/commands.h"


class ServerDirExplorer : public QWidget
{
    Q_OBJECT


    QListWidget *listWgt;

    QToolButton *backBut,
                *forwardBut,
                *gotoBut;

    QLineEdit *dirLineEdit;

    Socket *socket;

    void closeEvent(QCloseEvent *event);

    void listDir(const QString &path);
    void fillListWidget(const QStringList &list);

private slots:
    void on_itemClicked(QListWidgetItem *item);
    void on_backButTriggered();
    void on_forwardButTriggered();
    void on_gotoButTriggered();


public:
    explicit ServerDirExplorer(QWidget *parent = nullptr);

};

#endif // SERVERDIREXPLORER_H
