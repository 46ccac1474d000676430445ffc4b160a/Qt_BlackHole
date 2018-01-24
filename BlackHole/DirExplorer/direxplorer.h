#ifndef DIREXPLORER_H
#define DIREXPLORER_H


#include <QDebug>


#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDesktopWidget>
#include <QListWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QMessageBox>

#include <QListWidgetItem>
#include <QAction>
#include <QDir>
#include <QFileInfo>

#include <QApplication>
#include <QDesktopServices>
#include <QSettings>
#include <skeys.h>

#include <QCloseEvent>


class DirExplorer : public QWidget
{
    Q_OBJECT


    QListWidget *listWgt;

    QToolButton *backBut,
                *forwardBut,
                *gotoBut;

    QLineEdit *dirLineEdit;

    QDir curDir;

    void closeEvent(QCloseEvent *event);

    void openDir();

private slots:
    void on_itemClicked(QListWidgetItem *item);
    void on_backButTriggered();
    void on_forwardButTriggered();
    void on_gotoButTriggered();


public:
    explicit DirExplorer(QWidget *parent = nullptr);

};

#endif // DIREXPLORER_H
