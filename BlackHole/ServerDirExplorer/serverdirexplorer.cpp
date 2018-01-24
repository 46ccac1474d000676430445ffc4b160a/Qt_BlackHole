#include "serverdirexplorer.h"

ServerDirExplorer::ServerDirExplorer(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    socket = new Socket(this);

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
    connect(listWgt, &QListWidget::itemDoubleClicked, this, &ServerDirExplorer::on_itemClicked);

//tool buttons
    //back button
    backBut = new QToolButton();
    backBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowBack));
    connect(backBut, &QToolButton::clicked, this, &ServerDirExplorer::on_backButTriggered);
    //forward button
    forwardBut = new QToolButton();
    forwardBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowForward));
    connect(forwardBut, &QToolButton::clicked, this, &ServerDirExplorer::on_forwardButTriggered);
    //goto button
    gotoBut = new QToolButton();
    gotoBut->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    connect(gotoBut, &QToolButton::clicked, this, &ServerDirExplorer::on_gotoButTriggered);
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

    //TODO

    this->setLayout(vlay);

    QRect r = QDesktopWidget().geometry();
    int x = r.width()/2 - 400;
    int y = r.height()/2 - 275;

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(x, y, 800, 550);
    this->setStyleSheet("* { outline: none; }");
    this->show();
}


void ServerDirExplorer::closeEvent(QCloseEvent *event)
{
    parentWidget()->show(); //restores parent widget
    QWidget::closeEvent(event);
}


void ServerDirExplorer::listDir(const QString &path)
{

}


void ServerDirExplorer::fillListWidget(const QStringList &list)
{
    listWgt->clear();

    for(QString dir : list)
    {
        listWgt->addItem(dir);
    }
}


void ServerDirExplorer::on_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
}


void ServerDirExplorer::on_backButTriggered()
{

}


void ServerDirExplorer::on_forwardButTriggered()
{

}


void ServerDirExplorer::on_gotoButTriggered()
{

}
