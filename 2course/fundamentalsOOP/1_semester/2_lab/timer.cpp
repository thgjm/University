#include "Timer.h"

Timer::Timer(QObject *parent)
    : QObject(parent), hourCount(nullptr), minuteCount(nullptr), secondCount(nullptr), AmPm(""), timer(new QTimer(this)), timeZone()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Timer::timeout);
}

void Timer::start(int msec)
{
    timer->start(msec);
}

void Timer::stop()
{
    if(timer)
    timer->stop();
}

void Timer::setSpinBoxes(QSpinBox *hours, QSpinBox *minutes, QSpinBox *seconds)
{
    hourCount = hours;
    minuteCount=minutes;
    secondCount=seconds;
}

QTime Timer::getInitialTime()
{
    int hours = hourCount ? hourCount->value() : 0;
    int minutes = minuteCount ? minuteCount->value() : 0;
    int seconds = secondCount ? secondCount->value() : 0;
    return QTime(hours, minutes, seconds);
}

void Timer::setTimeZone(QTimeZone *zone)
{
    timeZone=zone;
}

QDateTime Timer::changeTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime().toTimeZone(*timeZone);
    return currentDateTime;
}



int Timer::getRemainingTime() {

    int remainingTime = timer->remainingTime()/1000;
    return remainingTime+1;
}

bool Timer::isActive()
{
    if (!timer)
        return false;
    return timer->isActive();
}
