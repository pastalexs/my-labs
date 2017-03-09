#ifndef GAUSPIRAMIDA_H
#define GAUSPIRAMIDA_H

#include "vector"
#include <matrixImg.h>
class GausPiramida
{
public:
    GausPiramida(matrixImg *img);
    static vector<double>* getKernelGauss(double sigma);
};

#endif // GAUSPIRAMIDA_H
