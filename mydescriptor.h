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

class myDescriptor
{
private:
    vector<discript> myVectorDescriptor;
    int rSizeX;
    int rSizeY;
    int histogramSize;
    matrixImg sobelX;
    matrixImg sobelY;
    Border border;
    int partsCount;

public:
    myDescriptor(matrixImg &sobelX, matrixImg &sobelY, Border border, int rSizeX, int rSizeY, int HistogramSize, int partsCount);
    vector<discript> calculationDescriptor(const vector<Point> &points);
    vector<discript> getVectorDiscriptor(){
        return myVectorDescriptor;
    }
    static double distance(myDescriptor &descr1, myDescriptor &descr2,int i,int j);

};

#endif // MYDESCRIPTOR_H
