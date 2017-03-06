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
    matrixImg(vector<double> *vector, int width, int height);

    QImage getImg() const;
    QImage getImg(vector<double> *vector, int width, int height) const;

    matrixImg sobelOnCoordinate(double *arrayV, double *arrayG, int size);

    void convolution(double *mass,int sizeN,int sizeM);
    vector<double> convolution(vector<double> *img, double *mass, int sizeN, int sizeM);

    QImage gradient(matrixImg *matrixX, matrixImg *imagY);

    vector<double> getKernelGauss(double sigma);

    vector<double> getVector();
    int getWidth();
    int getHeignt();
private:
    int width, height;
    vector<double> vectorImg;

    double getElement(int i, int j, int sizeM, int sizeN, int column, int row) const;
    double getElement(vector<double> *img, int i, int j, int sizeM, int sizeN, int column, int row) const;

    vector<double> matrixImg::convertToVector(QImage *image) const;
    vector<double> getGrayVector(QPixmap *pix) const;




    void summa(vector<double> *x, vector<double> *y);

};

#endif // SOBAL_H
