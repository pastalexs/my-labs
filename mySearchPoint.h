#ifndef SEARCH_H
#define SEARCH_H

#include <matrixImg.h>
#include <gauspiramida.h>

using namespace std;

struct Point{
    int x,y;
    double intens;
    Point(int x,int y,double inte){
        this->x=x;
        this->y=y;
        this->intens=inte;
    }
};

class mySearchPoint
{
public:
    mySearchPoint(matrixImg &img);
    matrixImg harris(Border border,double limit);
    matrixImg moravek(Border border, double limit);
    QImage saveImgAndPoint();
    void adaptiveNonMaximumSuppression(int countPoint);
private:
    vector<Point> vectorPoint;
    matrixImg myImg;
    void searchPoint(Border border, const matrixImg &img, double limit, int windows);
    double distance(int a1, int a2, int b1, int b2);

};

#endif // SEARCH_H
