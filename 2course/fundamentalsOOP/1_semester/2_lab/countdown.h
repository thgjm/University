#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QFont>
#include <QFontDatabase>

class CountDown : public QObject {
    Q_OBJECT

public:
    explicit CountDown(QObject *parent = nullptr, QTime countTime = QTime(0, 0));

    void start();
    void stop();
    void setDisplay(QLabel *timeLabel);

private slots:
    void updateTime();

private:
    QTimer *countDown;
    QLabel *countdownLabel;
    QTime countTime;
    QTime countTimeLeft;
};

#endif // COUNTDOWN_H
