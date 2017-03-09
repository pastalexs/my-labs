#ifndef SOBAL_H
#define SOBAL_H

#include <vector>
#include <QPixmap>

using namespace std;

class matrixImg
{
public:
    matrixImg(const QPixmap *pix);
    matrixImg(const vector<double> *vector, int width, int height);

    QImage* getImg() const;
    static QImage* getImg(const vector<double> *vector, int width, int height);

    matrixImg*  twoConvolution(const double *arrayV,const double *arrayG, int size) const;

    void convolution(const double *mass,int sizeN,int sizeM);
    static matrixImg* convolution(matrixImg *matrixImg, const double *mass, int sizeN, int sizeM);

    static QImage* gradient(matrixImg *matrixX, matrixImg *imagY);

    vector<double>* getVector();
    int getWidth() const;
    int getHeignt() const;
private:
    int width, height;
    vector<double> vectorImg;

    vector<double> convolution(vector<double> *img, double *mass, int sizeN, int sizeM) const;

    double getElement(int column, int row) const;
    static double getElement(matrixImg *matrix, int column, int row);

    vector<double> convertToVector(QImage *image) const;
    vector<double> getGrayVector(QPixmap *pix) const;




    void summa(vector<double> *x, vector<double> *y);

};

#endif // SOBAL_H
