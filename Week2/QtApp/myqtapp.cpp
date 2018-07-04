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
    try {
        QString paramString = ui->lineEdit_imagesize->text();
        image = createImage(paramString);
        cv::imshow("image", image);

    }
    catch (ParsingException& exception) {
        qDebug() << "parsing exception" << exception.msg;
    }
}

cv::Mat MyQtApp::createImage(const QString& paramString)
{
    ParamParser parser(paramString);
    // study point: auto??
    auto& params = parser.getParams();
    if(params.size() != 2)
        throw ParsingException("reset image needs two parameters");

    cv::Mat image = cv::Mat(params[1], params[0], CV_8UC3);
    image.setTo(cv::Scalar(255,255,255));
    return image;
}

void MyQtApp::on_pushButton_drawcircle_clicked()
{
    try {
        QString params = ui->lineEdit_circleparams->text();
        ParamParser parser(params);
        CircleDrawer circle(parser.getParams());
        circle.draw(image);
        cv::imshow("image", image);
    }
    catch (ParsingException& exception) {
        qDebug() << "parsing exception" << exception.msg;
    }
    catch (CircleException& exception) {
        qDebug() << "circle exception" << exception.msg;
    }
}


// study point: c++ functional, please read
// https://en.cppreference.com/w/cpp/utility/functional/function
#include <functional>

void MyQtApp::on_pushButton_draw_circle_objects_clicked()
{
    auto func = std::bind(&MyQtApp::addCircle, this, ui->lineEdit_circleparams->text());
    drawCircle(func);
}

void MyQtApp::on_pushButton_undo_circle_clicked()
{
    auto func = [&](){ if(circles.size()==0) throw CircleException("no circle to draw");
                      circles.pop_back(); };
    drawCircle(func);
}

void MyQtApp::drawCircle(auto& func)
{
    try {
        func();
        qDebug() << "now there are" << circles.size() << "circles";

        image.setTo(cv::Scalar(255,255,255));
        // study point: c++11 new for loop style
        for(auto& circle: circles)
            circle.draw(image);
        cv::imshow("image", image);
    }
    catch (ParsingException& exception) {
        qDebug() << "parsing exception" << exception.msg;
    }
    catch (CircleException& exception) {
        qDebug() << "circle exception" << exception.msg;
    }
}

void MyQtApp::addCircle(QString params)
{
    ParamParser parser(params);
    CircleDrawer circle(parser.getParams());
    circles.push_back(circle);
}

