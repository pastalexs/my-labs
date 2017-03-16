#ifndef SOBAL_H
#define SOBAL_H

#include <vector>
#include <QPixmap>

using namespace std;

class matrixImg
{
public:
    matrixImg();
    matrixImg(const QPixmap &pix);
    matrixImg(const vector<double> &vector, int width, int height);

    matrixImg& operator=(matrixImg& matrixImg) = default;

    QImage getImg() const;
    static QImage getImg(const vector<double> &vector, int width, int height);

    matrixImg  twoConvolution(const double arrayV[], const double arrayG[], int size) const;

    void convolution(const double array[],int sizeN,int sizeM);
    static matrixImg convolution(const matrixImg &matrixImg, const double array[], int sizeN, int sizeM);

    static QImage gradient(const matrixImg &matrixX,const matrixImg &imagY);

    matrixImg degradationImg() const;

    void save(QString name) const;

    const vector<double> &getVector() const;
    int getWidth() const;
    int getHeignt() const;

private:
    int width, height;
    vector<double> vectorImg;

    double getElement(int column, int row) const;
    static double getElement(const matrixImg &matrix, int column, int row);

    vector<double> convertToVector(const QImage &image) const;
    vector<double> getGrayVector(const QPixmap &pix) const;

};

#endif // SOBAL_H
