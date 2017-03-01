#include "matrixImg.h"


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
void matrixImg::sobel() {
    double massVert[] ={1,0,-1};
    double massGoris[] = {1,2,1};

    int size = sizeof(massVert)/sizeof(*massVert);

    vector<double> x = getMutlMatrix(massVert,massGoris,&size);
    vector<double> y = getMutlMatrix(massGoris,massVert,&size);
    gradiet(&x,&y);
}

void matrixImg::gauss() {
    double mass[] = {0.028087,0.23431,0.475207,0.23431,0.028087};
    //double mass[] = {1,2,1};
    int size = sizeof(mass)/sizeof(*mass);
    vector<double> x=getMutlMatrix(mass,mass,&size);
    vector<double> y = getMutlMatrix(mass,mass,&size);
    gradiet(&x,&y);
}
