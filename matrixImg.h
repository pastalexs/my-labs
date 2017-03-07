#ifndef SOBAL_H
#define SOBAL_H

#include <vector>
#include <QPixmap>

using namespace std;

class matrixImg
{
public:
    matrixImg(QPixmap *pix);
    matrixImg(vector<double> *vector, int width, int height);

    QImage* getImg() const;
    static QImage* getImg(vector<double> *vector, int width, int height);

    matrixImg*  twoConvolution(double *arrayV, double *arrayG, int size);

    void convolution(double *mass,int sizeN,int sizeM);
    static matrixImg* convolution(matrixImg *matrixImg, double *mass, int sizeN, int sizeM);

    static QImage* gradient(matrixImg *matrixX, matrixImg *imagY);

    vector<double>* getVector();
    int getWidth();
    int getHeignt();
private:
    int width, height;
    vector<double> vectorImg;

    vector<double> convolution(vector<double> *img, double *mass, int sizeN, int sizeM);

    double getElement(int i, int j, int sizeM, int sizeN, int column, int row) const;
    static double getElement(matrixImg *matrix, int i, int j, int sizeM, int sizeN, int column, int row);

    vector<double> matrixImg::convertToVector(QImage *image) const;
    vector<double> getGrayVector(QPixmap *pix) const;




    void summa(vector<double> *x, vector<double> *y);

};

#endif // SOBAL_H
