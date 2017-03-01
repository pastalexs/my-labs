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
    vector<double> getMutlMatrix(double *massVert, double *massGoris, int *size);
    QImage getImg() const;
    void gauss();
    void sobel();
private:
    int width, height;
    vector<double> vectorImg;
    vector<double> getMass(int *i, int *j, int *size) const;
    vector <double> convertToVector(QPixmap *pix) const;
    vector <double> getGrayVector(QPixmap *pix) const;
    void gradiet(vector<double> *x, vector<double> *y);
};

#endif // SOBAL_H
