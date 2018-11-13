#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <eigen3/Eigen/Eigen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // add opengl widget
    glwidget = new GlWidget();
    ui->verticalLayout->addWidget(glwidget);
    // allocate memory for opengl vertices
    gvm::InitVertices();
    gvm::AddCartesianAxes();
    gvm::ShowAddedVertices();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
    timer->start();

    motionTracker = new MotionTracker;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_reset_view_clicked()
{
    glwidget->ResetView();
}

void MainWindow::on_pushButton_clicked()
{
    Eigen::Vector3f translation(ui->doubleSpinBox_tx->value(),
                                ui->doubleSpinBox_ty->value(),
                                ui->doubleSpinBox_tz->value());
    Eigen::Vector3f rotation(ui->doubleSpinBox_rx->value(),
                             ui->doubleSpinBox_ry->value(),
                             ui->doubleSpinBox_rz->value());
    Pose6DofQt currPose(translation, rotation);

    if(cars.empty())
        cars.emplace_back(currPose);
    else
    {
        Pose6DofQt lastPose = cars.back().getPose();
        Pose6DofQt neoGlobalPose = lastPose * currPose;
        cars.emplace_back(neoGlobalPose);
    }

    // draw cars
    for(Car car: cars)
        car.draw();

    gvm::AddCartesianAxes();
    gvm::ShowAddedVertices();
}

void MainWindow::on_pushButton_start_clicked()
{
    if(timer->isActive())
        timer->stop();
    else
        timer->start();
}

void MainWindow::on_timer()
{
    motionTracker->run();
}

void MainWindow::on_pushButton_setup_clicked()
{
    motionTracker->setup();
}
