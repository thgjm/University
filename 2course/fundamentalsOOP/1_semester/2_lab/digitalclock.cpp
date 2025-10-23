#include "digitalclock.h"

DigitalClock::DigitalClock(QObject *parent)
    : QObject(parent), timeLabel(nullptr), TimeFormat("HH:mm:ss") {
    clock = new QTimer(this);
    connect(clock, &QTimer::timeout, this, &DigitalClock::updateTime);
}

void DigitalClock::start() {
    clock->start(1000);
    updateTime();
}

void DigitalClock::stop() {
    clock->stop();
}

void DigitalClock::setDisplay(QLabel *display)
{
    timeLabel = display;
    timeLabel->setStyleSheet("QLabel { color : white; }");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Orbitron/static/Orbitron-Regular.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        timeLabel->setFont(QFont(family, 65));
    } else {
        qDebug() << "The font was not uploaded.";
    }
}
void DigitalClock::updateTime() {

    QDateTime currentDateTime = QDateTime::currentDateTime().toTimeZone(*timeZone);
    QTime time = currentDateTime.time();
    QString timeText = time.toString(TimeFormat);

    if (timeLabel) {
        timeLabel->setText(timeText);
    }
    emit timeUpdated(timeText);
}

void DigitalClock::setTimeZone(QTimeZone *zone)
{
    timeZone=zone;
}

void DigitalClock::changeFormat(QString format)
{
    TimeFormat = format;
    updateTime();
}
