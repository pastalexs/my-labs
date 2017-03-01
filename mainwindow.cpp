#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QFileDialog>
#include "matrixImg.cpp"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsScene *scene2 = new QGraphicsScene();
    QPixmap pix;
    pix.load("img.png");
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView->resize(pix.size());
    this->lab1(pix);
   //  this->gays(pix);
    myImg=pix;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
   /* QPixmap pix;
    QString imgName = QFileDialog::getOpenFileName(0, "Выбор изображения", "", "*.jpg *.png");
    pix.load(imgName);*/
    QPixmap pix;
    pix.load("img.png");
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(pix);
    this->lab1(pix);
}


void MainWindow::on_pushButton_2_clicked()
{
    this->gauss(myImg);
}

void MainWindow::gauss(QPixmap pix){
    matrixImg *lab1 = new matrixImg(&pix);
    lab1->gauss();
    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage((lab1->getImg())));
}

void MainWindow::lab1(QPixmap pix){
    matrixImg *lab1 = new matrixImg(&pix);
    lab1->sobel();

    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage((lab1->getImg())));

}
