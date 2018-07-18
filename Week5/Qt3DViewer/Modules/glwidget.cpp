#include "glwidget.h"
#include <math.h>

GlWidget::GlWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    lightpos = QVector3D(0,0,3);
    rot_radius = 0.1f;
    projection.setToIdentity();
    projection.perspective(45, 4.f/3.f, 0.1f, 100.f);

    // determine default rotation and translation in advance
//    rot_default.setVector(0, 0, 0.01);
//    rot_default.setScalar(1);
//    rot_default.normalize();
//    trn_default = QVector3D(0, 0, -1);
//    viewPose.setToIdentity();
//    viewPose.translate(trn_default);
//    viewPose.rotate(rot_default);

    defaultPose = QMatrix4x4(    0.873826,  0.003236,  0.48622, -0.649472
                                ,0.015838, -0.999637, -0.02180,  0.148617
                                ,0.485981,  0.026758, -0.87356,  1.47186
                                ,0       ,  0       ,  0      ,  1);
    viewPose = defaultPose;

    setFocusPolicy(Qt::StrongFocus);
}

void GlWidget::ResetView()
{
//    viewPose.setToIdentity();
//    viewPose.translate(trn_default);
//    viewPose.rotate(rot_default);

    viewPose = defaultPose;
}

void GlWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);

    // link shaders
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, QString(ProjectPath) + "/Modules/vertex.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, QString(ProjectPath) + "/Modules/fragment.frag");
    program.bindAttributeLocation("vertex", 0);
    program.bindAttributeLocation("normal", 1);
    program.bindAttributeLocation("color", 2);
    program.link();
    program.bind();

    vertexLocation = program.attributeLocation("vertex");
    normalLocation = program.attributeLocation("normal");
    colorLocation = program.attributeLocation("color");
    ptsizeLocation = program.attributeLocation("ptsize");
    mvpMatrixLocation = program.uniformLocation("mvpMatrix");
    lightPosLocation = program.uniformLocation("lightPos");

    // Light position is fixed.
    program.setUniformValue(lightPosLocation, QVector3D(0, 0, 70));
    program.release();

    // Use QBasicTimer because its faster than QTimer
    timer.start(30, this);
}

void GlWidget::paintGL()
{
    glClearColor(0,0,0,1); // this must be the first among gl_functions
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // set viewMatrix from viewPose
    QMatrix4x4 viewMatrix;
    QVector3D localPoint;
    localPoint = QVector3D(0,0,0);
    QVector3D eye = viewPose.map(localPoint);
    localPoint = QVector3D(0,0,1);
    QVector3D center = viewPose.map(localPoint);
    localPoint = QVector3D(0,-1,0);
    QVector3D up = viewPose.map(localPoint) - eye;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(eye,center,up);

    program.bind();
    program.enableAttributeArray(vertexLocation);
    program.enableAttributeArray(normalLocation);
    program.enableAttributeArray(colorLocation);
    program.enableAttributeArray(ptsizeLocation);

    program.setAttributeArray(vertexLocation, gvm::PositPtr());
    program.setAttributeArray(normalLocation, gvm::NormalPtr());
    program.setAttributeArray(colorLocation, gvm::ColorPtr());
    program.setAttributeArray(ptsizeLocation, gvm::PtsizePtr(), 1);
    program.setUniformValue(mvpMatrixLocation, projection * viewMatrix);
    program.setUniformValue(lightPosLocation, lightpos);

    glDrawArrays(GL_POINTS, gvm::PtBegin(), gvm::PtNum());
    glDrawArrays(GL_LINES, gvm::LnBegin(), gvm::LnNum());
    glDrawArrays(GL_TRIANGLES, gvm::TrBegin(), gvm::TrNum());

    program.disableAttributeArray(vertexLocation);
    program.disableAttributeArray(normalLocation);
    program.disableAttributeArray(colorLocation);
    program.disableAttributeArray(ptsizeLocation);
    program.release();
}

void GlWidget::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    projection.setToIdentity();
    projection.perspective(45, (qreal)w/(qreal)h, 0.1f, 100.f);
}

void GlWidget::timerEvent(QTimerEvent *e)
{
    // Update scene
    update();
}

void GlWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePress = QVector2D(e->localPos());
}

void GlWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // compute mose movement
    QVector2D mouseMove = QVector2D(e->localPos()) - mousePress;

    const float degScale = 0.1f;
    float rotDegree=0;
    QMatrix4x4 rotMatrix;
    QVector3D transVec;

    // rotation and translation operations are applied in local frame
    // .rotate(add) -> Mat_old * Mat_add
    // matrix transforms local point to global point
    // translate first and then rotate

    // ignore small movement axis
    // when draged horizontally, rotate about Y-axis
    if(fabsf(mouseMove.x()) > fabsf(mouseMove.y()))
    {
        // set rotation
        rotDegree = mouseMove.x()*degScale;
        rotMatrix.setToIdentity();
        rotMatrix.rotate(rotDegree, 0,-1,0);
        // calculate translation (local frame)
        transVec = rot_radius*(QVector3D(1,0,0) - rotMatrix*QVector3D(1,0,0));
        // commit translation and rotation
        viewPose.translate(transVec);
        viewPose.rotate(rotDegree, 0,-1,0);
    }
    // when draged vertically, rotate about X-axis
    else
    {
        // set rotation
        rotDegree = -mouseMove.y()*degScale;
        rotMatrix.setToIdentity();
        rotMatrix.rotate(rotDegree, -1,0,0);
        // calculate translation (local frame)
        transVec = rot_radius*(QVector3D(1,0,0) - rotMatrix*QVector3D(1,0,0));
        // commit translation and rotation
        viewPose.translate(transVec);
        viewPose.rotate(rotDegree, -1,0,0);
    }

    qDebug() << "updated pose (drag)" << viewPose;
}

void GlWidget::wheelEvent(QWheelEvent* e)
{
    const float moveScale = 0.0002f;
    float move = moveScale * e->delta();
    viewPose.translate(0,0,move);

//    qDebug() << "updated pose (wheel)" << viewPose;
}

void GlWidget::keyPressEvent(QKeyEvent* e)
{
    static float moveScale = 0.02f;
    if(e->key() == Qt::Key_Up)
        viewPose.translate(0,-moveScale,0);
    else if(e->key() == Qt::Key_Down)
        viewPose.translate(0,moveScale,0);
    else if(e->key() == Qt::Key_Left)
        viewPose.translate(-moveScale,0,0);
    else if(e->key() == Qt::Key_Right)
        viewPose.translate(moveScale,0,0);
    else if(e->key() == Qt::Key_PageUp)
        moveScale *= 2;
    else if(e->key() == Qt::Key_PageDown)
        moveScale /= 2;
}
