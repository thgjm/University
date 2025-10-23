#include "timerwidget.h"
#include "ui_timerwidget.h"

TimerWidget::TimerWidget(QWidget *parent, TimerInfo timerInfo, SettingsWindow* settings)
    : QStackedWidget(parent)
    , ui(new Ui::TimerWidget), timerInfo(timerInfo), settings(settings), player()
{
    ui->setupUi(this);

    setupUI();

    timer = new Timer(this);
    timer->AmPm = timerInfo.timer->AmPm;
    timer->TimeFormat = timerInfo.timer->TimeFormat;
    timer->timeZone = timerInfo.timer->timeZone;

    qDebug()<<timerInfo.soundName;
    qDebug()<<timerInfo.imageName;
    qDebug()<<timerInfo.appName;
    qDebug()<<timerInfo.documentName;

    qDebug()<<timerInfo.Title;
    if(timerInfo.Ttype == TimerType::Timer)
    {
        qDebug()<<"Timer";
        setCurrentWidget(ui->TimerPage);
        ui->timerName->setText(timerInfo.Title);
    }
    else if(timerInfo.Ttype == TimerType::Alarm)
    {
        SetAlarmDisplay();
        qDebug()<<"Alarm";
        setCurrentWidget(ui->AlarmPage);
        ui->alarmName->setText(timerInfo.Title);
    }

    ui->horizontalLayout_2->setAlignment(Qt::AlignCenter);

    qDebug() << timerInfo.selectedDateTime.time().hour()<<":"<<timerInfo.selectedDateTime.time().minute()<<":"<<timerInfo.selectedDateTime.time().second();

    connect(ui->resetButton, &QPushButton::clicked, this, &TimerWidget::resetTimer);
    connect(ui->stopButton, &QPushButton::clicked, this, &TimerWidget::PauseResume);

    TimerStart();

    ui->timerName->setText(timerInfo.Title);

    connect(ui->infoButton, &QPushButton::clicked, this, &TimerWidget::openInfoWindow);

    connect(ui->turnOnOffButton, &QPushButton::clicked, this, &TimerWidget::TurnAlarmOnOff);

    connect(ui->alarmInfo, &QPushButton::clicked, this, &TimerWidget::openInfoWindow);

    connect(ui->deleteTimerButton, &QPushButton::clicked, this, &TimerWidget::deleteTimer);
    connect(ui->deleteAlarmButton, &QPushButton::clicked, this, &TimerWidget::deleteTimer);
}

TimerWidget::~TimerWidget()
{
    delete ui;
}



//-------------TIMER----------------

void TimerWidget::PauseResume()
{
    if (timerPaused)
    {
        timerPaused = !timerPaused;
        TimerStart();

        ui->stopButton->setText("Pause");

        ui->stopButton->setStyleSheet("QPushButton { background-color: rgba(204, 85, 0, 1); } "
                                      "QPushButton:hover { background-color: rgba(204, 85, 0, 0.8); }"
                                      "QPushButton:pressed { background-color: rgba(204, 85, 0, 0.7); }");
    }
    else
    {
        timerPaused = !timerPaused;
        remainingTime = timer->getRemainingTime();
        timer->stop();
        countDown->stop();
        ui->stopButton->setText("Resume");
        ui->stopButton->setStyleSheet("QPushButton { background-color: rgba(0, 110, 4, 1); } "
                                      "QPushButton:hover { background-color: rgba(0, 110, 4, 0.8); }"
                                      "QPushButton:pressed { background-color: rgba(0, 110, 4, 0.7); }");
    }
}

void TimerWidget::resetTimer()
{
    if(timerPaused)
    {
        countDown = new CountDown(this, QTime(0, 0).addSecs(timerInfo.selectedDateTime.time().second() + timerInfo.selectedDateTime.time().minute() * 60 + timerInfo.selectedDateTime.time().hour() * 3600));
        countDown->setDisplay(ui->countdownLabel);
        countDown->start();
        countDown->stop();
        ui->stopButton->setText("Start");
        ui->stopButton->setStyleSheet("QPushButton { background-color: rgba(0, 110, 4, 1); } "
                                      "QPushButton:hover { background-color: rgba(0, 110, 4, 0.8); }"
                                      "QPushButton:pressed { background-color: rgba(0, 110, 4, 0.7); }");
    }
    if (countDown)
    {
        countDown->stop();
        delete countDown;
        countDown = nullptr;
        if(ui->stopButton->text()!="Start") { ui->stopButton->setText("Pause");
            ui->stopButton->setStyleSheet("QPushButton { background-color: rgba(204, 85, 0, 1); } "
                                          "QPushButton:hover { background-color: rgba(204, 85, 0, 0.8); }"
                                          "QPushButton:pressed { background-color: rgba(204, 85, 0, 0.7); }");
        }
    }
    remainingTime = 0;
    ui->resetButton->setText("Reset Timer");
    ui->resetButton->setStyleSheet("QPushButton { background-color: rgba(75, 0, 130, 1); } "
                                  "QPushButton:hover { background-color: rgba(75, 0, 130, 0.8); }"
                                  "QPushButton:pressed { background-color: rgba(75, 0, 130, 0.7); }");
    ui->stopButton->show();
    TimerStart();
}


