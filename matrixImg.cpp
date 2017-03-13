#include "matrixImg.h"

matrixImg::matrixImg(const QPixmap &pix)
{
    QImage img = pix.toImage();
    this->width = img.width();
    this->height = img.height();
    this->vectorImg= this->convertToVector(img);
}

matrixImg::matrixImg(const vector<double> &vector, int width, int height)
{
    this->width = width;
    this->height = height;
    vectorImg.reserve(vector.size()+vectorImg.size());
    vectorImg.insert(vectorImg.end(), vector.begin(), vector.end());
}
vector<double> matrixImg::getGrayVector(const QPixmap &pix) const {
    vector <double> gray;
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

QImage matrixImg::gradient(const matrixImg &matrixX, const matrixImg &matrixImgY)
{
    const vector<double>& x = matrixX.getVector();
    const vector<double>& y = matrixImgY.getVector();
    vector <double> resultImg;
    for(int i=0;i<matrixX.getWidth();i++)
    {
        for(int j=0;j<matrixX.getHeignt();j++)
        {
            double Gx = x.at(i*matrixX.getHeignt()+j);
            double Gy = y.at(i*matrixX.getHeignt()+j);
            resultImg.push_back(sqrt(Gx*Gx+Gy*Gy));
        }
    }
   return getImg(resultImg,matrixX.getWidth(),matrixX.getHeignt());
}

vector <double> matrixImg::convertToVector(const QImage &image) const {
    vector <double> myVector;
    for(int j=0;j<image.width();j++)
    {
        for(int i=0;i<image.height();i++)
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
    QImage result =QImage(width,height,QImage::Format_RGB32);
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

QImage matrixImg::getImg(const vector<double> &vector, int width, int height){
    double min=*min_element(vector.begin(),vector.end());
    double max=*max_element(vector.begin(),vector.end());
    double range=max - min;
    QImage result = QImage(width,height,QImage::Format_RGB32);
    for(int j=0;j<width;j++)
    {
        for(int i=0;i<height;i++)
        {
            if(range!=0){
                QColor color;
                double G=(vector.at(j*height+i)-min)/range;
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

double matrixImg::getElement(int column,int row) const {

    if(row>=0&&row<(height-1)&&column>=0&&column<width){
        return vectorImg.at(column*height+row);
    }
    else {
        return 0;
    }
}
double matrixImg::getElement(const matrixImg &matrix, int column, int row) {

     const vector<double>& vector = matrix.getVector();

    if(row>=0&&row<matrix.getHeignt()&&column>=0&&column<matrix.getWidth()){
        return vector.at(column*matrix.getHeignt()+row);
    }
    else {
        return 0;
    }
}

void matrixImg::convolution(const double array[], int sizeN, int sizeM)//свертка
{
    //matrixImg result=convolution(matrixImg(vectorImg,width,height),array,sizeN,sizeM);
    //const vector<double>& resultImg = result.getVector();
    vector <double> resultImg;
    for(int m=0;m<width;m++)
    {
        for(int k=0;k<height;k++)
        {
            double resultZnath=0;
            int indexMass=0;
            for(int i=0;i<sizeN;i++){
                double summaString=0;
                for(int j=0;j<sizeM;j++){
                    summaString+=array[indexMass++]*getElement(m+sizeM/2-j,k+sizeN/2-i);
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

matrixImg matrixImg::convolution(const matrixImg &matrix, const double array[], int sizeN, int sizeM)//свертка
{
    vector <double> resultImg;
    for(int m=0;m<matrix.getWidth();m++)
    {
        for(int k=0;k<matrix.getHeignt();k++)
        {
            double resultZnath=0;
            int indexMass=0;
            for(int i=0;i<sizeN;i++){
                double summaString=0;
                for(int j=0;j<sizeM;j++){
                    summaString+=array[indexMass++]*getElement(matrix,m+sizeM/2-j,k+sizeN/2-i);
                }
                resultZnath+=summaString;
            }
            resultImg.push_back(resultZnath);
        }
    }
    return matrixImg(resultImg,matrix.getWidth(),matrix.getHeignt());
}
matrixImg matrixImg::twoConvolution(const double arrayV[], const double arrayG[], int size) const {
    int one=1;

    matrixImg x = convolution(matrixImg(vectorImg,width,height),arrayV,size,one);
    x = convolution(x,arrayG,one,size);

    return x;
}

const vector<double> &matrixImg::getVector() const
{
    return vectorImg;
}

int matrixImg::getWidth() const
{
    return width;
}

int matrixImg::getHeignt() const
{
    return height;
}
