#include "sobal.h"

Sobal::Sobal(QPixmap pix)
{
    this->pixMap = pix;
    this->img= pix.toImage();
    this->width = img.width();
    height=img.height();
}
QVector <double> Sobal::getGrayVector() {
    QVector <double> gray;
    for(int j=0;j<height;j++)
    {
        for(int i=0;i<width;i++)
        {
            QColor color= img.pixel(i,j);
            gray.push_back(0.213*color.red() + 0.715*color.green() + 0.072*color.blue());
        }
    }

    return gray;
}
/*
QVector <double> Sobal::getContyr(QPixmap pixMap)
{
   // QImage result(img.width(),img.height(),img.format());
    QVector <double> res;
   // double min=0,max=0;
    for(int j=0;j<height;j++)
    {
        for(int i=0;i<width;i++)
        {
            QVector <QColor>  pixel=this->getMass9x9(img,i,j);
            QVector <double> gray;
            for(int p=0; p<pixel.size();p++)
            {
                gray.push_back(0.213*pixel.at(p).red() + 0.715*pixel.at(p).green() + 0.072*pixel.at(p).blue());
            }
           // double G=sqrt(pow(this->getSobalX(gray),2)+pow(this->getSobalY(gray),2));
           // res.push_back(G);
            pixel.clear();
            gray.clear();
        }
    }
    return res;
    /*double randge = max-min;
    if(randge==0){
        randge=1;
    }
    for(int j=1,k=0;j<height;j++)
    {
        for(int i=1;i<width;i++)
        {

            if(k<=res.size()){
                 QColor it;
                double norm=((res.at(k)-min)/randge)*255;
                k++;
                if(norm>255)
                {
                    it.setRgb(255,255,255);
                }
                else
                {
                    it.setRgb(norm,norm,norm);
                }
                result.setPixel(i,j,it.rgb());
            }
        }
    }
    QFile file("result.jpg");
    file.open(QIODevice::WriteOnly);
    (new QPixmap())->fromImage(result).save(&file, "JPG");
    return result;
}*/
QVector<double> Sobal::getMass9x9(QVector<double> gray, int i, int j){
    QVector <double>  pixel;
    for(int colum=j-1;colum<=j+1;colum++){
        for(int line=i-1;line<=i+1;line++){
            if(line>0&&line<height||colum>0&&colum<width){
                pixel.push_back(gray.at(i*massivDlina+j));
            }
            else {
                pixel.push_back(255);
            }
        }
    }
    return pixel;
}
int massXVert[] ={1,0,-1};
int massXGoris[] = {1,2,1};
QVector <double> Sobal::getSobalX(QVector<double> gray){
    QVector <double> resultImg;
    for(int m=0;m<height;m++)
    {
        for(int k=0;k<width;k++)
        {
            QVector <double>  pixel=this->getMass9x9(gray,k,m);
            QVector<double> result;
            for(int i=0;i<sizeof(massXGoris);i++){
                double stringUmn=0; //умноженная строка
                for(int j=0;j<sizeof(massXVert);j++){
                    stringUmn+=massXVert[j]*pixel.at(i*massivDlina+j);
                }
                result.push_back(stringUmn);
            }
            double resultZnath=0;
            for(int i=0;i<sizeof(massXGoris);i++){
                resultZnath+=massXGoris[i]*result.at(i);
            }
             resultImg.push_back(resultZnath);
        }
    }
    return resultImg;
}
double Sobal::getSobalY(QVector<double> gray){

    QVector<double> result;
    for(int i=0;i<3;i++){
        result.push_back(gray.at(i*3)+2*gray.at(i*3+1)+gray.at(i*3+2));
    }
    return result.at(0)+0*result.at(1)-result.at(2);
}
