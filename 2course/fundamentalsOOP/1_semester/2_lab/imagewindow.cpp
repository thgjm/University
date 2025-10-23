#include "imagewindow.h"
#include "ui_imagewindow.h"

imageWindow::imageWindow(QWidget *parent, QString FileName)
    : QDialog(parent)
    , ui(new Ui::imageWindow), imgName(FileName)
{
    ui->setupUi(this);
    QPixmap pix(imgName);
    int w = ui->label_pic->width();
    int h = ui->label_pic->height();
    ui->label_pic->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
}

imageWindow::~imageWindow()
{
    delete ui;
}


void imageWindow::loadImage()
{
    QPixmap pix(imgName);
    ui->label_pic->setPixmap(pix.scaled(ui->label_pic->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void imageWindow::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    loadImage();
}
