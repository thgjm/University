#ifndef TIMERINFOWINDOW_H
#define TIMERINFOWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QComboBox>
#include <QAbstractButton>
#include <QFileDialog>
#include "timerinfo.h"
#include <QMediaPlayer>
#include <QAudioOutput>

class SettingsWindow;

class Timer;

namespace Ui {
class TimerInfoWindow;
}

class TimerInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TimerInfoWindow(QWidget *parent, TimerInfo timerInfo, SettingsWindow* settings, Timer *timer);
    ~TimerInfoWindow();
    void copyComboBoxItems(QComboBox* source, QComboBox* destination);
    void setupUI();

signals:
    void timerInfoUpdated(TimerInfo timerInfo, Timer *timer);

private slots:
    void uploadImage();
    void uploadSound();
    void uploadApp();
    void uploadDocument();
    void OKPushed();
    void CancelPushed();
    void playAudio();
    void pauseAudio();

private:
    Ui::TimerInfoWindow *ui;
    SettingsWindow* settings;
    TimerInfo timerInfo;
    Timer *timer;
    QTimer *audioTimer;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};

#endif // TIMERINFOWINDOW_H
