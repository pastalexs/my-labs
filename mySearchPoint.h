#ifndef SEARCH_H
#define SEARCH_H

#include <matrixImg.h>
#include <gauspiramida.h>

using namespace std;

struct Point{
    int x,y;
    Point(int x,int y){
        this->x=x;
        this->y=y;
    }
};

class mySearchPoint
{
public:
    mySearchPoint(matrixImg &img);
    matrixImg harris(Border border,double limit);
    matrixImg moravek(Border border, double limit);
    void saveImgAndPoint();
private:
    vector<Point> vectorPoint;
    matrixImg myImg;
    void searchPoint(Border border,const matrixImg &img, double limit);
};

#endif // SEARCH_H
