#include "mySearchPoint.h"
#include <iostream>

using namespace std;

mySearchPoint::mySearchPoint(matrixImg &img)
{
    this->myImg=img;
}

matrixImg mySearchPoint::harris(Border border, double limit)
{
    matrixImg gauss = GausPiramida::getGauss(myImg,1);
    const int windows = 5;

    //Sobel
    double massVert[] ={1,0,-1};
    double massGoris[] = {1,2,1};

    const int size3=3;

    matrixImg sobX = myImg.twoConvolution(border,massVert,massGoris,size3);

    matrixImg sobY = myImg.twoConvolution(border,massGoris,massVert,size3);
    //
    vector<double> result;
    double A,B,C;
    const double k=0.06;
    for(int x=0;x<myImg.getHeignt();x++){
        for(int y=0;y<myImg.getWidth();y++){
            A=B=C=0;
            for(int u=-windows;u<=windows;u++)
            {
                for(int v=-windows;v<=windows;v++)
                {
                    double w = gauss.getElement(border,v+windows,u+windows);
                    double Ix = sobX.getElement(border,y+v,x+u);
                    double Iy = sobY.getElement(border,y+v,x+u);
                    A += w * pow(Ix,2);
                    B += w * Ix * Iy;
                    C += w * pow(Iy,2);
                }
            }
            double f = A*C - pow(B,2) + k*pow(A+C,2);
            result.push_back(f);
        }
    }
    searchPoint(border,matrixImg(result,myImg.getWidth(),myImg.getHeignt()),limit);
    return matrixImg(result,myImg.getWidth(),myImg.getHeignt());
}

matrixImg mySearchPoint::moravek(Border border, double limit)
{
    vector<double> vectorError;
    double window = 5;
    vector<double> result;
    for(int x=0; x<myImg.getHeignt();x++)
    {
        for(int y=0;y<myImg.getWidth();y++)
        {
            vectorError.clear();
            for(int dx=-1; dx<=1; dx++)
            {
                for(int dy=-1; dy<=1; dy++){
                    if(dx!=0 && dy!=0){
                        double currwntError =0;
                        for(int u = -window; u<=window; u++){
                            for(int v=-window; v<=window;v++){
                                double inten = myImg.getElement(border,x+u,y+v);
                                double shiftInten = myImg.getElement(border,x+u+dx,y+v+dy);
                                double error = inten-shiftInten;
                                currwntError += pow(error,2);
                            }
                        }
                        vectorError.push_back(currwntError);
                    }
                }
            }
            double minError = *min_element(vectorError.begin(),vectorError.end());
            result.push_back(minError);
        }
    }
    searchPoint(border,matrixImg(result,myImg.getWidth(),myImg.getHeignt()),limit);
    return matrixImg(result,myImg.getWidth(),myImg.getHeignt());
}

void mySearchPoint::searchPoint(Border border, const matrixImg &img, double limit)
{
    const int window = 2;
    vectorPoint.clear();
    for(int x=0;x<img.getHeignt();x++){
        for(int y=0;y<img.getWidth();y++){
            double element = img.getElement(border,y,x);
            if(element > limit){
                bool isMaxLoc = true;
                for(int u=-window;u<window;u++){
                    for(int v=-window;v<window;v++){
                        double shiftElement = img.getElement(border,y+v,x+u);
                        if(shiftElement >= element){
                            isMaxLoc = false;
                            break;
                        }
                    }
                    if(isMaxLoc==false){
                        break;
                    }
                }
                if(isMaxLoc){
                    vectorPoint.emplace_back(Point(x,y));
                }
            }
        }
    }
    cout<<"size: "<<vectorPoint.size()<<endl;
}

void mySearchPoint::saveImgAndPoint()
{
    QImage img = myImg.getImg();
    QPainter painter(&img);
    painter.setPen(qRgb(255,0,0));
    for(Point point : vectorPoint) {
        painter.drawEllipse(point.y,point.x, 2,2);
    }
    img.save("C:\\AGTU\\img\\point\\point.png","png");
}
