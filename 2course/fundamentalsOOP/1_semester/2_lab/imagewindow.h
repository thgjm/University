#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QDialog>
#include <QPixmap>
#include <QString>

namespace Ui {
class imageWindow;
}

class imageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit imageWindow(QWidget *parent = nullptr, QString FileName = QString());
    ~imageWindow();

    void loadImage();
    void resizeEvent(QResizeEvent *event);

private:
    Ui::imageWindow *ui;
    QString imgName;
};

#endif // IMAGEWINDOW_H
