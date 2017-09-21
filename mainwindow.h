#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <glwidget.h>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    GLWidget* glWidget;
};

#endif // MAINWINDOW_H
