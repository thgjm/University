#include "countdown.h"

CountDown::CountDown(QObject *parent, QTime countTime)
    : QObject(parent), countdownLabel(nullptr), countTime(countTime)  {
    countDown = new QTimer(this);
    connect(countDown, &QTimer::timeout, this, &CountDown::updateTime);
}


void CountDown::start() {
    countTimeLeft = countTime;
    updateTime();
    countDown->start(1000);
}

void CountDown::stop() {
    disconnect(countDown, &QTimer::timeout, this, &CountDown::updateTime);
    countDown->stop();
}

void CountDown::setDisplay(QLabel *display)
{
    countdownLabel = display;
    countdownLabel->setStyleSheet("QLabel { color : orange; }");

    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Orbitron/static/Orbitron-Regular.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        countdownLabel->setFont(QFont(family, 40));
    } else
        qDebug() << "The font was not uploaded.";
}

void CountDown::updateTime() {

    if (countTimeLeft > QTime(0, 0)) {
        QString timeText = countTimeLeft.toString("HH:mm:ss");
        if (countdownLabel)
            countdownLabel->setText(timeText);
        countTimeLeft = countTimeLeft.addSecs(-1);
    } else {
        countdownLabel->setText("00:00:00");
        countDown->stop();
    }
}
