#ifndef TIMERLISTWINDOW_H
#define TIMERLISTWINDOW_H

#include <QDialog>
#include <QScrollArea>
#include "timerwidget.h"

namespace Ui {
class TimerListWindow;
}

class TimerListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TimerListWindow(QWidget *parent, SettingsWindow *settings);
    ~TimerListWindow();
    void addTimerWidget(const TimerInfo &timerInfo);
    void removeTimerWidget(TimerWidget* timerWidget);

public slots:
    void clearAllTimers();

private:
    QList<TimerWidget*> timerWidgets;
    QVBoxLayout *mainLayout;
    QWidget *timerSection;
    QWidget *alarmSection;
    QVBoxLayout *timerLayout;
    QVBoxLayout *alarmLayout;
    QScrollArea *timerScrollArea;
    QScrollArea *alarmScrollArea;
    SettingsWindow *settings;
};

#endif // TIMERLISTWINDOW_H
