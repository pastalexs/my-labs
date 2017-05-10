#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include "gauspiramida.h"
#include "matrixImg.h"
#include "mySearchPoint.h"


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
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    QPixmap myImg;
    Ui::MainWindow *ui;
    void lab1(QPixmap pix);
    void gauss(QPixmap pix);

    void lab2(QPixmap pix);
    void lab3(QPixmap pix);
    void lab4(QPixmap pix);
};

#endif // MAINWINDOW_H
