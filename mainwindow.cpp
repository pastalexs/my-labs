#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QFileDialog>
#include "sobal.cpp"
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QPixmap pix;
    QString imgName = QFileDialog::getOpenFileName(0, "Выбор изображения", "", "*.jpg *.png");
    pix.load(imgName);
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(pix);
    ui->graphicsView->resize(pix.size());
    this->lab1(pix);
}


void MainWindow::on_pushButton_2_clicked()
{

}
void MainWindow::lab1(QPixmap pix){
    Sobal *lab1 = new Sobal(pix);
    int massVert[] ={1,0,-1};
    int massGoris[] = {1,2,1};
    QVector<double> vectorImg = lab1->getSobal(lab1->getGrayVector(pix),massVert,massGoris,3);
    vectorImg = lab1->getSobal(vectorImg,massGoris,massVert,3);

    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((new QPixmap())->fromImage(lab1->getImg(vectorImg)));
    ui->graphicsView_2->resize(pix.size());

}
