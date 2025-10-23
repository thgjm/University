#include "timerinfowindow.h"
#include "ui_timerinfowindow.h"
#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "timer.h"
#include <QMessageBox>

TimerInfoWindow::TimerInfoWindow(QWidget *parent, TimerInfo timerInfo, SettingsWindow* settings, Timer *timer)
    : QDialog(parent)
    , ui(new Ui::TimerInfoWindow), timerInfo(timerInfo), timer(timer), player()
{
    ui->setupUi(this);

    setupUI();

    ui->PauseButton->hide();

    audioTimer = new QTimer(this);

    QString timeZoneInfo = "The timer's timezone is: " + timer->timeZone->id() + ".";
    QString formatinfo = "The timer's time format is: ";
    if(timer->TimeFormat == "HH:mm:ss") formatinfo+="24-h format.";
    else formatinfo+="12-h format.";

    ui->timeZoneInfo->setText(timeZoneInfo);
    ui->timeFormatInfo->setText(formatinfo);


    ui->calendarWidget->setMinimumDate(settings->ui->calendarWidget->minimumDate());

    ui->calendarWidget->setSelectedDate(timerInfo.selectedDateTime.date());

    if(timerInfo.Ttype == TimerType::Timer)
    {
        ui->hourCount->setMaximum(23);
        ui->hourCount->setMinimum(0);
        ui->AmPmBox->hide();
        ui->calendarWidget->hide();
        ui->timeFormatInfo->hide();
        ui->timeZoneInfo->hide();
    }

    if(timerInfo.Ttype == TimerType::Alarm)
    {
        if(timer->TimeFormat == "HH:mm:ss")
        {
            ui->hourCount->setMaximum(23);
            ui->hourCount->setMinimum(0);
            ui->AmPmBox->hide();
        }
        else
        {
            ui->hourCount->setMaximum(12);
            ui->hourCount->setMinimum(1);
            ui->AmPmBox->show();
        }
        ui->calendarWidget->show();
    }

    if (settings)
    {
        ui->hourCount->setValue(timerInfo.selectedDateTime.time().hour());
        ui->minuteCount->setValue(timerInfo.selectedDateTime.time().minute());
        ui->secondCount->setValue(timerInfo.selectedDateTime.time().second());
        copyComboBoxItems(settings->ui->soundComboBox, ui->soundComboBox);
        copyComboBoxItems(settings->ui->imageComboBox, ui->imageComboBox);
        copyComboBoxItems(settings->ui->appComboBox, ui->appComboBox);
        copyComboBoxItems(settings->ui->documentComboBox, ui->documentComboBox);

        if(timerInfo.soundName!="")
        {
            ui->soundCheckBox_3->setChecked(true);
            int index = -1;
            for (int i = 0; i < ui->soundComboBox->count(); ++i)
                if (ui->soundComboBox->itemData(i).toString() == timerInfo.soundName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->soundComboBox->setCurrentIndex(index);
        }

        if(timerInfo.imageName!="")
        {
            ui->imageCheckBox->setChecked(true);

            int index = -1;
            for (int i = 0; i < ui->imageComboBox->count(); ++i)
                if (ui->imageComboBox->itemData(i).toString() == timerInfo.imageName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->imageComboBox->setCurrentIndex(index);
        }

        if(timerInfo.appName!="")
        {
            ui->appCheckBox->setChecked(true);
            int index = -1;
            for (int i = 0; i < ui->appComboBox->count(); ++i)
                if (ui->appComboBox->itemData(i).toString() == timerInfo.appName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->appComboBox->setCurrentIndex(index);
        }

        if(timerInfo.documentName!="")
        {
            ui->documentCheckBox->setChecked(true);
            int index = -1;
            for (int i = 0; i < ui->documentComboBox->count(); ++i)
                if (ui->documentComboBox->itemData(i).toString() == timerInfo.documentName)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                ui->documentComboBox->setCurrentIndex(index);
        }

        connect(ui->uploadImageButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadImage);
        connect(ui->uploadSoundButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadSound);
        connect(ui->uploadAppButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadApp);
        connect(ui->uploadDocumentButton, &QPushButton::clicked, this, &TimerInfoWindow::uploadDocument);

        ui->timerName->setText(timerInfo.Title);
    }

    connect(ui->OKButton, &QPushButton::clicked, this, &TimerInfoWindow::OKPushed);
    connect(ui->CancelButton, &QPushButton::clicked, this, &TimerInfoWindow::CancelPushed);

    connect(ui->PlayButton, &QPushButton::clicked, this, &TimerInfoWindow::playAudio);
    connect(ui->soundComboBox, &QComboBox::currentIndexChanged, this, &TimerInfoWindow::playAudio);
    connect(ui->PauseButton, &QPushButton::clicked, this, &TimerInfoWindow::pauseAudio);
}

TimerInfoWindow::~TimerInfoWindow()
{
    delete ui;
}

void TimerInfoWindow::copyComboBoxItems(QComboBox* source, QComboBox* destination)
{
    destination->clear();
    for (int i = 0; i < source->count(); ++i)
    {
        destination->addItem(source->itemIcon(i), source->itemText(i));
        destination->setItemData(i, source->itemData(i));
    }
}

void TimerInfoWindow::uploadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString imageName = QFileInfo(filePath).fileName();

    QIcon icon(filePath);
    ui->imageComboBox->addItem(icon, imageName);
    ui->imageComboBox->setItemData(ui->imageComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Image Uploaded"), tr("You have successfully uploaded the image: %1").arg(imageName));
}

void TimerInfoWindow::uploadSound()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Sound File"), QString(), tr("Audio Files (*.mp3 *.wav *.ogg)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString soundName = QFileInfo(filePath).fileName();

    ui->soundComboBox->addItem(soundName);
    ui->soundComboBox->setItemData(ui->soundComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Audio File Uploaded"), tr("You have successfully uploaded the audio file: %1").arg(soundName));
}

void TimerInfoWindow::uploadApp()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Application"), "", tr("Executable Files (*.exe)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString appName = QFileInfo(filePath).fileName();
    QIcon appIcon(filePath);

    ui->appComboBox->addItem(appIcon, appName);
    ui->appComboBox->setItemData(ui->appComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Application Uploaded"), tr("You have successfully uploaded the application: %1").arg(appName));
}


void TimerInfoWindow::uploadDocument()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Document"), "", tr("Documents (*.pdf *.txt *.docx *.xlsx)"));
    if (filePath.isEmpty()) {
        return;
    }

    QString docName = QFileInfo(filePath).fileName();
    ui->documentComboBox->addItem(docName);
    ui->documentComboBox->setItemData(ui->documentComboBox->count() - 1, filePath);

    QMessageBox::information(this, tr("Document Uploaded"), tr("You have successfully uploaded the document: %1").arg(docName));
}

void TimerInfoWindow::playAudio()
{
    ui->PlayButton->hide();
    ui->PauseButton->show();
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    player->setAudioOutput(audioOutput);
    QString soundName = ui->soundComboBox->currentData().toString();
    if (soundName.startsWith("qrc:/"))
        player->setSource(QUrl(soundName));
    else
        player->setSource(QUrl::fromLocalFile(soundName));
    audioOutput->setVolume(50);
    player->play();

    audioTimer->setSingleShot(true);
    audioTimer->start(3000);
    connect(audioTimer, &QTimer::timeout, [this]() {
        if (player->isPlaying()) {
            player->stop();
        }
        if (audioTimer->isActive()) {
            audioTimer->stop();
        }
        ui->PlayButton->show();
        ui->PauseButton->hide();
    });
}

void TimerInfoWindow::pauseAudio()
{
    if(player->isPlaying()) player->stop();
    ui->PlayButton->show();
    ui->PauseButton->hide();
    if(audioTimer->isActive()) audioTimer->stop();
}

void TimerInfoWindow::OKPushed()
{
    if (ui->soundCheckBox_3->isChecked())
        timerInfo.soundName = ui->soundComboBox->currentData().toString();
    else
        timerInfo.soundName="";
    if (ui->imageCheckBox->isChecked())
        timerInfo.imageName = ui->imageComboBox->currentData().toString();
    else
        timerInfo.imageName="";
    if (ui->appCheckBox->isChecked())
        timerInfo.appName = ui->appComboBox->currentData().toString();
    else
        timerInfo.appName="";
    if (ui->documentCheckBox->isChecked())
        timerInfo.documentName = ui->documentComboBox->currentData().toString();
    else
        timerInfo.documentName="";

    timerInfo.Title = ui->timerName->text();

    int hours = ui->hourCount ? ui->hourCount->value() : 0;
    int minutes = ui->minuteCount ? ui->minuteCount->value() : 0;
    int seconds = ui->secondCount ? ui->secondCount->value() : 0;

    if(ui->AmPmBox->currentText() == "AM") timer->AmPm = "AM";
    else timer->AmPm = "PM";

    timerInfo.selectedDateTime.setTime(QTime(hours, minutes, seconds));
    timerInfo.selectedDateTime.setDate(ui->calendarWidget->selectedDate());



    if (timerInfo.Ttype == TimerType::Alarm)
    {

        QDateTime currentDateTime=timer->changeTime();
        QDateTime adjustedSelectedDateTime = timerInfo.selectedDateTime;
        if (timer->TimeFormat != "HH:mm:ss")
        {
            if (timer->AmPm == "PM" && adjustedSelectedDateTime.time().hour() != 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(12 * 3600);
            else if (timer->AmPm == "AM" && adjustedSelectedDateTime.time().hour() == 12)
                adjustedSelectedDateTime = adjustedSelectedDateTime.addSecs(-12 * 3600);
        }
        int secondsToPlay = currentDateTime.secsTo(adjustedSelectedDateTime);
        if (secondsToPlay <= 0)
        {
            QMessageBox::about(this, "Error.", "The selected time has already passed. Please choose another one.");
            return;
        }
    }

    emit timerInfoUpdated(timerInfo, timer);
    this->close();
}


void TimerInfoWindow::CancelPushed()
{
    this->close();
}

void TimerInfoWindow::setupUI()
{
    ui->horizontalLayout_38->setAlignment(ui->AlarmRadioButton, Qt::AlignCenter);
    ui->horizontalLayout_37->setAlignment(ui->TimerRadioButton, Qt::AlignCenter);
    ui->verticalLayout_9->setAlignment(ui->calendarWidget, Qt::AlignCenter);
    ui->verticalLayout_9->setAlignment(ui->timerName, Qt::AlignCenter);
    ui->verticalLayout_5->setAlignment(ui->soundCheckBox_3, Qt::AlignCenter);
    ui->verticalLayout_6->setAlignment(ui->imageCheckBox, Qt::AlignCenter);
    ui->verticalLayout_8->setAlignment(ui->documentCheckBox, Qt::AlignCenter);
    ui->verticalLayout_7->setAlignment(ui->appCheckBox, Qt::AlignCenter);
    setStyleSheet(R"(
QCheckBox {
        color: white;
    }
    QCheckBox::indicator {
        width: 20px;
        height: 20px;
    }

QPushButton
{
        font-size: 14px;
        background-color: rgba(75, 0, 130, 1);
        color: #f0f0f0;
        padding: 10px;
        border-radius: 8px;
        border: 1px solid white;
}
QPushButton:hover {
background-color: rgba(75, 0, 130, 0.8); }
QPushButton:pressed {
background-color: rgba(75, 0, 130, 0.8); }

    QComboBox
 {
        font-size: 14px;
        padding: 6px;
        border: 1px solid white;
        border-radius: 8px;
        background-color: #3c3f41;
        color: white;
    }
    QComboBox QAbstractItemView
 {
        background-color: #3c3f41;
        selection-background-color: #5c5f61;
        color: white;
    }

QLineEdit {
        font-size: 14px;
        background-color: indigo;
        color: white;
        border: 1px solid white;
        border-radius: 8px;
        padding: 5px;
    }



    QSpinBox {
        background-color: lightgray;
        color: black;
        border: 1px solid darkgray;
        padding: 5px;
        font-size: 14px;
        border-radius: 5px;
    }

QSpinBox:focus {
        border: 1px solid indigo;
        background-color: white;
    }
    QSpinBox::up-button, QSpinBox::down-button {
        background-color: darkgray;
        border: none;
        width: 15px;
    }
    QSpinBox::up-button:hover, QSpinBox::down-button:hover {
        background-color: rgba(75, 0, 130, 0.8);;
    }
    QSpinBox::up-button:pressed, QSpinBox::down-button:pressed {
        background-color: rgba(75, 0, 130, 0.7);;
    }

QLabel {
        font-family: 'Oxanium';
        font-size: 20px;
}

QPushButton#OKButton {
    background-color: rgba(0, 110, 4, 1)
}

QPushButton#OKButton:hover {
    background-color: rgba(0, 110, 4, 0.8)
}

QPushButton#OKButton:pressed {
    background-color: rgba(0, 110, 4, 0.8)
}

QPushButton#CancelButton {
    background-color: rgba(255, 140, 0, 1)
}

QPushButton#CancelButton:hover {
    background-color: rgba(255, 140, 0, 0.8)
}

QPushButton#CancelButton:pressed {
    background-color: rgba(255, 140, 0, 0.8)
}
)");
}
