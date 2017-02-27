#ifndef SOBAL_H
#define SOBAL_H
#include <QVector>
#include <QImage>

class Sobal
{
public:
    Sobal(QPixmap pix);
    QVector<double> getContyr(QPixmap pixMap);
     QVector <double> getGrayVector();
     QVector<double> getSobalX(QVector<double> gray);
     double getSobalY(QVector<double> gray);
     QImage normalis();
private:
    int width, height;
    const int massivDlina=3;
    QPixmap pixMap;
    QImage img;
    QVector<double> getMass9x9(QVector<double> gray, int i, int j);

};

#endif // SOBAL_H
