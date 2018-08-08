#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imagemanipulator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_load_image_clicked();
    void on_radioButton_rgb_toggled(bool checked);
    void on_radioButton_lab_toggled(bool checked);
    void on_radioButton_ycrcb_toggled(bool checked);
    void on_radioButton_hsv_toggled(bool checked);

    void on_radioButton_pixel_value_toggled(bool checked);
    void on_radioButton_histogram_toggled(bool checked);
    void on_lineEdit_hist_box_size_editingFinished();
    void on_radioButton_floodfill_toggled(bool checked);
    void on_lineEdit_floodfill_thresh_editingFinished();

private:
    Ui::MainWindow *ui;
    std::vector<ImageManipulator> imgman;

    void convertColor(COLOR color);
    void setEffect(EFFECT effect);
};

#endif // MAINWINDOW_H
