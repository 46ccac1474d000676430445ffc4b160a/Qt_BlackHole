#include "bhwidget.h"

BHWidget::BHWidget(int w, int shift) :
    QWidget(nullptr,

              Qt::Window
            | Qt::WindowDoesNotAcceptFocus
            | Qt::WindowStaysOnTopHint
            | Qt::FramelessWindowHint
            | Qt::SplashScreen

            )
{    
    QRect r = QDesktopWidget().availableGeometry();

    int x = r.x() + r.width() - w - shift;
    int y = r.y() + r.height() - w - shift;

    setGeometry(x, y, w, w);    //set position on screen
    setFixedSize(w, w);
    setCursor(Qt::BlankCursor); //hide cursor
    setAcceptDrops(true);

    QPixmap mask(":/Mask/mask150.png");
    setMask( mask.scaled( size() ).mask() );    //set round mask

    //create actions to tray
    QAction *hideWgt = new QAction("Hide");
    connect(hideWgt, &QAction::triggered, this, &BHWidget::hide);
    QAction *showWgt = new QAction("Show");
    connect(showWgt, &QAction::triggered, this, &BHWidget::on_showWgt);
    QAction *settingsWgt = new QAction("Settings");
    connect(settingsWgt, &QAction::triggered, this, &BHWidget::on_showSettings);
    QAction *exit = new QAction("Exit");
    connect(exit, &QAction::triggered, this, &BHWidget::on_userExit);

    QMenu *trayMenu = new QMenu(this);
    trayMenu->addAction(hideWgt);
    trayMenu->addAction(showWgt);
    trayMenu->addAction(settingsWgt);
    trayMenu->addAction(exit);

    //setting tray
    sysTray = new QSystemTrayIcon(this);
    sysTray->setIcon(mask);
    sysTray->setToolTip("Black hole");
    sysTray->setContextMenu(trayMenu);
    sysTray->show();


    connect(this, &BHWidget::rightButtonClicked, this, &BHWidget::on_rightButtonClicked);

}


void BHWidget::on_showWgt()
{
    hide(); //need to work on Ubuntu Linux
    show();
}


void BHWidget::on_showSettings()
{
    if (setWgt == nullptr)
    {
        setWgt = new SettingsWidget(this);
        connect(setWgt, &SettingsWidget::destroyed, [=](){setWgt = nullptr;});
    }
    setWgt->show();
}


void BHWidget::on_userExit()
{
    exit(0);
}


void BHWidget::on_rightButtonClicked()
{
    QSettings settings(CONFILE, QSettings::IniFormat);

    if (settings.value(LOCDIRENABLE, true).toBool())
        new DirExplorer(this);          //create explorer window if enabled
    else
        new ServerDirExplorer(this);    //create server explorer window if enabled

    hide();                             //and hide current widget
}


void BHWidget::paintEvent(QPaintEvent *event)   //paint black hole
{
    Q_UNUSED(event);

    int w = width();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);

    painter.setBrush(QBrush(QColor(0, 0, 0)));
    painter.drawRect(-1, -1, w+1, w+1);

    QRadialGradient gr(w/2., w/2., w/2.);
    //gr.setColorAt(0., QColor(0, 0, 0));
    gr.setColorAt(0.91, QColor(0, 0, 0));
    gr.setColorAt(0.965, QColor(20, 20, 35));
    gr.setColorAt(1., QColor(68, 71, 91));

    painter.setPen( QPen( QBrush( gr ), w/8 ) );
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(0, 0, w, w, 0, 16*360);

}


void BHWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) emit rightButtonClicked();
}


void BHWidget::dragEnterEvent(QDragEnterEvent *event)
{
    //if (event->mimeData()->hasUrls())
        event->accept();
}


void BHWidget::dropEvent(QDropEvent *event)
{
    QSettings settings(CONFILE, QSettings::IniFormat);

    const QMimeData *mime = event->mimeData();

    if (settings.value(LOCDIRENABLE, true).toBool())
        foreach (QUrl url, mime->urls())
        {
            QFile file(url.path());
            if (file.open(QIODevice::ReadOnly))
                file.copy(settings.value(LOCDIR, QDir::homePath()+"/Black Hole").toString() + "/" + url.fileName());

            file.flush();
            file.close();
        }
    else
        foreach (QUrl url, mime->urls())
        {
            //TODO
        }

}
