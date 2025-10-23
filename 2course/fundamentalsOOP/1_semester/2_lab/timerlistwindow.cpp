#include "timerlistwindow.h"
#include "settingswindow.h"

TimerListWindow::TimerListWindow(QWidget *parent, SettingsWindow *settings)
    : QDialog(parent), settings(settings)
{
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint |
                   Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    setWindowModality(Qt::NonModal);

    mainLayout = new QVBoxLayout(this);

    QLabel *timersLabel = new QLabel("Timers", this);
    timersLabel->setAlignment(Qt::AlignCenter);
    timersLabel->setStyleSheet("font-size: 30px; font-weight: bold; font-style: italic; color: orange; margin: 10px 0px; padding: 5px;");
    mainLayout->addWidget(timersLabel);

    timerSection = new QWidget(this);
    alarmSection = new QWidget(this);

    timerLayout = new QVBoxLayout(timerSection);
    timerSection->setLayout(timerLayout);

    alarmLayout = new QVBoxLayout(alarmSection);
    alarmSection->setLayout(alarmLayout);

    timerScrollArea = new QScrollArea(this);
    timerScrollArea->setWidget(timerSection);
    timerScrollArea->setWidgetResizable(true);

    alarmScrollArea = new QScrollArea(this);
    alarmScrollArea->setWidget(alarmSection);
    alarmScrollArea->setWidgetResizable(true);

    mainLayout->addWidget(timerScrollArea);

    QLabel *alarmsLabel = new QLabel("Alarms", this);
    alarmsLabel->setAlignment(Qt::AlignCenter);
    alarmsLabel->setStyleSheet("font-size: 30px; font-weight: bold; font-style: italic; color: orange; margin: 10px 0px; padding: 5px;");
    mainLayout->addWidget(alarmsLabel);

    mainLayout->addWidget(alarmScrollArea);

    setLayout(mainLayout);

    connect(settings, &SettingsWindow::timerCreated, this, &TimerListWindow::addTimerWidget);

}

TimerListWindow::~TimerListWindow()
{
    qDeleteAll(timerWidgets);
}

void TimerListWindow::addTimerWidget(const TimerInfo &timerInfo)
{
    TimerWidget *newTimerWidget = new TimerWidget(this, timerInfo, settings);
    if (newTimerWidget->timerInfo.Ttype == TimerType::Timer)
        timerLayout->addWidget(newTimerWidget);
    else if (newTimerWidget->timerInfo.Ttype == TimerType::Alarm)
        alarmLayout->addWidget(newTimerWidget);
    timerWidgets.append(newTimerWidget);
    connect(newTimerWidget, &TimerWidget::timerDeleted, this, &TimerListWindow::removeTimerWidget);
}

void TimerListWindow::removeTimerWidget(TimerWidget *timerWidget)
{
    if (timerWidgets.contains(timerWidget)) {
        timerWidgets.removeAll(timerWidget);
        if (timerWidget->timerInfo.Ttype == TimerType::Timer) {
            timerLayout->removeWidget(timerWidget);
        } else if (timerWidget->timerInfo.Ttype == TimerType::Alarm) {
            alarmLayout->removeWidget(timerWidget);
        }
        timerWidget->deleteLater();
    }
}

void TimerListWindow::clearAllTimers()
{
    for (TimerWidget *timerWidget : std::as_const(timerWidgets))
    {
        timerWidget->deleteTimer();
        if (timerWidget->timerInfo.Ttype == TimerType::Timer) {
            timerLayout->removeWidget(timerWidget);
        } else if (timerWidget->timerInfo.Ttype == TimerType::Alarm) {
            alarmLayout->removeWidget(timerWidget);
        }
    }
    qDeleteAll(timerWidgets);
    timerWidgets.clear();
    qDebug() << "All timers have been cleared.";
}
