#include "direct.h"

using namespace std;
using namespace cv;

double my_distance(int dx, int dy){
    return sqrt(dx * dx + dy * dy);
}


DirectVoronoi::DirectVoronoi():
    width(0), height(0), size(0), step(0), dmap(NULL), min_i(NULL), min_d(NULL){
    points.reserve(100);
    colors.reserve(100);
    srand(time(NULL));
}
DirectVoronoi::~DirectVoronoi(){ release(); }
void DirectVoronoi::init(const Size &sz, double (*d)(int, int)){
    int i, j, ii;

    release();

    width = sz.width;
    height = sz.height;
    size = width * height;
    step = width;

    dmap = new double[size];
    min_i = new int[size];
    min_d = new double[size];
    for(i = 0, ii = 0; i < height; ++i, ii += step)
        for(j = 0; j < width; ++j){
            dmap[ii + j] = d(i, j);
            min_i[ii + j] = -1;
            min_d[ii + j] = DBL_MAX;
        }
}
void DirectVoronoi::add_point(const Point &p){
    int i, j, ii, k = points.size();
    double tmp;

    // this part can be parrallelized: for(i = 0; i < size; ++i){ ... }
    for(i = 0, ii = 0; i < height; ++i, ii += step)
        for(j = 0; j < width; ++j){
            tmp = dmap[abs(i - p.y) * step + abs(j - p.x)];
            if(tmp < min_d[ii + j]){
                min_i[ii + j] = k;
                min_d[ii + j] = tmp;
            }
        }
    
    points.push_back(p);
    colors.push_back(Vec3b(rand() % 256, rand() % 256, rand() % 256));
}
void DirectVoronoi::draw(Mat &frame){
    int i, j, ii;
    Vec3b *rowicol;
    Vec3b red(0, 0, 255);

    frame.create(Size(width, height), CV_8UC3);
    frame.setTo(Vec3b(0, 0, 0));
    for(i = 0, ii = 0; i < height; ++i, ii += step){
        rowicol = frame.ptr<Vec3b>(i);
        for(j = 0; j < width; ++j)
            if(min_i[ii + j] != -1)
                rowicol[j] = colors[min_i[ii + j]];
    }
    for(i = 0, j = points.size(); i < j; ++i)
        circle(frame, points[i], 2, red);
}
void DirectVoronoi::release(){
    if(dmap)    delete[] dmap;
    if(min_i)   delete[] min_i;
    if(min_d)   delete[] min_d;
    points.clear();
}