//-------------ALARM----------------

void TimerWidget::SetAlarmDisplay()
{
    ui->alarmTimeLabel->setStyleSheet("QLabel { color : green; }");

    ui->alarmDateLabel->setStyleSheet("QLabel { color : white; }");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Orbitron/static/Orbitron-Regular.ttf");
    if (fontId != -1)
    {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        ui->alarmTimeLabel->setFont(QFont(family, 40));
    }
    else
        qDebug() << "The font was not uploaded.";

    fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Oxanium/static/Oxanium-Regular.ttf");
    if (fontId != -1)
    {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        ui->alarmDateLabel->setFont(QFont(family, 20));
    }
    else
        qDebug() << "The font was not uploaded.";

    QString timeText;
    QString DateText = timerInfo.selectedDateTime.date().toString("MMMM d, yyyy");
    ui->alarmDateLabel->setText(DateText);

    if(timer->TimeFormat == "HH:mm:ss")
    {
        timeText = timerInfo.selectedDateTime.time().toString("HH:mm:ss");
        ui->alarmTimeLabel->setText(timeText);
    }
    else
    {
        QTime adjustedSelectedTime = timerInfo.selectedDateTime.time();
        if (timer->AmPm == "PM" && adjustedSelectedTime.hour() != 12)
            adjustedSelectedTime = adjustedSelectedTime.addSecs(12 * 3600);
        else if (timer->AmPm == "AM" && adjustedSelectedTime.hour() == 12)
            adjustedSelectedTime = adjustedSelectedTime.addSecs(-12 * 3600);
        timeText = adjustedSelectedTime.toString("hh:mm:ss AP");
        ui->alarmTimeLabel->setText(timeText);
    }
}

void TimerWidget::TurnAlarmOnOff()
{
    if(ui->turnOnOffButton->text() == "Turn on again.")
    {
        timerInfo.selectedDateTime = timerInfo.selectedDateTime.addDays(1);
        SetAlarmDisplay();
        AlarmOn = false;
        ui->turnOnOffButton->setStyleSheet("QPushButton { background-color: rgba(255, 0, 0, 1); } "
                                           "QPushButton:hover { background-color: rgba(255, 0, 0, 0.8); }"
                                           "QPushButton:pressed { background-color: rgba(255, 0, 0, 0.7); }");
    }
    if(!AlarmOn)
    {
        QDateTime currentDateTime = timer->changeTime();
        QDateTime adjustedSelectedDateTime = timerInfo.selectedDateTime;
        if (timer->TimeFormat != "HH:mm:ss")
        {
            if (timer->AmPm == "PM" && adjustedSelectedDateTime.time().hour() != 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(12 * 3600);
            else if (timer->AmPm == "AM" && adjustedSelectedDateTime.time().hour() == 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(-12 * 3600);
        }

        int secondsToPlay = currentDateTime.secsTo(adjustedSelectedDateTime);

        if (secondsToPlay > 0)
            TimerStart();
        else
        {
            timerInfo.selectedDateTime = timerInfo.selectedDateTime.addDays(1);
            SetAlarmDisplay();
        }
        ui->turnOnOffButton->setText("Turn Off");
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : green; }");
        ui->turnOnOffButton->setStyleSheet("QPushButton { background-color: rgba(255, 0, 0, 1); } "
                                           "QPushButton:hover { background-color: rgba(255, 0, 0, 0.8); }"
                                           "QPushButton:pressed { background-color: rgba(255, 0, 0, 0.7); }");
    }
    else
    {
        timer->stop();
        ui->turnOnOffButton->setText("Turn On");
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : orange; }");
        ui->turnOnOffButton->setStyleSheet("QPushButton { background-color: rgba(0, 110, 4, 1); } "
                                           "QPushButton:hover { background-color: rgba(0, 110, 4, 0.8); }"
                                           "QPushButton:pressed { background-color: rgba(0, 110, 4, 0.7); }");
    }
    AlarmOn = !AlarmOn;
}

//-------------GENERAL----------------

