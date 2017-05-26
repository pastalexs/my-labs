#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QFileDialog>
#include "matrixImg.cpp"
#include "mySearchPoint.cpp"
#include "mydescriptor.cpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsScene *scene2 = new QGraphicsScene();
    QPixmap pix;
    pix.load("imgL.png");
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView->resize(pix.size());
    // this->lab1(pix);
    //this->gauss(pix);
    //this->lab2(pix);
     //this->lab3(pix);
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
    matrixImg matr = pointSearch.harris(Border::Wrapping,/*3.81861e+015*/2.90925e+007);
    pointSearch.adaptiveNonMaximumSuppression(500);
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap((QPixmap()).fromImage(pointSearch.saveImgAndPoint()));
   /* mySearchPoint pointSearchMoravek = mySearchPoint(lab3);
    pointSearchMoravek.moravek(Border::Wrapping,15000);
    pointSearchMoravek.adaptiveNonMaximumSuppression(500);
    //pointSearch.saveImgAndPoint();
    // pointSearch.adaptiveNonMaximumSuppression(500);
    ui->graphicsView_2->scene()->clear();
    ui->graphicsView_2->scene()->addPixmap((QPixmap()).fromImage(pointSearchMoravek.saveImgAndPoint()));*/
    pointSearch.saveImgAndPoint();
}

void MainWindow::lab4(QPixmap pix)
{
    matrixImg lab = matrixImg(pix);
    mySearchPoint pointSearch = mySearchPoint(lab);
    pointSearch.harris(Border::Wrapping,2.90925e+007);
    pointSearch.adaptiveNonMaximumSuppression(500);

    QImage qImage1 = pix.toImage();
    vector<Point> firstVector = pointSearch.getVector();


    double massVert[] ={1,0,-1};
    double massGoris[] = {1,2,1};

    int size3=3;

    matrixImg sobelX = lab.twoConvolution(Border::Black,massVert,massGoris,size3);
    matrixImg sobelY = lab.twoConvolution(Border::Black,massGoris,massVert,size3);
    matrixImg gauss = GausPiramida::getGauss(lab,2);
    myDescriptor decript1= myDescriptor(gauss,sobelX,sobelY,Border::Black,1,1,16,36);
    decript1.calculationDescriptor(firstVector,0);

    QPixmap pix2;
    pix2.load("imgL90.png");
    matrixImg lab2 = matrixImg(pix2);
    mySearchPoint pointSearch2 = mySearchPoint(lab2);
    pointSearch2.harris(Border::Wrapping,2.90925e+007);
    pointSearch2.adaptiveNonMaximumSuppression(500);

    vector<Point> secondVector = pointSearch2.getVector();


    matrixImg sobelX2 = lab2.twoConvolution(Border::Black,massVert,massGoris,size3);
    matrixImg sobelY2 = lab2.twoConvolution(Border::Black,massGoris,massVert,size3);
    matrixImg gauss2 = GausPiramida::getGauss(lab2,2);
    myDescriptor decript2= myDescriptor(gauss2,sobelX2,sobelY2,Border::Black,1,1,16,36);
    decript2.calculationDescriptor(secondVector,0);


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
    QColor black = QColor(0, 0, 0),
            green = QColor(0, 255, 0),
            red = QColor(255, 0, 0),
            blue = QColor(0, 0, 255);
    double best=1;
    vector<ProximateDescriptors> overlap =decript1.findOverlap(decript1,decript2,best);
    for(int i=0;i<overlap.size();i++){
        switch (color) {
        case 0:
            pen.setColor(black);
            color++;
            break;
        case 1:
            pen.setColor(green);
            color++;
            break;
        case 2:
            pen.setColor(blue);
            color++;
            break;
        case 3:
            pen.setColor(red);
            color = 0;
            break;
        }
        painter.setPen(qRgb(0,255,0));
        //painter.drawEllipse(overlap.at(i).fY, overlap.at(i).fX, 10,10);
        //painter.drawEllipse(overlap.at(i).sY + pix.width(), overlap.at(i).sX, 10,10);

        painter.setPen(pen);
        painter.drawLine(overlap.at(i).fY, overlap.at(i).fX, overlap.at(i).sY + pix.width(), overlap.at(i).sX);
    }
   /* for (int i = 0; i < firstVector.size(); i++) {
        double minDist = numeric_limits<double>::max();
        double secMinDist = minDist;
        int indexMin = -1;
        for (int j = 0; j < secondVector.size(); j++) {
            double distance = myDescriptor::distance(decript1,decript2,i,j);
            if (minDist > distance) {
                secMinDist = minDist;
                minDist = distance;
                indexMin = j;
                continue;
            }
            if (secMinDist > distance) {
                secMinDist = distance;
            }
        }
        const double rate = minDist / secMinDist;
        if (rate <= T) {
            switch (color) {
            case 0:
                pen.setColor(black);
                color++;
                break;
            case 1:
                pen.setColor(green);
                color++;
                break;
            case 2:
                pen.setColor(blue);
                color++;
                break;
            case 3:
                pen.setColor(red);
                color = 0;
                break;
            }
            painter.setPen(pen);
            const int x1 = firstVector.at(i).x,
                    y1 = firstVector.at(i).y,
                    x2 = secondVector.at(indexMin).x + qImage1.width(),
                    y2 = secondVector.at(indexMin).y;
            painter.drawLine(x1, y1, x2, y2);
        }*/
   // }
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


