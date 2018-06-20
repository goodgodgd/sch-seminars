#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    char path[100];
    sprintf(path, "%s/lena.png", PROJECT_PATH);
    cv::Mat img = cv::imread(path);
    cv::imshow("lena", img);
}
