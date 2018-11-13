#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QBasicTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "glvertexmanager.h"


class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GlWidget(QWidget *parent = 0);
    void ResetView();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void timerEvent(QTimerEvent *e);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent* e);
    void keyPressEvent(QKeyEvent* e);

private:
    QOpenGLContext *context;
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    QMatrix4x4 projection;
    QMatrix4x4 viewPose;
    QMatrix4x4 defaultPose;
    QVector3D lightpos;
    QQuaternion rot_default;
    QVector3D trn_default;
    float rot_radius;
    QVector2D mousePress;

    int vertexLocation;
    int normalLocation;
    int colorLocation;
    int ptsizeLocation;
    int mvpMatrixLocation;
    int lightPosLocation;

signals:

public slots:

};

#endif // GLWIDGET_H
