#ifndef SOBAL_H
#define SOBAL_H
#include <QVector>
#include <QImage>
#include <vector>

using namespace std;

class matrixImg
{
public:
    matrixImg(QPixmap *pix);
    QImage getImg() const;
    void gauss(double sigma);
    void sobel();
     void pyramid(int scale);
    void convolution(double *mass,int *sizeN,int *sizeM);
    vector<double> convolution(vector<double> *img,double *mass,int *sizeN, int *sizeM);
private:
    int width, height;
    vector<double> vectorImg;

    vector<double> getMass2(int *i, int *j, int *sizeM, int *sizeN) const;
    vector<double> getMass2(vector<double> *img,int *i, int *j, int *sizeM, int *sizeN) const;
    vector<double> convertToVector(QPixmap *pix) const;
    vector<double> getGrayVector(QPixmap *pix) const;
    void gradiet(vector<double> *x, vector<double> *y);

    vector<double> getCoreGauss(double sigma);

   //это старые методы, но пусть будут
    vector<double> getMass(int *i, int *j, int *size) const;
    vector<double> getMutlMatrix(double *massVert, double *massGoris, int *size);
    void getMutlMatrix2(double *massVert, double *massGoris, int *size);
    void summa(vector<double> *x, vector<double> *y);

};

#endif // SOBAL_H
