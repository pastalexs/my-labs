#ifndef GAUSPIRAMIDA_H
#define GAUSPIRAMIDA_H

#include "vector"
#include <matrixImg.h>
struct ElementPiramid{
    matrixImg myImg;
    double zeroSigma;
    double sigma;

    ElementPiramid(matrixImg &img, double zeroS, double ost){
        myImg=img;
        zeroSigma=zeroS;
        sigma=ost;
    }
};

class GausPiramida
{
public:
    GausPiramida(const matrixImg &img, int countOctav, int countLevel);
    static vector<double> getKernelGauss(double sigma);
    void savePiramid() const;

private:
    matrixImg getGauss(const matrixImg &img,double deltaSigma) const;
    vector<ElementPiramid> myVector;

};

#endif // GAUSPIRAMIDA_H
