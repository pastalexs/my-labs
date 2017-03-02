#include "matrixImg.h"
#include <math.h>

matrixImg::matrixImg(QPixmap *pix)
{
    QImage img = pix->toImage();
    this->width = img.width();
    this->height = img.height();
    this->vectorImg= this->convertToVector(pix);
}
vector<double> matrixImg::getGrayVector(QPixmap *pix) const {
    vector <double> gray;
    QImage image = pix->toImage();
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

void matrixImg::gradiet(vector<double> *x, vector<double> *y)
{
    vector <double> resultImg;
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            double Gx= x->at(i*height+j);
            double Gy= y->at(i*height+j);
            resultImg.push_back(sqrt(Gx*Gx+Gy*Gy));
        }
    }
    vectorImg.clear();
    vectorImg.reserve(resultImg.size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), resultImg.begin(), resultImg.end());
}
void matrixImg::summa(vector<double> *x, vector<double> *y)
{
    vector <double> resultImg;
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            resultImg.push_back(x->at(i*height+j)+y->at(i*height+j));
        }
    }
    vectorImg.clear();
    vectorImg.reserve(resultImg.size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), resultImg.begin(), resultImg.end());
}
vector <double> matrixImg::convertToVector(QPixmap *pix) const {
    vector <double> myVector;
    QImage image = pix->toImage();
    for(int j=0;j<width;j++)
    {
        for(int i=0;i<height;i++)
        {
            QColor color= image.pixel(j,i);
            myVector.push_back(color.red()+color.black()+color.green());
        }
    }

    return myVector;
}

