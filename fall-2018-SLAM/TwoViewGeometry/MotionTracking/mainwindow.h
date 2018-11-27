#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Modules/glwidget.h"
#include "Modules/motiontracker.h"

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
    void on_timer();
    void on_pushButton_reset_view_clicked();
    void on_pushButton_start_clicked();
    void on_pushButton_setup_clicked();

private:
    Ui::MainWindow *ui;
    GlWidget* glwidget;
    QTimer* timer;
    MotionTracker* motionTracker;
};

#endif // MAINWINDOW_H
