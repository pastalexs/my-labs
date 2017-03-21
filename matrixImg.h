#ifndef SOBAL_H
#define SOBAL_H

#include <vector>
#include <QPixmap>

using namespace std;

enum class Border
{
    Black, CopyValue, Reflect,Wrapping
};

class matrixImg
{
public:
    matrixImg();
    matrixImg(const QPixmap &pix);
    matrixImg(const vector<double> &vector, int width, int height);

    matrixImg& operator=(matrixImg& matrixImg) = default;

    QImage getImg() const;
    static QImage getImg(const vector<double> &vector, int width, int height);

    matrixImg  twoConvolution(Border border,const double arrayV[], const double arrayG[], int size) const;

    void convolution(Border border,const double array[],int sizeN,int sizeM);
    static matrixImg convolution(Border border,const matrixImg &matrixImg, const double array[], int sizeN, int sizeM);

    static QImage gradient(const matrixImg &matrixX,const matrixImg &imagY);

    matrixImg degradationImg(Border border) const;

    void save(QString name) const;

    const vector<double> &getVector() const;
    int getWidth() const;
    int getHeignt() const;

private:
    int width, height;
    vector<double> vectorImg;

    double getElement(Border border, int column, int row) const;
    static double getElement(Border border,const matrixImg &matrix, int column, int row);

    static double getBorder(Border border,const matrixImg &matrix, int column, int row);

    vector<double> convertToVector(const QImage &image) const;
    vector<double> getGrayVector(const QPixmap &pix) const;

};

#endif // SOBAL_H
