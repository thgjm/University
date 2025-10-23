#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTimeZone>
#include "digitalclock.h"
#include "settingswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    TimerListWindow *Tlists;
    void clearTimerList();
    void setupUI();

public slots:
    void setTimerListWindow(TimerListWindow* tlists);

private slots:
    void Clock(const QTimeZone &TimeZone);
    QString FillTimeZones();
    void onTimeZoneChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void openSettingsWindow();

    void showTimerList();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *layout;
    DigitalClock* clock;
    QTimeZone timeZone;
    QStringList timeZones;
    QTime selectedTime;
    QString TimeFormat;
    SettingsWindow *sett;
};
#endif // MAINWINDOW_H
