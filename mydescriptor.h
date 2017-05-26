#ifndef MYDESCRIPTOR_H
#define MYDESCRIPTOR_H

#include <matrixImg.h>
#include <gauspiramida.h>
#include <mySearchPoint.h>
#define M_PI 3.14

struct discript{
    int x,y;
    vector<double> vectorDescript;
    discript(int x,int y,vector<double> vector){
        this->x=x;
        this->y=y;
        this->vectorDescript=vector;
    }
};

struct ProximateDescriptors{
    int fX;
    int fY;
    int sX;
    int sY;
    ProximateDescriptors(int fX,int fY,int sX,int sY){
        this->fX=fX;
        this->fY=fY;
        this->sX=sX;
        this->sY=sY;
    }
};

class myDescriptor
{
private:
    vector<discript> myVectorDescriptor;
    int rSizeX;
    int rSizeY;
    int histogramSize;
    matrixImg sobelX;
    matrixImg sobelY;
    matrixImg gauss;
    Border border;
    int partsCount;

public:
    myDescriptor(matrixImg &gauss,matrixImg &sobelX, matrixImg &sobelY, Border border, int rSizeX, int rSizeY, int HistogramSize, int partsCount);
    vector<discript> calculationDescriptor(const vector<Point> &points, double aroundAngle);

    vector<discript> getVectorDiscriptor(){
        return myVectorDescriptor;
    }
    static double distance(myDescriptor &descr1, myDescriptor &descr2,int i,int j);
    discript normal(discript &descr);
    vector<double> calculateDistance(const myDescriptor &f, const myDescriptor &s);
    vector<ProximateDescriptors> findOverlap(const myDescriptor &firstD, const myDescriptor &secondD, double &best);
    discript calculationDescriptor2(const vector<double> &cont, const Point &point, const int index);
};

#endif // MYDESCRIPTOR_H
