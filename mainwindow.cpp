#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QFileDialog>
#include "matrixImg.cpp"
#include "mySearchPoint.cpp"
#include "descriptorfinder.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsScene *scene2 = new QGraphicsScene();
    QPixmap pix;
    pix.load("imgL90.png");
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView->resize(pix.size());
    // this->lab1(pix);
    //this->gauss(pix);
    //this->lab2(pix);
    // this->lab3(pix);
    this->lab4(pix);
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
    int size3=(int)result.size();
    matrixImg gauss = lab1-> twoConvolution(Border::Reflect,massCore,massCore,size3);
    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage(gauss.getImg()));
}

void MainWindow::lab2(QPixmap pix){
    matrixImg lab2 = matrixImg(pix);
    GausPiramida piramid = GausPiramida(lab2,5,5);
    piramid.savePiramid();
}

void MainWindow::lab3(QPixmap pix)
{
    matrixImg lab3 = matrixImg(pix);
    mySearchPoint pointSearch = mySearchPoint(lab3);
    matrixImg matr = pointSearch.harris(Border::Wrapping,3.81861e+015);
    pointSearch.adaptiveNonMaximumSuppression(500);
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap((QPixmap()).fromImage(pointSearch.saveImgAndPoint()));
    mySearchPoint pointSearchMoravek = mySearchPoint(lab3);
    pointSearchMoravek.moravek(Border::Wrapping,15000);
    pointSearchMoravek.adaptiveNonMaximumSuppression(500);
    //pointSearch.saveImgAndPoint();
    // pointSearch.adaptiveNonMaximumSuppression(500);
    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage(pointSearchMoravek.saveImgAndPoint()));
    pointSearch.saveImgAndPoint();
}

void MainWindow::lab4(QPixmap pix)
{
    matrixImg lab = matrixImg(pix);
    mySearchPoint pointSearch = mySearchPoint(lab);
    pointSearch.harris(Border::Wrapping,3.81861e+015);
    pointSearch.adaptiveNonMaximumSuppression(100);

    QImage qImage1 = pix.toImage();
    vector<Point> firstVector = pointSearch.getVector();

    QPixmap pix2;
    pix2.load("imgL.png");
    matrixImg lab2 = matrixImg(pix2);
    mySearchPoint pointSearch2 = mySearchPoint(lab2);
    pointSearch2.harris(Border::Wrapping,3.81861e+015);
    pointSearch2.adaptiveNonMaximumSuppression(100);

    vector<Point> secondVector = pointSearch2.getVector();

    QImage qImage2 = pix2.toImage();
    QImage result = QImage(qImage1.width() + qImage2.width(),max(qImage1.height(), qImage2.height()),QImage::Format_RGB32);
    for (int i = 0; i < qImage1.height(); i++) {
        for (int j = 0; j < qImage1.width(); j++) {
            result.setPixel(j, i, qImage1.pixel(j, i));
        }
    }
    for (int i = 0, shift = qImage1.width(); i < qImage2.height(); i++) {
        for (int j = 0; j < qImage2.width(); j++) {
            result.setPixel(j + shift, i, qImage2.pixel(j, i));
        }
    }

    QPainter painter(&result);
    QPen pen = QPen();
    pen.setWidth(1);
    painter.setPen(pen);
    int color = 0;
    const double T = 0.8;
    QColor red = QColor(255, 0, 0),
            green = QColor(0, 255, 0),
            blue = QColor(0, 0, 255),
            black = QColor(0, 0, 0);
    for (int i = 0; i < firstVector.size(); i++) {
        double minDistance = numeric_limits<double>::max(), secondMinDistance = minDistance;
        int indexMin = -1;
        for (int j = 0; j < secondVector.size(); j++) {
            double distance = pointSearch2.distance(firstVector.at(i).x,secondVector.at(j).x,firstVector.at(i).y,secondVector.at(j).y);
            if (minDistance > distance) {
                secondMinDistance = minDistance;
                minDistance = distance;
                indexMin = j;
                continue;
            }
            if (secondMinDistance > distance) {
                secondMinDistance = distance;
            }
        }
        const double rate = minDistance / secondMinDistance;
        if (rate <= T) {
            switch (color) {
            case 0:
                pen.setColor(red);
                color++;
                break;
            case 1:
                pen.setColor(blue);
                color++;
                break;
            case 2:
                pen.setColor(green);
                color++;
                break;
            case 3:
                pen.setColor(black);
                color = 0;
                break;
            }
            painter.setPen(pen);
            const int x1 = firstVector.at(i).x,
                    y1 = firstVector.at(i).y,
                    x2 = secondVector.at(indexMin).x + qImage1.width(),
                    y2 = secondVector.at(indexMin).y;
            painter.drawLine(x1, y1, x2, y2);
        }
    }
    QDir outputDir ("C:/AGTU/img/dist");
    result.save(outputDir.absoluteFilePath("img.jpg"), "jpg");
    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage(result));
}


void MainWindow::lab1(QPixmap pix){
    matrixImg *lab1 = new matrixImg(pix);

    double massVert[] ={1,0,-1};
    double massGoris[] = {1,2,1};

    int size3=3;

    matrixImg x = lab1-> twoConvolution(Border::Black,massVert,massGoris,size3);

    matrixImg y = lab1-> twoConvolution(Border::Black,massGoris,massVert,size3);

    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage(lab1->gradient(x,y)));
}


