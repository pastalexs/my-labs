#include "gauspiramida.h"

GausPiramida::GausPiramida(matrixImg *img)
{
    //level=0;
   // matrix= new matrixImg(img->getVector(),img->getWidth(),img->getHeignt());
    //sigma=1.6;
   // deltaSigma=
    //int scale; //масштаб
    /*gauss(scale);
        vector<double> result;
        int twoScale =2*scale;
        for(int i=0;i<width;i+=scale)
        {
            for(int j=0;j<height;j+=scale)
            {
                double summa=0;
                for(int k=i;k<scale;k++)
                {
                    for(int m=j;m<scale;m++)
                    {
                       summa+= vectorImg.at(k*height+m);
                    }
                }
                result.push_back(summa/twoScale);
            }
        }
        width/=scale;
        height/=scale;*/
}
vector<double>* GausPiramida::getKernelGauss(double sigma) {
    vector<double> result;
    double element=1/(sqrt(2*3.14159265358979323846)*sigma);
    double element2=2*sigma*sigma;
    int partLine = qRound(3 * sigma + 0.5);
    int size = 2 * partLine + 1;
    double sum=0;
    for(int i=0;i<=size;i++){
        result.push_back(element*exp(-pow(partLine-i,2)/element2));
        sum+=result.at(i);
    }
    vector<double>* resultImg= new vector<double>();
    for(int i=0;i<=size;i++){
        resultImg->push_back(result.at(i)/sum);
    }
    return resultImg;
}
