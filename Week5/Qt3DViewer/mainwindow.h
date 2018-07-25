#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Modules/glwidget.h"
#include "Modules/car.h"

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
    void on_pushButton_reset_view_clicked();
    void on_pushButton_clicked();

private:
    void eigenExamples();

    Ui::MainWindow *ui;
    GlWidget* glwidget;

    std::vector<Car> cars;


};

#endif // MAINWINDOW_H