QImage matrixImg::getImg() const{
    double min=*min_element(vectorImg.begin(),vectorImg.end()) ;
    double max=*max_element(vectorImg.begin(),vectorImg.end());
    double range=max - min;
    QImage result(width,height,QImage::Format_RGB32);
    for(int j=0;j<width;j++)
    {
        for(int i=0;i<height;i++)
        {
            if(range!=0){
                QColor color;
                double G=(vectorImg.at(j*height+i)-min)/range;
                G*=255;
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


vector<double> matrixImg::getMass(int *i, int *j, int *size) const {
    int begin = *size/2;
    int end = begin;
    if(*size%2==0){
        begin--;
    }
    vector <double>  pixel;
    for(int colum=*j-begin;colum<=*j+end;colum++){
        for(int line=*i-begin;line<=*i+end;line++){
            if(line>0&&line<(height-1)&&colum>0&&colum<width){
                pixel.push_back(vectorImg.at(colum*height+line));
            }
            else {
                pixel.push_back(255);
            }
        }
    }
    return pixel;
}
vector<double> matrixImg::getMass2(int *i, int *j, int *sizeM,int *sizeN) const {
    int beginM = *sizeM/2;
    int endM = beginM;
    if(*sizeM%2==0){
        beginM--;
    }
    int beginN = *sizeN/2;
    int endN = beginN;
    if(*sizeN%2==0){
        beginN--;
    }
    vector <double>  pixel;
    for(int colum=*j-beginM;colum<=*j+endM;colum++){
        for(int line=*i-beginN;line<=*i+endN;line++){
            if(line>0&&line<(height-1)&&colum>0&&colum<width){
                pixel.push_back(vectorImg.at(colum*height+line));
            }
            else {
                pixel.push_back(255);
            }
        }
    }
    return pixel;
}
vector<double> matrixImg::getMass2(vector<double> *img,int *i, int *j, int *sizeM,int *sizeN) const {
    int beginM = *sizeM/2;
    int endM = beginM;
    if(*sizeM%2==0){
        beginM--;
    }
    int beginN = *sizeN/2;
    int endN = beginN;
    if(*sizeN%2==0){
        beginN--;
    }
    vector <double>  pixel;
    for(int colum=*j-beginN;colum<=*j+endN;colum++){
        for(int line=*i-beginM;line<=*i+endM;line++){
            if(line>=0&&line<(height-1)&&colum>=0&&colum<width){
                pixel.push_back(img->at(colum*height+line));
            }
            else {
                pixel.push_back(255);
            }
        }
    }
    return pixel;
}
vector<double> matrixImg::getMutlMatrix(double *massVert, double *massGoris, int *size) {

    vector <double> resultImg;
    for(int m=0;m<width;m++)
    {
        for(int k=0;k<height;k++)
        {
            vector <double> pixel = this->getMass(&k,&m,size);
            vector<double> result;
            for(int i=0;i<*size;i++){
                double stringUmn=0; //умноженная строка
                for(int j=0;j<*size;j++){
                    stringUmn+=massVert[j]*pixel.at(i*(*size)+j);
                }
                result.push_back(stringUmn);
            }
            double resultZnath=0;
            for(int i=0;i<*size;i++){
                resultZnath+=massGoris[i]*result.at(i);
            }
             resultImg.push_back(resultZnath);
        }
    }
    return resultImg;

}
void matrixImg::getMutlMatrix2(double *massVert, double *massGoris, int *size) {

    vector <double> resultImg;
    for(int m=0;m<width;m++)
    {
        for(int k=0;k<height;k++)
        {
            vector <double> pixel = this->getMass(&k,&m,size);
            vector<double> result;
            for(int i=0;i<*size;i++){
                double stringUmn=0; //умноженная строка
                for(int j=0;j<*size;j++){
                    stringUmn+=massVert[j]*pixel.at(i*(*size)+j);
                }
                result.push_back(stringUmn);
            }
            double resultZnath=0;
            for(int i=0;i<*size;i++){
                resultZnath+=massGoris[i]*result.at(i);
            }
             resultImg.push_back(resultZnath);
        }
    }
    vectorImg.clear();
    vectorImg.reserve(resultImg.size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), resultImg.begin(), resultImg.end());
}


void matrixImg::convolution(double *mass,int *sizeN, int *sizeM)//свертка
{
    vector <double> resultImg;
    for(int m=0;m<width;m++)
    {
        for(int k=0;k<height;k++)
        {
            vector <double> pixel = this->getMass2(&k,&m,sizeM,sizeN);
            double resultZnath=0;
            int indexMass=0;
            for(int i=0;i<*sizeN;i++){
                double summaString=0;
                for(int j=0;j<*sizeM;j++){
                    summaString+=mass[indexMass++]*pixel.at(i*(*sizeM)+j);
                }
                resultZnath+=summaString;
            }
            resultImg.push_back(resultZnath);
        }
    }
    vectorImg.clear();
    vectorImg.reserve(resultImg.size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), resultImg.begin(), resultImg.end());
}
vector<double> matrixImg::convolution(vector<double> *img,double *mass,int *sizeN, int *sizeM)//свертка
{
    vector <double> resultImg;
    for(int m=0;m<width;m++)
    {
        for(int k=0;k<height;k++)
        {
            vector <double> pixel = this->getMass2(img,&k,&m,sizeM,sizeN);
            double resultZnath=0;
            int indexMass=0;
            for(int i=0;i<*sizeN;i++){
                double summaString=0;
                for(int j=0;j<*sizeM;j++){
                    summaString+=mass[indexMass++]*pixel.at(i*(*sizeM)+j);
                }
                resultZnath+=summaString;
            }
            resultImg.push_back(resultZnath);
        }
    }
    return resultImg;
}
void matrixImg::sobel() {
    double massVert[] ={1,0,-1};
    double massGoris[] = {1,2,1};

    int size1=1;
    int size3=3;

    vector<double> x = convolution(&vectorImg,massVert,&size3,&size1);
    x = convolution(&x,massGoris,&size1,&size3);
    vector<double> y = convolution(&vectorImg,massGoris,&size3,&size1);
    y = convolution(&y, massVert,&size1,&size3);
    gradiet(&x,&y);
}
vector<double> matrixImg::getCoreGauss(double sigma) {
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
    vector<double> resultImg;
    for(int i=0;i<=size;i++){
        resultImg.push_back(result.at(i)/sum);
    }
    return resultImg;
}

void matrixImg::pyramid(int scale)
{
    gauss(scale);
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
    height/=scale;
    vectorImg.clear();
    vectorImg.reserve(result.size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), result.begin(), result.end());
}

void matrixImg::gauss(double sigma) {
    //double mass[] = {0.028087,0.23431,0.475207,0.23431,0.028087};
    vector<double> result=getCoreGauss(sigma);
    double summa=0;
    double *massCore = new double[result.size()];
    for(int i=0;i<result.size();i++){
        summa+=result.at(i);
        massCore[i]=result.at(i);
    }
    int size1=1;
    int size3=result.size();
    convolution(massCore,&size3,&size1);
    convolution(massCore,&size1,&size3);
}
