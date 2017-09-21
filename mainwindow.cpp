
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    glWidget=new GLWidget;
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    glWidget->setFormat(format);
    setCentralWidget(glWidget);
}

MainWindow::~MainWindow()
{

}
