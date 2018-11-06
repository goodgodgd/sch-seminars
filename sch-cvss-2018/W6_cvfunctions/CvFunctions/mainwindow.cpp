#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButton_rgb->setChecked(true);
    ui->radioButton_pixel_value->setChecked(true);
    ui->lineEdit_hist_box_size->setText("30");
    ui->lineEdit_floodfill_thresh->setText("2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_load_image_clicked()
{
    QString caption = "Open image";
    QString defaultPath = QString(PRJ_PATH) + "/../../images";
    QString pattern = "Images (*.png *.xpm *.jpg)";
    QString fileName = QFileDialog::getOpenFileName(this, caption, defaultPath, pattern);

    imgman.emplace_back(fileName);
}

void MainWindow::on_radioButton_rgb_toggled(bool checked)
{
    if(checked) convertColor(COLOR::RGB);
}

void MainWindow::on_radioButton_lab_toggled(bool checked)
{
    if(checked) convertColor(COLOR::LAB);
}

void MainWindow::on_radioButton_ycrcb_toggled(bool checked)
{
    if(checked) convertColor(COLOR::YCrCb);
}

void MainWindow::on_radioButton_hsv_toggled(bool checked)
{
    if(checked) convertColor(COLOR::HSV);
}

void MainWindow::convertColor(COLOR color)
{
    for(auto& im: imgman)
        im.convertColor(color);
}

void MainWindow::on_radioButton_pixel_value_toggled(bool checked)
{
    setEffect(EFFECT::VALUE);
}

void MainWindow::on_radioButton_histogram_toggled(bool checked)
{
    setEffect(EFFECT::HISTOGRAM);
}

void MainWindow::on_lineEdit_hist_box_size_editingFinished()
{
    setEffect(EFFECT::HISTOGRAM);
}

void MainWindow::on_radioButton_floodfill_toggled(bool checked)
{
    setEffect(EFFECT::FLOODFILL);
}

void MainWindow::on_lineEdit_floodfill_thresh_editingFinished()
{
    setEffect(EFFECT::FLOODFILL);
}

void MainWindow::setEffect(EFFECT effect)
{
    bool success = false;
    int value = 0;
    if(effect == EFFECT::HISTOGRAM)
        value = ui->lineEdit_hist_box_size->text().toInt(&success);
    if(effect == EFFECT::FLOODFILL)
        value = ui->lineEdit_floodfill_thresh->text().toInt(&success);
    if(!success)
        return;

    for(auto& im: imgman)
        im.setEffect(effect, value);
}
