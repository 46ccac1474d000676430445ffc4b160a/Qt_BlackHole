#include "direxplorer.h"

DirExplorer::DirExplorer(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
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
    connect(listWgt, &QListWidget::itemDoubleClicked, this, &DirExplorer::on_itemClicked);

//tool buttons
    //back button
    backBut = new QToolButton();
    backBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowBack));
    connect(backBut, &QToolButton::clicked, this, &DirExplorer::on_backButTriggered);
    //forward button
    forwardBut = new QToolButton();
    forwardBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowForward));
    connect(forwardBut, &QToolButton::clicked, this, &DirExplorer::on_forwardButTriggered);
    //goto button
    gotoBut = new QToolButton();
    gotoBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    connect(gotoBut, &QToolButton::clicked, this, &DirExplorer::on_gotoButTriggered);
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
    QString homePath = QSettings(CONFILE, QSettings::IniFormat).value(LOCDIR, QDir::homePath()+"/Black Hole").toString();
    if (!curDir.cd(homePath))
    {
        curDir.mkpath(homePath);
        curDir.setPath(homePath);
    }
    dirLineEdit->setText(curDir.absolutePath());
    openDir();

    this->setLayout(vlay);

    QRect r = QDesktopWidget().geometry();
    int x = r.width()/2 - 400;
    int y = r.height()/2 - 275;

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(x, y, 800, 550);
    this->setStyleSheet("* { outline: none; }");
    this->show();
}


void DirExplorer::closeEvent(QCloseEvent *event)
{
    parentWidget()->show(); //restores parent widget
    QWidget::closeEvent(event);
}


void DirExplorer::openDir()
{
    dirLineEdit->setText(curDir.absolutePath());

    listWgt->clear();

    QFileInfoList infoList = curDir.entryInfoList();
    int N = infoList.size();

    for (int i = 2; i < N; i++)
    {
        QListWidgetItem *item;

        //for directory
        if (infoList[i].isDir()) item = new QListWidgetItem(
                    QApplication::style()->standardIcon(QStyle::SP_DirIcon),
                    infoList[i].fileName()
                    );

        //for file
        if (infoList[i].isFile())  item = new QListWidgetItem(
                        QApplication::style()->standardIcon(QStyle::SP_FileIcon),
                        infoList[i].fileName()
                        );

        listWgt->addItem(item);
    }
}


void DirExplorer::on_itemClicked(QListWidgetItem *item)
{
    qDebug() << "in on_itemClicked:";

    QString absFPath = curDir.absoluteFilePath(item->text());

    if (QFileInfo(absFPath).isFile())
    {
        QDesktopServices::openUrl(QUrl(absFPath));
        return;
    }

    if (curDir.cd(item->text())) openDir();
    else QMessageBox::information(this, "", "Can't open dir:\n" + absFPath);

}


void DirExplorer::on_backButTriggered()
{
    qDebug() << "in on_backButTriggered:";
    if (curDir.isRoot()) return;
    if (curDir.cdUp()) openDir();
    else QMessageBox::information(this, "", "Can't open dir:\n" + curDir.entryList()[0]);
}


void DirExplorer::on_forwardButTriggered()
{

}


void DirExplorer::on_gotoButTriggered()
{
    qDebug() << "in on_gotoButTriggered:";
    if (curDir.cd(dirLineEdit->text())) openDir();
    else
    {
        QMessageBox::information(this, "", "Can't open dir:\n" + dirLineEdit->text());
        dirLineEdit->setText(curDir.absolutePath());
    }
}
