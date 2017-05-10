#include "mydescriptor.h"

myDescriptor::myDescriptor(matrixImg &x, matrixImg &y, Border border, int rSizeX, int rSizeY, int HistogramSize, int partsCount)
{
    this->sobelX=x;
    this->sobelY=y;
    this->border=border;
    this->rSizeX=rSizeX;
    this->rSizeY=rSizeY;
    this->histogramSize=HistogramSize;
    this->partsCount=partsCount;
}

vector<discript> myDescriptor::calculationDescriptor(const vector<Point> &points)
{
    vector<discript> result;
    //const int descriptorSize = rSizeX * rSizeY * partsCount;
    vector<double> content;
    const int size = rSizeX * histogramSize;
    foreach (Point point, points) {
        content.clear();
        for(int x=-size; x<size; x++){
            for(int y=-size; y<size; y++){

                const int pointX = point.x + x;
                const int pointY = point.y + y;
                const double dx = sobelX.getElement(border,pointX, pointY);
                const double dy = sobelY.getElement(border,pointX, pointY);
                content.push_back(atan2(dy, dx));
            }
        }
        result.push_back(discript(point.x,point.y,content));
    }
    myVectorDescriptor.clear();
    myVectorDescriptor.reserve(result.size()+myVectorDescriptor.size());
    myVectorDescriptor.insert(myVectorDescriptor.end(), result.begin(), result.end());
    return result;
}

double myDescriptor::distance(myDescriptor &descr1, myDescriptor &descr2, int i, int j)
{
     auto vector1= descr1.getVectorDiscriptor();
     auto vector2= descr2.getVectorDiscriptor();
    double distance=0;
    for(int n=0;n<vector1.size();n++){
        double dist = vector1.at(i).vectorDescript.at(n) - vector2.at(j).vectorDescript.at(n);
        distance += pow(dist,2);
    }
    return sqrt(distance);
}
