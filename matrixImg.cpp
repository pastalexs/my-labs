#include "sobal.h"


const int massivDlina=3;

Sobal::Sobal(QPixmap pix)
{
    QImage img = pix.toImage();
    this->width = img.width();
    this->height=img.height();
}
QVector <double> Sobal::getGrayVector(QPixmap pix) const {
    QVector <double> gray;
    QImage image = pix.toImage();
    for(int j=0;j<width;j++)
    {
        for(int i=0;i<height;i++)
        {
            QColor color= image.pixel(j,i);
            gray.push_back(0.213*color.red() + 0.715*color.green() + 0.072*color.blue());
        }
    }

    return gray;
}


QImage Sobal::getImg(QVector<double> gray) const{
    double min=*std::min_element(gray.begin(),gray.end()) ;
    double max=*std::max_element(gray.begin(),gray.end());
    double range=max - min;
    QImage result(width,height,QImage::Format_RGB32);
    for(int j=0;j<width;j++)
    {
        for(int i=0;i<height;i++)
        {
            if(range!=0){
                QColor color;
                //double G=((gray.at(i*height+j)-min)/range)*255;
                 double G=gray.at(j*height+i);
                if(G>255)
                {
                    color.setRgb(255,255,255);
                }
                else
                {
                    color.setRgb(G,G,G);
                }
                result.setPixel(j,i,color.rgb());
            }
            else {
                 QColor color(100,100,100);
                result.setPixel(j,i, color.rgb());
            }
        }
    }
    return result;
}

QVector<double> Sobal::getMass9x9(QVector<double> gray, int i, int j) const {
    QVector <double>  pixel;
    for(int colum=j-1;colum<=j+1;colum++){
        for(int line=i-1;line<=i+1;line++){
            if(line>0&&line<(height-1)&&colum>0&&colum<width){
                pixel.push_back(gray.at(colum*height+line));
            }
            else {
                pixel.push_back(255);
            }
        }
    }
    return pixel;
}

QVector <double> Sobal::getSobal(QVector<double> gray,int *massVert,int *massGoris,int size) const{

    QVector <double> resultImg;
    for(int m=0;m<width;m++)
    {
        for(int k=0;k<height;k++)
        {
            QVector <double> pixel = this->getMass9x9(gray,k,m);
            QVector<double> result;
            for(int i=0;i<size;i++){
                double stringUmn=0; //умноженная строка
                for(int j=0;j<size;j++){
                    stringUmn+=massVert[j]*pixel.at(i*massivDlina+j);
                }
                result.push_back(stringUmn);
               // result.push_back(pixel.at(i*massivDlina)+0*pixel.at(i*massivDlina+1)-pixel.at(i*massivDlina+2));
            }
            double resultZnath=0;
            for(int i=0;i<size;i++){
                resultZnath+=massGoris[i]*result.at(i);
            }
             resultImg.push_back(resultZnath);
        }
    }
    return resultImg;
}

