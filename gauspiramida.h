#ifndef GAUSPIRAMIDA_H
#define GAUSPIRAMIDA_H

#include "vector"
#include <matrixImg.h>
struct ElementLevelsPiramid{
    matrixImg myImg;
    double zeroSigma;
    double ostav;

    ElementLevelsPiramid(matrixImg &img, double zeroS, double ost){
        myImg=img;
        zeroSigma=zeroS;
        ostav=ost;
    }
};

class GausPiramida
{
public:
    GausPiramida(const matrixImg &img, int countlevel, int countOst);
    static vector<double> getKernelGauss(double sigma);
    void savePiramid() const;

private:
    matrixImg getGauss(const matrixImg &img,double deltaSigma) const;
    vector<ElementLevelsPiramid> myVector;

};

#endif // GAUSPIRAMIDA_H
