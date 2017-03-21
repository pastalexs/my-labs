#include "gauspiramida.h"


GausPiramida::GausPiramida(const matrixImg &img, int countOctav, int countLevel)
{
    const double intSigma=0.5;
    const double zeroSigma=1.6;
    const double k = pow(2,(1./countOctav));
    double deltaSigma = sqrt(pow(zeroSigma,2) - pow(intSigma,2));

    matrixImg newImg = getGauss(img,deltaSigma);

    ElementPiramid element = ElementPiramid(newImg,zeroSigma,zeroSigma);

    for(int i=0;i<countOctav;i++){
        for(int j=0;j<countLevel;j++){
            myVector.push_back(element);
            double sigma=element.zeroSigma*k;

            double nextSigma=element.sigma*k;
            deltaSigma = sqrt(pow(nextSigma,2) - pow(element.sigma,2));
            newImg = getGauss(newImg,deltaSigma);

            if(j==countLevel-1){
                newImg = newImg.degradationImg(Border::Wrapping);
                element = ElementPiramid(newImg,sigma/2,nextSigma/2);
            }
            else{
                element = ElementPiramid(newImg,sigma,nextSigma);
            }
        }

    }
}
vector<double> GausPiramida::getKernelGauss(double sigma)  {
    vector<double> result;
    double element=1/(sqrt(2*3.14159265358979323846)*sigma);
    double element2=2*sigma*sigma;
    int partLine = qRound(3 * sigma);
    int size = 2 * partLine;
    double sum=0;
    for(int i=0;i<=size;i++){
        result.push_back(element*exp(-pow(partLine-i,2)/element2));
        sum+=result.at(i);
    }
    vector<double> resultImg = vector<double>();
    for(int i=0;i<=size;i++){
        resultImg.push_back(result.at(i)/sum);
    }
    return resultImg;
}

void GausPiramida::savePiramid() const
{
    for(int i=0;i<myVector.size();i++){
        ElementPiramid element = myVector.at(i);
        if(element.myImg.getHeignt()!=0&&element.myImg.getWidth()!=0){
            element.myImg.save("C:\\AGTU\\img\\"+QString::number(i)+" "+QString::number(element.zeroSigma)+" "+QString::number(element.sigma));
        }
    }
}

matrixImg GausPiramida::getGauss(const matrixImg &img,double deltaSigma) const
{
    vector<double> result=GausPiramida::getKernelGauss(deltaSigma);
    double summa=0;
    double *massCore = new double[result.size()];
    for(int i=0;i<result.size();i++){
        summa+=result.at(i);
        massCore[i]=result.at(i);
    }
    int size3=result.size();
    return img.twoConvolution(Border::Wrapping,massCore,massCore,size3);
}
