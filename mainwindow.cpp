#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    manager(this),
    settings("Mark", "MarkShowNumbers"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIconText("Mark' 'Show' 'Numbers");
    this->setWindowTitle("Mark Show Numbers");
//    setWindowTitle(QCoreApplication::applicationName());
    ui->mainToolBar->setVisible(false);
    ui->radioButton->setChecked(true);
    ui->buttonShow->setEnabled(false);
    ui->buttonHide->setEnabled(false);

    readSettings();

    setupMenuBar();
    setupConnections();
    createItems();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar { new QMenuBar(this) };
    QMenu *menuFile { new QMenu("&File", menuBar) };
    QAction *actionOpenImage { new QAction("&Open Image", menuFile) };
    QAction *actionPositionNumber { new QAction("&Position Number", menuFile) };
    QAction *actionSizeImage { new QAction("&Size Image", menuFile) };
    QAction *actionOriginalSizeImage { new QAction("&Original Size Image", menuFile) };
    actionOriginalSizeImage->setCheckable(true);
    actionOriginalSizeImage->setChecked(true);
    QAction *actionClearImages { new QAction("&Clear Images", menuFile) };
    QAction *actionQuit { new QAction("&Quit", menuFile) };
    menuFile->addAction(actionOpenImage);
    menuFile->addAction(actionPositionNumber);
    menuFile->addAction(actionSizeImage);
    menuFile->addAction(actionOriginalSizeImage);
    menuFile->addAction(actionClearImages);
    menuFile->addAction(actionQuit);

    connect(actionOpenImage, &QAction::triggered, this, &MainWindow::onActionOpenImage);
    connect(actionPositionNumber, &QAction::triggered, this, &MainWindow::onActionPositionNumber);
    connect(actionSizeImage, &QAction::triggered, this, &MainWindow::onActionSizeImage);
    connect(actionOriginalSizeImage, &QAction::triggered, this, &MainWindow::onActionOriginalSizeImage);
    connect(actionClearImages, &QAction::triggered, this, &MainWindow::onActionClearImages);
    connect(actionQuit, &QAction::triggered, this, &MainWindow::close);

    QMenu *menuHelp { new QMenu("Help", this) };
    QAction *actionAboutApp { new QAction("About app", menuHelp) };
    QAction *actionAboutDeveloper { new QAction("About Developer", menuHelp) };
    QAction *actionAboutQt { new QAction("About Qt", menuHelp) };
    menuHelp->addAction(actionAboutApp);
    menuHelp->addAction(actionAboutDeveloper);
    menuHelp->addAction(actionAboutQt);

    connect(actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(actionAboutDeveloper, &QAction::triggered, this, &MainWindow::onActionDeveloper);
    connect(actionAboutApp, &QAction::triggered, this, &MainWindow::onActionAboutApp);

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuHelp);
    this->setMenuBar(menuBar);
}


void MainWindow::setupConnections()
{
    connect(ui->buttonShow, &QPushButton::clicked, this, &MainWindow::slotShow);
    connect(ui->buttonHide, &QPushButton::clicked, this, &MainWindow::slotHide);
    connect(ui->listWidget, &QListWidget::itemPressed, this, &MainWindow::slotItemPressed);
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::slotSelectionChanged);
    connect(ui->listWidget, &QListWidget::itemActivated, this, &MainWindow::slotItemActivated);
    connect(ui->radioButton, &QRadioButton::toggled, [this](){ this->ui->listWidget->setFocus(); });
}

void MainWindow::createItems()
{
    ui->listWidget->clear();
    for (QString strPath : manager.getListImage()) {
        QListWidgetItem *item {new QListWidgetItem(QPixmap(strPath), QFileInfo(strPath).fileName(), ui->listWidget)};
        item->setData(11, QVariant(strPath));
    }
}

