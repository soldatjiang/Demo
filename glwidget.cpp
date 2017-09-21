#include "glwidget.h"
#include <QVector>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget* parent):
    QOpenGLWidget(parent),
    m_xRot(0),
    m_yRot(0)
{
}

GLWidget::~GLWidget()
{

}

void GLWidget::initShaders()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!m_program.link())
        close();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initShaders();
    m_program.bind();

    m_program.setUniformValue("lightPosition",QVector4D(6.0f,3.0f,5.0f,1.0f));//设置光源在空间中的位置
    m_program.setUniformValue("lightIntensity",QVector3D(0.9f,0.9f,0.9f));//设置光源强度

    loadOBJMTL();

    QVector3D eye(20, 13, 5);
    QVector3D center(0, 0, 0);
    QVector3D up(0, 1, 0);

    /*
        m_view为视点变换矩阵
        eye为眼睛在空间中的位置
        center为眼睛注视的中心
        up为向上的方向向量
    */

    m_view.setToIdentity();
    m_view.lookAt(eye,center,up);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
}

void GLWidget::resizeGL(int w,int h)
{
    glViewport(0,0,w,h);
    m_proj.setToIdentity();
    m_proj.perspective(60.0f,(float)w/h,0.3f,1000);
    update();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model;
    /*
        model为模型变换矩阵，设定模型在空间中的位置，大小，旋转
    */
    m_program.bind();
    model.setToIdentity();
    model.translate(-10.0f,0.0f,4.5f);
    model.scale(0.8f);
    model.rotate(-90,0,1,0);
    model.rotate(m_xRot / 16.0f, 1, 0, 0);
    model.rotate(m_yRot / 16.0f, 0, 1, 0);

    loaderBoard.drawObjEntity(&m_program,model,m_view,m_proj); //绘制实体

    model.rotate(90,0,1,0);
    model.translate(19.0f,0.0f,-9.3f);

    loaderCoordinate.drawObjEntity(&m_program,model,m_view,m_proj);
}

void GLWidget::loadOBJMTL()
{
    if(!loaderBoard.Load(":/obj/saber_c3_evk_v2_0_for_qt_edit2.obj",":/obj/saber_c3_evk_v2_0_for_qt_edit2.mtl"))
    {
        qDebug()<<"Fail to load model";
        exit(1);
    }

    loaderBoard.allocateVertexBuffer(&m_program);//分配图元空间*/

    if(!loaderCoordinate.Load(":/obj/coordinate.obj",":/obj/coordinate.mtl"))
    {
        qDebug()<<"Fail to load model";
        exit(1);
    }

    loaderCoordinate.allocateVertexBuffer(&m_program);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600,400);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(60,60);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos=event->pos();
}
\
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx=event->x()-m_lastPos.x();
    int dy=event->y()-m_lastPos.y();

    m_xRot+=dx;
    m_yRot+=dy;
    update();
}
