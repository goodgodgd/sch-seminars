#include "myqtapp.h"
#include "ui_myqtapp.h"

#include <QFileDialog>
#include <QDebug>

#define USE_FUNCTIONAL


MyQtApp::MyQtApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyQtApp)
{
    ui->setupUi(this);
    ui->lineEdit_imagesize->setText("400,300");
    ui->lineEdit_circleparams->setText("200,100,50");
    defaultPath = QString(PRJ_PATH) + "/../../images";
}

MyQtApp::~MyQtApp()
{
    delete ui;
}

/// study point: how to create functions linked to guis
void MyQtApp::on_pushButton_openimage_clicked()
{
    /// study point: QString usage
    QString caption = "Open image";
    QString pattern = "Images (*.png *.xpm *.jpg)";
    /// study point: file open gui box
    QString fileName = QFileDialog::getOpenFileName(this, caption, defaultPath, pattern);

    /// study point: read and show image
    image = cv::imread(fileName.toStdString());
    cv::imshow("image", image);
}

void MyQtApp::on_pushButton_writeimage_clicked()
{
    if(image.empty())
    {
        qDebug() << "image is empty";
        return;
    }
    QString name = ui->lineEdit_imagename->text();
    if(name.isEmpty())
    {
        qDebug() << "save name is empty";
        return;
    }

    // study point: how to access and write pixel value
    // invert image
    cv::Vec3b white(255,255,255);
    for(int y=0; y<image.rows; y++)
        for(int x=0; x<image.cols; x++)
            image.at<cv::Vec3b>(y,x) = white - image.at<cv::Vec3b>(y,x);

    cv::imshow("image", image);
    QString filename = defaultPath + "/" + name + ".jpg";
    cv::imwrite(filename.toStdString(), image);
}

void MyQtApp::on_pushButton_resetimage_clicked()
{
    drawer.initImage(ui->lineEdit_imagesize->text());
}

void MyQtApp::on_pushButton_draw_circle_objects_clicked()
{
    drawer.pushCircle(ui->lineEdit_circleparams->text());
}

void MyQtApp::on_pushButton_undo_circle_clicked()
{
    drawer.popShape();
}

void MyQtApp::on_pushButton_draw_rect_objects_clicked()
{
    drawer.pushRect(ui->lineEdit_circleparams->text());
}
