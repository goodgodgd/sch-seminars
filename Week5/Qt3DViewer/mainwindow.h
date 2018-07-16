#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Share/glwidget.h"


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
    void drawFrame();

    void on_pushButton_reset_view_clicked();

private:
    Ui::MainWindow *ui;
    GlWidget* glwidget;
    QTimer* timer;
};

#endif // MAINWINDOW_H
