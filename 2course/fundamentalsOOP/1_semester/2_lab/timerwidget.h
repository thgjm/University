#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QStackedWidget>
#include <QDialog>
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTimeZone>
#include "imagewindow.h"
#include <QRadioButton>
#include <QLabel>
#include <QFontDatabase>
#include <QMessageBox>
#include <QIcon>
#include <QFileInfo>
#include <QProcess>
#include <QDesktopServices>
#include "countdown.h"
#include "timerinfowindow.h"

class SettingsWindow;

namespace Ui {
class TimerWidget;
}

class TimerWidget : public QStackedWidget
{
    Q_OBJECT

public:
    explicit TimerWidget(QWidget *parent, TimerInfo timerInfo, SettingsWindow* settings);
    ~TimerWidget();

    TimerInfo timerInfo;

    bool timerPaused = false;
    bool AlarmOn = true;
    int remainingTime = 0;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    imageWindow *img;
    CountDown *countDown;
    TimerInfoWindow *info;
    SettingsWindow *settings;
    Timer *timer;
    void setupUI();

signals:
    void timerDeleted(TimerWidget* timerWidget);

public slots:
    void deleteTimer();

private slots:
    void TimerStart();
    void PlaySoundtrack();
    void ShowImage();
    void openApplication();
    void openDocument();
    void updateTimeoutConnections();
    void resetTimer();
    void PauseResume();
    void generalTimeout();
    void openInfoWindow();
    void updateTimerInfo(TimerInfo timerInfo, Timer *timer);

    void SetAlarmDisplay();
    void TurnAlarmOnOff();

private:
    Ui::TimerWidget *ui;

    /*TimerInfo timerInfo;

    bool timerPaused = false;
    bool AlarmOn = true;
    int remainingTime = 0;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    imageWindow *img;
    CountDown *countDown;
    TimerInfoWindow *info;
    SettingsWindow *settings;*/
};

#endif // TIMERWIDGET_H
