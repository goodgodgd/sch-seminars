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

    eigenExamples();

    Eigen::Vector3f vec(1,2,3);
    qDebug() << "print eigen vector" << vec;
}

void MainWindow::eigenExamples()
{
    Eigen::Vector3f vector(1,0,1);
    vector << 1, 2, 3;
    float a = vector(0);
    vector(1) = a;
    std::cout << "vector " << vector.transpose() << std::endl;

    Eigen::Matrix3f mat3;
    mat3 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    std::cout << "matrix " << mat3 << std::endl;

    Eigen::Affine3f aff;
    Eigen::Matrix4f mat4;

    Eigen::Vector3f pt(1,2,3);
    Eigen::Translation3f trans(pt);
    float angle = 1.f;
    Eigen::Vector3f axis(1,0,0);
    Eigen::AngleAxisf anax(angle, axis);

    aff = trans * anax;
    std::cout << "transform \n" << aff.matrix() << std::endl;

    aff = aff * aff;
    std::cout << "transform \n" << aff.matrix() << std::endl;

    Pose6Dof pose1(Eigen::Vector3f(1,2,3), Eigen::Vector3f(0, 0, 0.5f));
    std::cout << "pose1 \n" << pose1.affPose.matrix() << std::endl;
    Pose6Dof pose2(Eigen::Vector3f(2,1,0), Eigen::Vector3f(0, 0, 0.1f));
    std::cout << "pose2 \n" << pose2.affPose.matrix() << std::endl;
    Pose6Dof pose3 = pose2 * pose1;
    std::cout << "pose3 \n" << pose3.affPose.matrix() << std::endl;
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
