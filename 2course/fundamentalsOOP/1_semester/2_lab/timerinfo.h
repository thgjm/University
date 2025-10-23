#ifndef TIMERINFO_H
#define TIMERINFO_H

#include <QString>
#include <QDateTime>
#include "timer.h"

enum class TimerType {Timer, Alarm};

class TimerInfo {
public:
    TimerInfo(QString soundName,
              QString imageName,
              QString appName,
              QString documentName,
              QString Title,
              QDateTime selectedDateTime,
              Timer *timer,
              TimerType Ttype
              )
        : soundName(soundName), imageName(imageName), appName(appName),
        documentName(documentName), Title(Title), selectedDateTime(selectedDateTime),  timer(timer), Ttype(Ttype) {}

    QString soundName;
    QString imageName;
    QString appName;
    QString documentName;
    QString Title;
    QDateTime selectedDateTime;
    Timer* timer;
    TimerType Ttype;
};

#endif // TIMERINFO_H