void MainWindow::createTable(QTableWidget &tableWidget, QPoint labelPoint)
{
    tableWidget.setRowCount(QGuiApplication::screens().count());
    tableWidget.setColumnCount(4);
    QTableWidgetItem* ptwi = nullptr;
    QStringList lstHorizontal;
    lstHorizontal << "Width" << "Height" << "Move along X" << "Move along Y";
    tableWidget.setHorizontalHeaderLabels(lstHorizontal);
    QString strVertical;
    int row {0};
    for (QScreen *screen : QGuiApplication::screens())
    {
        if (screen->name() == QGuiApplication::primaryScreen()->name()){
            strVertical = "Primary Screen ";
        }
        else{
            strVertical = "Screen " + screen->name();
        }
        tableWidget.setVerticalHeaderItem(row, new QTableWidgetItem(strVertical));

        ptwi = new QTableWidgetItem(QString::number(screen->size().width()));
        tableWidget.setItem(row, 0, ptwi);
        ptwi = new QTableWidgetItem(QString::number(screen->size().height()));
        tableWidget.setItem(row, 1, ptwi);
        ptwi = new QTableWidgetItem(QString::number(labelPoint.x()));
        tableWidget.setItem(row, 2, ptwi);
        ptwi = new QTableWidgetItem(QString::number(labelPoint.y()));
        tableWidget.setItem(row, 3, ptwi);
        ++row;
    }
}

void MainWindow::refreshPosition(QTableWidget &tableWidget)
{
    for(int var {0}; var < QGuiApplication::screens().count(); ++var){
        int xPos {tableWidget.item(var, 2)->text().toInt()};
        int yPos {tableWidget.item(var, 3)->text().toInt()};
        manager.changePosLabel(QPoint(xPos, yPos));
    }
}

void MainWindow::readSettings()
{
    QPoint startPosition(settings.value("/PosImage/posX").toInt(), settings.value("/PosImage/posY").toInt());
    manager.setLabelPosition(startPosition);
    for (int var = 0; var < settings.value("/PathImage/CountPath").toInt(); ++var) {
        if(QFile(settings.value("/PathImage/Path" + QString::number(var)).toString()).exists()){
            manager.setListImage(QStringList(settings.value("/PathImage/Path" + QString::number(var)).toString()));
        }
    }
}

void MainWindow::writeSettings()
{
    settings.setValue("/PosImage/posX", manager.getLabelPos().x());
    settings.setValue("/PosImage/posY", manager.getLabelPos().y());
    settings.setValue("/PathImage/CountPath", manager.getListImage().size());
    for(int var = 0; var < manager.getListImage().size(); ++var){
        settings.setValue("/PathImage/Path" + QString::number(var), manager.getListImage().at(var));
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    manager.hideLabel();
}

void MainWindow::onActionOpenImage()
{
    QStringList list = QFileDialog::getOpenFileNames(this, "Open Dialog");
    manager.setListImage(list);
    createItems();
}

void MainWindow::onActionAboutApp()
{
    QDialog dialog;
    QLabel label;
    label.setText("This app is made for Mark. Ask him what it can do:)\n"
                  "I made this app for my brother Mark.\n"
                  "It displays the numbers to the projector that the user has loaded into the program.\n"
                  "These may not be necessarily numbers. You can move this object,\n"
                  "change its size and control its appearance and disappearance.\n");
    QVBoxLayout lay;
    lay.addWidget(&label);
    dialog.setLayout(&lay);
    dialog.resize(344, 23);
    dialog.exec();
}

void MainWindow::onActionDeveloper()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);
    QScrollArea scr(&dialog);
    QWidget wgt(&dialog);

    QPalette palette;
    palette.setBrush(QPalette::Normal, QPalette::Background,QBrush(Qt::lightGray));
    dialog.setPalette(palette);

    QVBoxLayout vbox(&wgt);
    QVBoxLayout vbox2;
        QLabel lab1;
        lab1.setPixmap(QPixmap(":/new/prefix1/Logo_temporarily.png"));
        QLabel lab2;
        lab2.setTextInteractionFlags(Qt::TextSelectableByMouse);
        lab2.setText("Developer: \n"
                     "  Ilya Songrov. I am musician:) And I like programing.\n");
        QLabel lab3;
        lab3.setTextInteractionFlags(Qt::TextSelectableByMouse);
        lab3.setText("My contacts: \n"
                     "  Viber, Telegram:  +380983008644\n"
                     "  Email: ilyasongrov@gmail.com\n"
                     "Author of the logo: \n"
                     "  Mark Kostenko");

        QLabel lab4;
        lab4.setTextInteractionFlags(Qt::TextSelectableByMouse);
        lab4.setText("The program is free if used for the glory of God. "
                     "It can be distributed under a free software license\n"
                     "if it does not harm and violate the Qt license.\n");
        vbox.addWidget(&lab1);
        vbox.addWidget(&lab2);
        vbox.addWidget(&lab3);
        vbox.addWidget(&lab4);

        wgt.setLayout(&vbox);

        scr.setWidget(&wgt);
        vbox2.addWidget(&scr);

    dialog.setLayout(&vbox2);
    dialog.setFixedSize(570,390);
    dialog.exec();
}

