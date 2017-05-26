#include "mydescriptor.h"

myDescriptor::myDescriptor(matrixImg &gauss, matrixImg &x, matrixImg &y, Border border, int rSizeX, int rSizeY, int HistogramSize, int partsCount)
{
    this->gauss=gauss;
    this->sobelX=x;
    this->sobelY=y;
    this->border=border;
    this->rSizeX=rSizeX;
    this->rSizeY=rSizeY;
    this->histogramSize=HistogramSize;
    this->partsCount=partsCount;
}

vector<discript> myDescriptor::calculationDescriptor(const vector<Point> &points,double aroundAngle)
{
    vector<discript> result;
    vector<double> content;
    const int size = rSizeX * histogramSize;
    foreach (Point point, points) {
        content.clear();
        for(int i=0;i<4*size*size;i++){
            content.push_back(0);
        }
        for(int x=-size; x<size; x++){
            for(int y=-size; y<size; y++){

                int pointX = point.x + x;
                int pointY = point.y + y;
                double dx = sobelX.getElement(border,pointX, pointY);
                double dy = sobelY.getElement(border,pointX, pointY);
                //content.push_back(atan2(dy, dx)* 180 / M_PI);

                double sigma = 2;
                double gausXY = exp(-(pow(x,2) + pow(y,2))/(2*pow(sigma,2)))/(2 * M_PI * pow(sigma,2));

                double w = sqrt(pow(dx,2) + pow(dy,2)) * gausXY;

                int aroundX = (int)(x*cos(aroundAngle) + y*sin(aroundAngle)) + size/2;
                int aroundY = (int)(y*cos(aroundAngle) - x*sin(aroundAngle)) + size/2;

                if (aroundX < 0 || aroundX > size - 1 || aroundY < 0 || aroundY > size - 1) {
                    continue;
                }
                int histogramNum = aroundX/histogramSize*rSizeX + aroundY/histogramSize;
                double angle = atan2(dy, dx) + M_PI- aroundAngle;
                if (angle > 2 * M_PI) {
                    angle = angle - 2 * M_PI;
                }
                if (angle < 0) {
                    angle = angle + 2 * M_PI;
                }

                double partNum = angle / M_PI / 2 * partsCount;
                partNum = max(0.0, min(partsCount - 0.001, partNum));

                const double partVariation = partNum - (int)partNum;
                int index = histogramNum * partsCount + (int)round(partNum) % partsCount;
                content.at(index)+= w * (1 - partVariation);
                index = (int)(histogramNum * partsCount + partNum + 1) % partsCount;
                content.at(index) += w * partVariation;
            }
        }
        //
        int firstIndex=0;
        int secondIndex=1;
        if(content.at(firstIndex) > content.at(secondIndex)){
            firstIndex = 0;
            secondIndex = 1;
        }    else{
            secondIndex = 0;
            firstIndex = 1;
        }
        for (int i = 0; i < content.size(); i++) {
            if(content.at(secondIndex) < content.at(i)) {
                if(content.at(firstIndex) < content.at(i)){
                    secondIndex = firstIndex;
                    firstIndex = i;
                }else{
                    secondIndex = i;
                }
            }else{
                firstIndex = firstIndex;
                secondIndex = secondIndex;
            }
        }

        if(content.at(firstIndex) * 0.8 < content.at(secondIndex)){
            result.push_back(calculationDescriptor2(content,point,firstIndex));
            result.push_back(calculationDescriptor2(content,point,secondIndex));
        }else{
            result.push_back(calculationDescriptor2(content,point,firstIndex));
        }

    }
    myVectorDescriptor.clear();
    myVectorDescriptor.reserve(result.size()+myVectorDescriptor.size());
    myVectorDescriptor.insert(myVectorDescriptor.end(), result.begin(), result.end());
    return result;
}

double myDescriptor::distance(myDescriptor &descr1, myDescriptor &descr2, int i, int j)
{
    auto vector1= descr1.getVectorDiscriptor();
    auto vector2= descr2.getVectorDiscriptor();
    double distance=0;
    for(int n=0;n<vector1.size();n++){
        double dist = vector1.at(i).vectorDescript.at(n) - vector2.at(j).vectorDescript.at(n);
        distance += pow(dist,2);
    }
    return sqrt(distance);
}

