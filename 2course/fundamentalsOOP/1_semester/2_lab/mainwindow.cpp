#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settingswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), Tlists(nullptr)
{
    ui->setupUi(this);

    setupUI();

    QString userTimeZoneId = FillTimeZones();
    qDebug() << "The user is in this timezone: " << userTimeZoneId;

    timeZone = QTimeZone(userTimeZoneId.toUtf8());

    TimeFormat = "HH:mm:ss";

    clock = new DigitalClock(this);
    clock->setDisplay(ui->labelTime);

    sett = new SettingsWindow(timeZone, TimeFormat, this, this);

    Clock(timeZone);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(openSettingsWindow()));


    connect(ui->openListButton, &QPushButton::clicked, this, &MainWindow::showTimerList);

    connect(sett, &SettingsWindow::timerListWindowCreated, this, &MainWindow::setTimerListWindow);

    connect(ui->clearListButton, &QPushButton::clicked, this, &MainWindow::clearTimerList);

    ui->verticalLayout->setAlignment(ui->pushButton, Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearTimerList()
{
    if (Tlists)
        Tlists->clearAllTimers();
     else
        qDebug() << "TimerListWindow is not created yet.";
}

void MainWindow::openSettingsWindow()
{
    sett->setModal(true);
    sett->setWindowTitle("Settings");
    sett->resize(600, 600);
    sett->show();
    sett->exec();
}

void MainWindow::Clock(const QTimeZone &TimeZone)
{
    clock->setTimeZone(new QTimeZone(TimeZone));
    clock->start();
}


QString MainWindow::FillTimeZones()
{
    QList<QByteArray> timeZoneIdsByteArray = QTimeZone::availableTimeZoneIds();

    QStringList timeZoneIds;
    for (const QByteArray &id : timeZoneIdsByteArray) {
        timeZoneIds.append(QString::fromUtf8(id));
    }

    for (const QString &id : timeZoneIds) {
        ui->comboBox->addItem(id);
    }

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onTimeZoneChanged);

    QTimeZone userTimeZone = QTimeZone::systemTimeZone();
    QString userTimeZoneId = userTimeZone.id();

    return userTimeZoneId;
}


void MainWindow::onTimeZoneChanged(int index)
{
    QString selectedTimeZoneId = ui->comboBox->currentText();
    qDebug() << "Selected timezone:" << selectedTimeZoneId;

    timeZone = QTimeZone(selectedTimeZoneId.toUtf8());

    QDateTime currentTime = QDateTime::currentDateTime().toTimeZone(timeZone);
    QTime currenttTime = currentTime.time();
    qDebug() << "Current time in selected timezone: " << currenttTime.toString("HH:mm:ss");
    sett->setTimeZone(timeZone);
    Clock(timeZone);
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    if (index == 0)
        TimeFormat = "HH:mm:ss";
    else
        TimeFormat = "hh:mm:ss AP";
    clock->changeFormat(TimeFormat);
    sett->setTimeFormat(TimeFormat);
}

void MainWindow::showTimerList()
{
    if (!Tlists) {
        Tlists = new TimerListWindow(this, sett);
        if (!Tlists)
        {
            qDebug() << "Failed to allocate memory for TimerListWindow.";
            return;
        }
        Tlists->setWindowTitle("Timers List");
    }
    if(Tlists->isMinimized()) Tlists->showNormal();
    Tlists->resize(700, 700);
    Tlists->show();
    Tlists->raise();
    Tlists->activateWindow();
}

void MainWindow::setTimerListWindow(TimerListWindow* tlists) {
    this->Tlists = tlists;
}

void MainWindow::setupUI()
{
    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Oxanium/static/Oxanium-Regular.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        QApplication::setFont(font);
    } else {
        qDebug() << "Font loading failed.";
    }

    setStyleSheet(R"(
    QMainWindow, QDialog {
        background-color: black;
    }
    QPushButton#pushButton, QPushButton#openListButton, QPushButton#clearListButton {
        font-family: 'Oxanium';
        font-size: 16px;
        background-color: rgba(75, 0, 130, 1);
        color: #f0f0f0;
        padding: 10px;
        border-radius: 8px;
        border: 1px solid white;
    }
QPushButton#pushButton:hover,
QPushButton#openListButton:hover,
QPushButton#clearListButton:hover {
        background-color: rgba(75, 0, 130, 0.8);
    }
QPushButton#pushButton:pressed,
QPushButton#openListButton:pressed,
QPushButton#clearListButton:pressed {
        background-color: rgba(75, 0, 130, 0.7);
    }
    QComboBox#comboBox,
    QComboBox#comboBox_2
 {
        font-family: 'Oxanium';
        font-size: 14px;
        padding: 6px;
        border: 1px solid white;
        border-radius: 8px;
        background-color: #3c3f41;
        color: white;
    }
    QComboBox QAbstractItemView#comboBox,
QComboBox QAbstractItemView#comboBox_2
 {
        background-color: #3c3f41;
        selection-background-color: #5c5f61;
        color: white;
    }
)");
}
