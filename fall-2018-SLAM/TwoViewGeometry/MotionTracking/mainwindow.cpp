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
    timer->setInterval(100);
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
    gvm::AddCartesianAxes();
    gvm::ShowAddedVertices();
}

void MainWindow::on_pushButton_setup_clicked()
{
    bool done = motionTracker->setup();
    gvm::AddCartesianAxes();
    gvm::ShowAddedVertices();

    if(done)
        timer->stop();
    else
        timer->start();
}
