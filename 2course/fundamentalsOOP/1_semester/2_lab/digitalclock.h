#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QLabel>
#include <QFont>
#include <QComboBox>
#include <QFontDatabase>

class DigitalClock : public QObject {
    Q_OBJECT

public:
    explicit DigitalClock(QObject *parent = nullptr);
    void start();
    void stop();
    void setDisplay(QLabel *timeLabel);
    void setTimeZone(QTimeZone *zone);
    void changeFormat(QString format);

signals:
    void timeUpdated(QString &timeText);

private slots:
    void updateTime();

private:
    QTimer *clock;
    QLabel *timeLabel;
    QTimeZone *timeZone;
    QString TimeFormat;
};

#endif // DIGITALCLOCK_H
