#ifndef GAUSPIRAMIDA_H
#define GAUSPIRAMIDA_H

#include "vector"
#include <matrixImg.h>
struct ElementPiramid{
    matrixImg myImg;
    double sigma;

    ElementPiramid(matrixImg &img, double ost){
        myImg=img;
        sigma=ost;
    }
};

class GausPiramida
{
public:
    GausPiramida(const matrixImg &img, int countOctav, int countLevel);
    static vector<double> getKernelGauss(double sigma);
    void savePiramid() const;
    static matrixImg getGauss(const matrixImg &img,double deltaSigma);

    int getCountOctav(){ return countOctav;}
    int getCountLevel(){ return countLevel;}
    ElementPiramid getElementVector(int octav, int level){
        return myVector.at(countOctav*octav+level);
    }

private:
    int countOctav,countLevel;
    vector<ElementPiramid> myVector;

};

#endif // GAUSPIRAMIDA_H
