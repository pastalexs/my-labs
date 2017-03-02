#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVector>
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

    void pyramid(QPixmap pix);
};

#endif // MAINWINDOW_H