discript myDescriptor::normal(discript &descr){
    double sum = 0;
    foreach (double element, descr.vectorDescript) {
        sum+= pow(element,2);
    }
    sum = sqrt(sum);
    vector<double> vector;
    foreach (double element, descr.vectorDescript) {
        vector.push_back(element/sum);
    }
    return discript(descr.x,descr.y,vector);
}
discript myDescriptor::calculationDescriptor2(const vector<double> &cont,const Point &point,const int index){

    const int size = rSizeX * histogramSize;

    const double di = - 0.5 * (cont[(index + 1 + size) % size] - cont[(index - 1 + size) % size]) / (cont[(index + 1 + size) % size] + cont[(index - 1 + size) % size] - 2 * cont[index]) ;
    const double aroundAngle = 2 * M_PI * (index + di + 0.5) / size ;

    vector<double> content;
    for(int i=0;i<4*size*size;i++){
        content.push_back(0);
    }
    for(int x=-size; x<size; x++){
        for(int y=-size; y<size; y++){

            int pointX = point.x + x;
            int pointY = point.y + y;
            double dx = sobelX.getElement(border,pointX, pointY);
            double dy = sobelY.getElement(border,pointX, pointY);

            double sigma = 2;
            double gausXY=   exp(-(pow(x,2) + pow(y,2))/(2*pow(sigma,2)))/(2 * M_PI * pow(sigma,2));

            double w = sqrt(pow(dx,2) + pow(dy,2)) * gausXY;

            int aroundX = (int)(x*cos(aroundAngle) + y*sin(aroundAngle)) + size/2;
            int aroundY = (int)(y*cos(aroundAngle) - x*sin(aroundAngle)) + size/2;

            if (aroundX < 0 || aroundX > size - 1 || aroundY < 0 || aroundY > size - 1) {
                continue;
            }
            int histogramNum = aroundX/histogramSize*rSizeX + aroundY/histogramSize;
            double angle = atan2(dy, dx) + M_PI;
            if (angle > 2 * M_PI) {
                angle = angle - 2 * M_PI;
            }
            if (angle < 0) {
                angle = angle + 2 * M_PI;
            }

            double partNum = angle / M_PI / 2 * partsCount;
            partNum = max(0.0, min(partsCount - 0.001, partNum));

            const double partVariation = partNum - (int)partNum;
            int index = histogramNum * partsCount + (int)round(partNum) % partsCount;
            content.at(index)+= w * (1 - partVariation);
            index = (int)(histogramNum * partsCount + partNum + 1) % partsCount;
            content.at(index) += w * partVariation;
        }
    }
    discript decr=discript(point.x,point.y,content);
    decr = normal(decr);

    for(int i=0;i<decr.vectorDescript.size();i++){
        if(decr.vectorDescript[i]<0.2)
        {
            decr.vectorDescript[i]=0.2;
        }
    }
    decr = normal(decr);
    return decr;
}
vector<ProximateDescriptors> myDescriptor::findOverlap(const myDescriptor &firstD, const myDescriptor &secondD, double &best){
    int fDescriptorsCount = (int)firstD.myVectorDescriptor.size();
    int sDescriptorsCount = (int)secondD.myVectorDescriptor.size();
    vector<double> distances = calculateDistance(firstD,secondD);
    double firstOverlapDistance;
    double secondOverlapDistance;
    vector<ProximateDescriptors> overlaps;
    int firstOverlap, secondOverlap;

    for(int i=0;i<fDescriptorsCount;i++){
        const int index = i*sDescriptorsCount;
        if (distances[index] < distances[index + 1]){
            firstOverlap = 0;
            firstOverlapDistance = distances[index];
            secondOverlap = 1;
            secondOverlapDistance = distances[index + 1];
        }
        else{
            firstOverlap = 1;
            firstOverlapDistance = distances[index+1];
            secondOverlap = 0;
            secondOverlapDistance = distances[index];
        }

        for(int j=2;j<sDescriptorsCount;j++){
            double distance = distances[index+j];
            if(distance < distances[index+firstOverlap]){
                secondOverlap = firstOverlap;
                secondOverlapDistance = firstOverlapDistance;
                firstOverlap = j;
                firstOverlapDistance = distance;
                continue;
            }
            if(distance < distances[index+secondOverlap]){
                secondOverlap = j;
                secondOverlapDistance = distance;
            }
        }
        double matchProc = firstOverlapDistance/secondOverlapDistance;
        if(matchProc < 0.7)
        {
            if(matchProc < best){
                best = matchProc;
            }
            const int fX = firstD.myVectorDescriptor[i].x;
            const int fY = firstD.myVectorDescriptor[i].y;
            const int sX = secondD.myVectorDescriptor[firstOverlap].x;
            const int sY = secondD.myVectorDescriptor[firstOverlap].y;
            overlaps.emplace_back(ProximateDescriptors(fX,fY,sX,sY));
        }
    }
    return overlaps;
}
vector<double> myDescriptor::calculateDistance(const myDescriptor &f, const myDescriptor &s){
    const int fDescriptorsCount = (int)f.myVectorDescriptor.size();
    const int sDescriptorsCount = (int)s.myVectorDescriptor.size();
    cout<<fDescriptorsCount<<endl;
    cout<<sDescriptorsCount<<endl;
    const int descriptorSize = rSizeX * rSizeY * partsCount;
    vector<double> distances;
    distances.resize(fDescriptorsCount * sDescriptorsCount);
    for(int i=0;i<fDescriptorsCount;i++){
        for(int j=0;j<sDescriptorsCount;j++){
            double distance = 0;
            for(int n=0;n<descriptorSize;n++){
                const double dist = f.myVectorDescriptor[i].vectorDescript[n] - s.myVectorDescriptor[j].vectorDescript[n];
                distance += pow(dist,2);
            }
            distances[i*sDescriptorsCount+j] = sqrt(distance);
        }
    }
    return distances;
}
