#ifndef SOBAL_H
#define SOBAL_H
#include <QVector>
#include <QImage>

class Sobal
{
public:
    Sobal(QPixmap pix);
    QVector <double> getGrayVector(QPixmap pix) const;
    QVector<double> getSobal(QVector<double> gray, int *massVert, int *massGoris, int size)const;
    QImage getImg(QVector<double> gray) const;

private:
    int width, height;
    QVector<double> Sobal::getMass9x9(QVector<double> gray, int i, int j) const;
};

#endif // SOBAL_H