void MainWindow::onActionPositionNumber()
{
        QDialog dialog;
        QTableWidget *tableWidget {new QTableWidget(&dialog)};
        QSpacerItem *spaser { new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum) };
        QPushButton *buttonReset { new QPushButton("&Reset", &dialog) };
        QPushButton *buttonApply { new QPushButton("&Apply", &dialog) };
        QPushButton *buttonOk { new QPushButton("&Ok", &dialog) };
        QGridLayout *grid { new QGridLayout(&dialog) };
        grid->addWidget(tableWidget, 0, 0, 1, 0);
        grid->addWidget(buttonReset, 1, 0);
        grid->addItem(spaser, 1, 1);
        grid->addWidget(buttonApply, 1, 2);
        grid->addWidget(buttonOk, 1, 3);
        dialog.setLayout(grid);
        dialog.resize(743,275);
        createTable(*tableWidget, manager.getLabelPos());

        connect(buttonOk, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(buttonApply, &QPushButton::clicked, [this, &tableWidget](){ this->refreshPosition(*tableWidget); });
        connect(buttonReset, &QPushButton::clicked, [this, &tableWidget](){
            this->createTable(*tableWidget, manager.getStartPosition());
            this->refreshPosition(*tableWidget); });
        if(dialog.exec() == QDialog::Accepted){
            this->refreshPosition(*tableWidget);
        }
}

void MainWindow::onActionSizeImage()
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::WindowCloseButtonHint);
    QLabel *labelWidth { new QLabel("Width", &dialog) };
    labelWidth->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QLabel *labelHeight { new QLabel("Height", &dialog) };
    labelHeight->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QSpinBox *spinWidth { new QSpinBox (&dialog) };
    spinWidth->setRange(0, 5000);
    spinWidth->setValue(manager.getSizeLabel().width());
    spinWidth->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QSpinBox *spinHeight { new QSpinBox (&dialog) };
    spinHeight->setRange(0, 5000);
    spinHeight->setValue(manager.getSizeLabel().height());
    spinHeight->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QPushButton *buttonCancel { new QPushButton("&Cancel", &dialog) };
    QPushButton *buttonOk { new QPushButton("&Ok", &dialog) };
    QGridLayout grid;
    grid.addWidget(labelWidth, 0, 0);
    grid.addWidget(labelHeight, 0, 1);
    grid.addWidget(spinWidth, 1, 0);
    grid.addWidget(spinHeight, 1, 1);
    grid.addWidget(buttonCancel, 2, 0);
    grid.addWidget(buttonOk, 2, 1);
    dialog.setLayout(&grid);
    dialog.resize(103, 103);
    connect(buttonOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(buttonCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    if(dialog.exec() == QDialog::Accepted){
        manager.changeSizeImage(QSize(spinWidth->value(), spinHeight->value()));
    }
}

void MainWindow::onActionOriginalSizeImage()
{
    QAction *action = static_cast<QAction *>(QObject::sender());
    manager.setOriginalSize(action->isChecked());
}

void MainWindow::onActionClearImages()
{
    manager.clearListImages();
    ui->listWidget->clear();
}

void MainWindow::slotShow()
{
    manager.showLabel(ui->radioButton->isChecked());
}

void MainWindow::slotHide()
{
    manager.hideLabel();
}

void MainWindow::slotItemPressed(QListWidgetItem *item)
{
    manager.setCurentPathImage(item->data(11).toString());
    ui->buttonShow->setEnabled(true);
    ui->buttonHide->setEnabled(true);
}

void MainWindow::slotItemActivated(QListWidgetItem *)
{
    manager.showLabel(ui->radioButton->isChecked());
}

void MainWindow::slotSelectionChanged()
{
    slotItemPressed(ui->listWidget->currentItem());
}



