#ifndef MYQTAPP_H
#define MYQTAPP_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QString>

namespace Ui {
class MyQtApp;
}

class MyQtApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyQtApp(QWidget *parent = 0);
    ~MyQtApp();

private slots:
    void on_pushButton_openimage_clicked();
    void on_pushButton_resetimage_clicked();
    void on_pushButton_drawcircle_clicked();

    void on_pushButton_draw_circle_objects_clicked();

    void on_pushButton_writeimage_clicked();

private:
    QString defaultPath;
    cv::Mat createImage(const QString& paramString);
    Ui::MyQtApp *ui;
    cv::Mat image;
};

#endif // MYQTAPP_H
