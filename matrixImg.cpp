#include "matrixImg.h"



matrixImg::matrixImg(QPixmap *pix)
{
    QImage img = pix->toImage();
    this->width = img.width();
    this->height = img.height();
    this->vectorImg= this->convertToVector(&img);
}

matrixImg::matrixImg(vector<double> *vector, int width, int height)
{
    this->width = width;
    this->height = height;
    vectorImg.reserve(vector->size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), vector->begin(), vector->end());
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

QImage* matrixImg::gradient(matrixImg *matrixX, matrixImg *matrixImgY)
{
    vector<double>* x = matrixX->getVector();
    vector<double>* y = matrixImgY->getVector();
    vector <double> resultImg;
    for(int i=0;i<matrixX->getWidth();i++)
    {
        for(int j=0;j<matrixX->getHeignt();j++)
        {
            double Gx= x->at(i*matrixX->getHeignt()+j);
            double Gy= y->at(i*matrixX->getHeignt()+j);
            resultImg.push_back(sqrt(Gx*Gx+Gy*Gy));
        }
    }
   return getImg(&resultImg,matrixX->getWidth(),matrixX->getHeignt());
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
vector <double> matrixImg::convertToVector(QImage *image) const {
    vector <double> myVector;
    for(int j=0;j<image->width();j++)
    {
        for(int i=0;i<image->height();i++)
        {
            QColor color= image->pixel(j,i);
            myVector.push_back(color.red()+color.black()+color.green());
        }
    }

    return myVector;
}

QImage *matrixImg::getImg() const{
    double min=*min_element(vectorImg.begin(),vectorImg.end()) ;
    double max=*max_element(vectorImg.begin(),vectorImg.end());
    double range=max - min;
    QImage* result = new QImage(width,height,QImage::Format_RGB32);
    for(int j=0;j<width;j++)
    {
        for(int i=0;i<height;i++)
        {
            if(range!=0){
                QColor color;
                double G=(vectorImg.at(j*height+i)-min)/range;
                G*=255;
                if(G>255||G<0)
                {
                    color.setRgb(255,255,255);
                }
                else
                {
                    color.setRgb(G,G,G);
                }
                result->setPixel(j,i,color.rgb());
            }
            else {
                 QColor color(100,100,100);
                result->setPixel(j,i, color.rgb());
            }
        }
    }
    return result;
}

QImage *matrixImg::getImg(vector<double> *vector,int width,int height){
    double min=*min_element(vector->begin(),vector->end()) ;
    double max=*max_element(vector->begin(),vector->end());
    double range=max - min;
    QImage* result = new QImage(width,height,QImage::Format_RGB32);
    for(int j=0;j<width;j++)
    {
        for(int i=0;i<height;i++)
        {
            if(range!=0){
                QColor color;
                double G=(vector->at(j*height+i)-min)/range;
                G*=255;
                //color.setRgb(G,G,G);
               if(G>255)
                {
                    color.setRgb(255,255,255);
                }
                else
                {
                   color.setRgb(G,G,G);
                }
                result->setPixel(j,i,color.rgb());
            }
            else {
                 QColor color(100,100,100);
                result->setPixel(j,i, color.rgb());
            }
        }
    }
    return result;
}

double matrixImg::getElement(int i, int j, int sizeM, int sizeN,int column,int row) const {
  //  return getElement(&vectorImg, i, j, sizeM, sizeN, column, row);
    if(sizeM==1){
    column+=i-1;
    row+=j;
    }else{
        if(sizeN==1){
            column+=i;
            row+=j-1;
        }
        else{
            column+=i-1;
            row+=j-1;
        }
    }

    if(row>=0&&row<(height-1)&&column>=0&&column<width){
        return vectorImg.at(column*height+row);
    }
    else {
        return 255;
    }
}
double matrixImg::getElement(matrixImg *matrix, int i, int j, int sizeM, int sizeN, int column, int row) {

    vector<double>* vector = matrix->getVector();
    if(sizeM==1){
    column+=i-1;
    row+=j;
    }else{
        if(sizeN==1){
            column+=i;
            row+=j-1;
        }
        else{
            column+=i-1;
            row+=j-1;
        }
    }

    if(row>=0&&row<matrix->getHeignt()&&column>=0&&column<matrix->getWidth()){
        return vector->at(column*matrix->getHeignt()+row);
    }
    else {
        return 255;
    }
}

void matrixImg::convolution(double *mass, int sizeN, int sizeM)//свертка
{
    matrixImg* result=convolution(new matrixImg(&vectorImg,width,height),mass,sizeN,sizeM);
    vector<double>* resultImg = result->getVector();
   /* vector <double> resultImg;
    for(int m=0;m<width;m++)
    {
        for(int k=0;k<height;k++)
        {
            double resultZnath=0;
            int indexMass=0;
            for(int i=0;i<sizeN;i++){
                double summaString=0;
                for(int j=0;j<sizeM;j++){
                    summaString+=mass[indexMass++]*getElement(m,k,sizeM,sizeN,j,i);
                }
                resultZnath+=summaString;
            }
            resultImg.push_back(resultZnath);
        }
    }*/
    vectorImg.clear();
    vectorImg.reserve(resultImg->size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), resultImg->begin(), resultImg->end());
}

matrixImg *matrixImg::convolution(matrixImg *matrix, double *mass, int sizeN, int sizeM)//свертка
{
    vector <double> resultImg;
    for(int m=0;m<matrix->getWidth();m++)
    {
        for(int k=0;k<matrix->getHeignt();k++)
        {
            double resultZnath=0;
            int indexMass=0;
            for(int i=0;i<sizeN;i++){
                double summaString=0;
                for(int j=0;j<sizeM;j++){
                    summaString+=mass[indexMass++]*getElement(matrix,m,k,sizeM,sizeN,j,i);
                }
                resultZnath+=summaString;
            }
            resultImg.push_back(resultZnath);
        }
    }
    return new matrixImg(&resultImg,matrix->getWidth(),matrix->getHeignt());
}
matrixImg *matrixImg:: twoConvolution(double *arrayV, double *arrayG, int size) {
    int one=1;

    matrixImg* x = convolution(new matrixImg(&vectorImg,width,height),arrayV,size,one);
    x = convolution(x,arrayG,one,size);

    return x;
}


vector<double>* matrixImg::getVector()
{
    return &vectorImg;
}

int matrixImg::getWidth()
{
    return width;
}

int matrixImg::getHeignt()
{
    return height;
}
