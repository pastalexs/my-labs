#include "mySearchPoint.h"
#include <iostream>
#include <ctime>

using namespace std;

mySearchPoint::mySearchPoint(matrixImg &img)
{
    this->myImg=img;
}

matrixImg mySearchPoint::harris(Border border, double limit)
{
    matrixImg gauss = GausPiramida::getGauss(myImg,1);
    const int windows = 2;

    //Sobel
    double massVert[] ={1,0,-1};
    double massGoris[] = {1,2,1};

    const int size3=3;

    matrixImg sobY = myImg.twoConvolution(border,massVert,massGoris,size3);

    matrixImg sobX = myImg.twoConvolution(border,massGoris,massVert,size3);
    //
    vector<double> result;
    double A,B,C;
    const double k=0.06;
    for(int y=0;y<myImg.getWidth();y++){
        for(int x=0;x<myImg.getHeignt();x++){
            A=B=C=0;
            for(int u=-windows;u<=windows;u++)
            {
                for(int v=-windows;v<=windows;v++)
                {
                    double w = gauss.getElement(border,u+windows,v+windows);
                    double Ix = sobX.getElement(border,y+u,x+v);
                    double Iy = sobY.getElement(border,y+u,x+v);
                    A += w * pow(Ix,2);
                    B += w * Ix * Iy;
                    C += w * pow(Iy,2);
                }
            }
            double f = A*C - pow(B,2) + k*pow(A+C,2);
            result.push_back(f);

        }
    }
    searchPoint(border,matrixImg(result,myImg.getWidth(),myImg.getHeignt()),windows, limit);
    return matrixImg(result,myImg.getWidth(),myImg.getHeignt());
}

matrixImg mySearchPoint::moravek(Border border, double limit)
{
    vector<double> vectorError;
    double window = 3;
    vector<double> result;
    for(int y=0;y<myImg.getWidth();y++)
    {
        for(int x=0; x<myImg.getHeignt();x++)
        {
            vectorError.clear();
            for(int dy=-1; dy<=1; dy++)
            {
                for(int dx=-1; dx<=1; dx++){
                    if(dx!=0 && dy!=0){
                        double currwntError =0;
                        for(int u = -window; u<=window; u++){
                            for(int v=-window; v<=window;v++){
                                double inten = myImg.getElement(border,y+u,x+v);
                                double shiftInten = myImg.getElement(border,y+u+dy,x+v+dx);
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
    searchPoint(border,matrixImg(result,myImg.getWidth(),myImg.getHeignt()),window, limit);
    return matrixImg(result,myImg.getWidth(),myImg.getHeignt());
}

void mySearchPoint::searchPoint(Border border, const matrixImg &img, int windows, double limit)
{
    const int window = 2;//windows;
    vectorPoint.clear();
    for(int y=0;y<img.getWidth();y++){
        for(int x=0;x<img.getHeignt();x++){
            double intens = img.getElement(border,y,x);
            if(intens > limit){
                 cout<<"intens: "<<intens<<endl;
                bool isMaxLoc = true;
                for(int u=-window;u<=window && isMaxLoc == true;u++){
                    for(int v=-window;v<=window;v++){
                        if (u == 0 && v == 0) {
                            //if( u!=
                        }
                        else{
                            double shiftIntens = img.getElement(border,y+u,x+v);
                            // cout<<"intens: "<<intens<<" shiftIntens: "<<shiftIntens<<endl;
                            if(shiftIntens > intens){
                                isMaxLoc = false;
                                break;
                            }
                        }
                    }
                }
                if(isMaxLoc){
                    vectorPoint.push_back(Point(x,y,intens));
                }
            }
        }
    }
    cout<<"size: "<<vectorPoint.size()<<endl;
    cout<<"time: "<<clock()/1000.0<<endl;
}

double mySearchPoint::distance(int a1, int a2, int b1, int b2)
{
    return sqrt(pow(a2-a1,2)+pow(b2-b1,2));
}

vector<double> mySearchPoint::normal(vector<double> &vectorImg,double limit)
{
    double min=*min_element(vectorImg.begin(),vectorImg.end()) ;
    double max=*max_element(vectorImg.begin(),vectorImg.end());
    double range=max - min;
    vector<double> result;
    for(int y=0;y<myImg.getWidth();y++){
        for(int x=0;x<myImg.getHeignt();x++){
            double intens = (vectorImg.at(x+y*myImg.getHeignt())-min)/range*100;
            //cout<<"intens: "<<intens<<endl;
            if(intens>=limit){
                result.push_back(intens);
            }else {
                result.push_back(0);
            }
        }
    }
    return result;
}
QImage mySearchPoint::saveImgAndPoint()
{
    QImage img = myImg.getImg();
    QPainter painter(&img);
    painter.setPen(qRgb(255,0,0));
    for(Point point : vectorPoint) {
        painter.drawEllipse(point.y,point.x, 1,1);
    }
    img.save("C:\\AGTU\\img\\point\\point.png","png");
    return img;
}

void mySearchPoint::adaptiveNonMaximumSuppression(int countPoint)
{
    int rad=0;
    const int maxRad= distance(0,myImg.getHeignt(),0,myImg.getWidth());
    const double cof= 0.9;
    double dist, intensI, intensJ;
    while(vectorPoint.size()>countPoint && rad<=maxRad){
        for(int i=0;i< vectorPoint.size();i++){
            intensI = vectorPoint[i].intens/cof;
            for(int j=i+1; j<vectorPoint.size(); j++)
            {
                dist= distance(vectorPoint[i].x,vectorPoint[j].x,vectorPoint[i].y,vectorPoint[j].y);
                intensJ = /*cof**/ vectorPoint[j].intens;
                if(dist<=rad && intensJ > intensI){
                    vectorPoint.erase(vectorPoint.begin()+i);
                    i--;
                    break;
                }
            }
        }
        rad++;
    }
    cout<<"adaptiveNonMaximumSuppression size: "<<vectorPoint.size()<<endl;
    cout<<"time: "<<clock()/1000.0<<endl;
}
