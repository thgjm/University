#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "timer.h"
#include "imagewindow.h"
#include <QCloseEvent>
#include <QIcon>
#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QDesktopServices>
#include "timerlistwindow.h"

class MainWindow;

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QTimeZone timezone, QString TimeFormat, QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    ~SettingsWindow();
    void setTimeZone(const QTimeZone &zone);
    void setTimeFormat(const QString &format);

    bool CheckTimeFormat();

    void setLists();

    Ui::SettingsWindow *ui;

    Timer *timer;

    void setupUI();

signals:
    void timerCreated(const TimerInfo &timerInfo);
    void timerListWindowCreated(TimerListWindow* tlists);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void updateTimeLabel();
    void openTimerLists();
    void uploadImage();
    void uploadSound();
    void uploadApp();
    void uploadDocument();
    void playAudio();
    void pauseAudio();

private:


    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QFile *file;
    QVBoxLayout *layout;
    QTimeZone timeZone;
    QTime selectedTime;
    imageWindow *img;
    QString TimeFormat;
    TimerType Ttype;
    QTimer *audioTimer;

    MainWindow *mainWindow;
    TimerListWindow *Tlists;
};

#endif // SETTINGSWINDOW_H