void TimerWidget::TimerStart()
{
    QDateTime currentDateTime = timer->changeTime();

    if (timerInfo.Ttype == TimerType::Timer)
    {
        if (remainingTime == 0)
            remainingTime = timerInfo.selectedDateTime.time().second() + timerInfo.selectedDateTime.time().minute() * 60 + timerInfo.selectedDateTime.time().hour() * 3600;
        qDebug() << "Timer mode seconds to play:" << remainingTime;
        if (remainingTime > 0)
        {
            countDown = new CountDown(this, QTime(0, 0).addSecs(remainingTime));
            countDown->setDisplay(ui->countdownLabel);
            if(!timerPaused)
            {
                countDown->start();
                timer->start(remainingTime * 1000);
            }
        }
        else
            timer->stop();
    }
    else if (timerInfo.Ttype == TimerType::Alarm)
    {
        QDateTime adjustedSelectedDateTime = timerInfo.selectedDateTime;

        if (timer->TimeFormat != "HH:mm:ss")
        {
            if (timer->AmPm == "PM" && adjustedSelectedDateTime.time().hour() != 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(12 * 3600);
            else if (timer->AmPm == "AM" && adjustedSelectedDateTime.time().hour() == 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(-12 * 3600);
        }

        int secondsToPlay = currentDateTime.secsTo(adjustedSelectedDateTime) + 1;

        qDebug() << "Calculated seconds to alarm:" << secondsToPlay;

        if (secondsToPlay > 0)
            timer->start(secondsToPlay * 1000);
        else
        {
            timerInfo.selectedDateTime = timerInfo.selectedDateTime.addDays(1);
            SetAlarmDisplay();
        }
    }
    updateTimeoutConnections();
}

void TimerWidget::updateTimeoutConnections()
{
    disconnect(timer, &Timer::timeout, this, &TimerWidget::PlaySoundtrack);
    disconnect(timer, &Timer::timeout, this, &TimerWidget::ShowImage);
    disconnect(timer, &Timer::timeout, this, &TimerWidget::openDocument);
    disconnect(timer, &Timer::timeout, this, &TimerWidget::openApplication);
    disconnect(timer, &Timer::timeout, this, &TimerWidget::generalTimeout);
    if (timerInfo.soundName!="")
        connect(timer, &Timer::timeout, this, &TimerWidget::PlaySoundtrack);
    if (timerInfo.imageName!="")
        connect(timer, &Timer::timeout, this, &TimerWidget::ShowImage);
    if (timerInfo.documentName!="")
        connect(timer, &Timer::timeout, this, &TimerWidget::openDocument);
    if (timerInfo.appName!="")
        connect(timer, &Timer::timeout, this, &TimerWidget::openApplication);
    connect(timer, &Timer::timeout, this, &TimerWidget::generalTimeout);
}

void TimerWidget::openInfoWindow()
{
    info = new TimerInfoWindow(nullptr, timerInfo, settings, timer);
    info->setWindowTitle("Timer Info Page");
    info->resize(600, 600);
    info->show();
    connect(info, &TimerInfoWindow::timerInfoUpdated, this, &TimerWidget::updateTimerInfo);
}


void TimerWidget::generalTimeout()
{
    if (timerInfo.Ttype == TimerType::Timer)
    {
        timer->stop();
        timerPaused = true;
        ui->stopButton->hide();
        ui->resetButton->setText("Restart Timer");
        ui->resetButton->setStyleSheet("QPushButton { background-color: rgba(0, 110, 4, 1); } "
                                      "QPushButton:hover { background-color: rgba(0, 110, 4, 0.8); }"
                                      "QPushButton:pressed { background-color: rgba(0, 110, 4, 0.7); }");

        QString msgtext = "Timer '" + timerInfo.Title + "' has elapsed.";
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle("Timer Finished");
        msgBox->setText(msgtext);
        msgBox->setStandardButtons(QMessageBox::Ok);

        connect(msgBox, &QMessageBox::buttonClicked, this, [this](QAbstractButton* button){
            if (button->text() == "OK" && player && player->playbackState() == QMediaPlayer::PlayingState)
            {
                player->stop();
            }
        });

        msgBox->exec();
    }
    else
    {
        ui->alarmTimeLabel->setStyleSheet("QLabel { color : red; }");
        ui->turnOnOffButton->setText("Turn on again.");
        ui->turnOnOffButton->setStyleSheet("QPushButton { background-color: rgba(0, 110, 4, 1); } "
                                           "QPushButton:hover { background-color: rgba(0, 110, 4, 0.8); }"
                                           "QPushButton:pressed { background-color: rgba(0, 110, 4, 0.7); }");

        QString msgtext = "Alarm '" + timerInfo.Title + "' has elapsed.";
        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setWindowTitle("Alarm Triggered");
        msgBox->setText(msgtext);
        msgBox->setStandardButtons(QMessageBox::Ok);

        connect(msgBox, &QMessageBox::buttonClicked, this, [this](QAbstractButton* button){
            if (button->text() == "OK" && player && player->playbackState() == QMediaPlayer::PlayingState)
            {
                player->stop();
            }
        });

        msgBox->exec();
    }
}

void TimerWidget::updateTimerInfo(TimerInfo newTimerInfo, Timer *timer)
{
    timerInfo.soundName = newTimerInfo.soundName;
    timerInfo.imageName = newTimerInfo.imageName;
    timerInfo.appName = newTimerInfo.appName;
    timerInfo.documentName = newTimerInfo.documentName;
    timerInfo.selectedDateTime = newTimerInfo.selectedDateTime;
    timerInfo.Title= newTimerInfo.Title;
    timer= timer;
    if (timerInfo.Ttype == TimerType::Timer)
    {
        ui->timerName->setText(timerInfo.Title);
        remainingTime=0;
        if(countDown)
        {   countDown->stop();
            delete countDown;
            countDown = nullptr; }
    }
    else
        ui->alarmName->setText(timerInfo.Title);
    SetAlarmDisplay();
    TimerStart();
    updateTimeoutConnections();
}

void TimerWidget::PlaySoundtrack()
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    if (timerInfo.soundName.startsWith("qrc:/"))
        player->setSource(QUrl(timerInfo.soundName));
    else
        player->setSource(QUrl::fromLocalFile(timerInfo.soundName));
    audioOutput->setVolume(50);
    player->play();
    qDebug() << "Soundtrack is playing.";
    qDebug() << "Playing audio from:" << timerInfo.soundName;
    timer->stop();
}

void TimerWidget::ShowImage()
{
    img = new imageWindow(this, timerInfo.imageName);
    img->setWindowTitle("Image");
    QIcon icon(timerInfo.imageName);
    img->setWindowIcon(icon);
    img->show();
    qDebug() << "Photo is shown:" << timerInfo.imageName;
    timer->stop();
}


void TimerWidget::openApplication()
{
    QFileInfo appInfo(timerInfo.appName);
    qDebug() << "Checking application path:" << timerInfo.appName;
    if (!appInfo.exists() || !appInfo.isExecutable()) {
        QMessageBox::warning(this, "Error", "The selected application cannot be found or is not executable.");
        qDebug() << "Application not found or not executable:" << timerInfo.appName;
        timerInfo.timer->stop();
        return;
    }
    QProcess::startDetached(timerInfo.appName);
    qDebug() << "Opening application:" << timerInfo.appName;
    timer->stop();
}

void TimerWidget::openDocument()
{

    qDebug() << "Opening document from path:" << timerInfo.documentName;
    if (QDesktopServices::openUrl(QUrl::fromLocalFile(timerInfo.documentName)))
        qDebug() << "Document opened successfully.";
    else
        qDebug() << "Failed to open document.";
    timer->stop();
}

void TimerWidget::deleteTimer()
{
    emit timerDeleted(this);

    this->deleteLater();
}

void TimerWidget::setupUI()
{
    ui->verticalLayout->setAlignment(ui->timerName, Qt::AlignCenter);
    ui->verticalLayout_2->setAlignment(ui->alarmName, Qt::AlignCenter);

    ui->turnOnOffButton->setStyleSheet("QPushButton { background-color: rgba(255, 0, 0, 1); } "
                                       "QPushButton:hover { background-color: rgba(255, 0, 0, 0.8); }"
                                       "QPushButton:pressed { background-color: rgba(255, 0, 0, 0.7); }");

    ui->stopButton->setStyleSheet("QPushButton { background-color: rgba(204, 85, 0, 1); } "
                                  "QPushButton:hover { background-color: rgba(204, 85, 0, 0.8); }"
                                  "QPushButton:pressed { background-color: rgba(204, 85, 0, 0.7); }");

    setStyleSheet(R"(
    QLabel#timerName, QLabel#alarmName{
    font-size: 22px;
    font-style: italic;
    color: white;
    margin: 5px 0px;
    padding: 5px;
    background-color: indigo;
    border: 1px solid white;
    border-radius: 8px;
    }
    QPushButton#deleteTimerButton, QPushButton#deleteAlarmButton {
    background-color: rgba(255, 0, 0, 1);
    }
    QPushButton#deleteTimerButton:hover, QPushButton#deleteAlarmButton:hover {
    background-color: rgba(255, 0, 0, 0.8);
    }
    QPushButton#deleteTimerButton:pressed, QPushButton#deleteAlarmButton:pressed {
    background-color: rgba(255, 0, 0, 0.7);
    }

)");
}
