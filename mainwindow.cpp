#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QFileDialog>
#include "matrixImg.cpp"
#include "gauspiramida.h"


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
    // this->lab1(pix);
    this->gauss(pix);
    this->lab2(pix);
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
    /* QPixmap pix;
    pix.load("img.png");
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(pix);*/
    this->lab1(myImg);
}


void MainWindow::on_pushButton_2_clicked()
{
    this->gauss(myImg);
}

void MainWindow::gauss(QPixmap pix){
    matrixImg *lab1 = new matrixImg(pix);
    double sigma=4;

    vector<double> result=GausPiramida::getKernelGauss(sigma);
    double summa=0;
    double *massCore = new double[result.size()];
    for(int i=0;i<result.size();i++){
        summa+=result.at(i);
        massCore[i]=result.at(i);
    }
    int size3=result.size();
    matrixImg gauss = lab1-> twoConvolution(massCore,massCore,size3);
    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage(gauss.getImg()));
}

void MainWindow::lab2(QPixmap pix){
    matrixImg lab2 = matrixImg(pix);
    GausPiramida piramid = GausPiramida(lab2,9,9);
    piramid.savePiramid();
}

void MainWindow::lab1(QPixmap pix){
    matrixImg *lab1 = new matrixImg(pix);

    double massVert[] ={1,0,-1};
    double massGoris[] = {1,2,1};

    int size3=3;

    matrixImg x = lab1-> twoConvolution(massVert,massGoris,size3);

    matrixImg y = lab1-> twoConvolution(massGoris,massVert,size3);

    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage(lab1->gradient(x,y)));
}


