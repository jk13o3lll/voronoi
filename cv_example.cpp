// g++ -IC:/OpenCV/build/opencv-4.0.1_omp_tbb_mkl/install/include cv_example.cpp -o test.exe -LC:/OpenCV/build/opencv-4.0.1_omp_tbb_mkl/install/x64/mingw/lib -lopencv_core401 -lopencv_imgproc401 -lopencv_highgui401

#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

// Global variables
Size wndsz(600, 400);
Subdiv2D test;
vector<Point> points;
const char wnd_name[] = "Display";
Mat frame;

// functions
void clear_frame(Mat &frame);
void draw_points(Mat &frame, const vector<Point> &points);
void draw_delaunay(Mat &frame, const Subdiv2D &subdiv);
void draw_voronoi(Mat &frame, const Subdiv2D &subdiv);
void onMouse(int event, int x, int y, int flags, void *param);

int main(){
    int key;

    test.initDelaunay(Rect(0, 0, wndsz.width, wndsz.height));
    namedWindow(wnd_name);
    setMouseCallback(wnd_name, onMouse);

    clear_frame(frame);
    imshow("Display", frame);
    while(1){
        key = waitKey(33);
        if(key == 27)  // ESC
            break;
    }
    destroyAllWindows();

    return 0;
}

void clear_frame(Mat &frame){
    frame.create(wndsz, CV_8UC3);
    frame.setTo(Vec3b(0, 0, 0));
}
void draw_points(Mat &frame, const vector<Point> &points){
    int i, n;
    Scalar red(0, 0, 255);

    for(i = 0, n = points.size(); i < n; ++i)
        circle(frame, points[i], 2, red);
}
void draw_delaunay(Mat &frame, const Subdiv2D &subdiv){
    int i, n;
    Scalar blue(255, 0, 0);

    vector<Vec4f> edges;
    subdiv.getEdgeList(edges);
    for(i = 0, n = edges.size(); i < n; ++i)
        line(frame, Point(edges[i][0], edges[i][1]), Point(edges[i][2], edges[i][3]), blue);
}
void draw_voronoi(Mat &frame, Subdiv2D &subdiv){
    int i, j, ni, nj;
    Scalar green(0, 255, 0);
    
    vector<int> idx;
    vector<vector<Point2f>> faceList;
    vector<Point2f> faceCenters;

    subdiv.getVoronoiFacetList(idx, faceList, faceCenters);
    for(i = 0, ni = faceList.size(); i < ni; ++i)
        for(j = 1, nj = faceList[i].size(); j < nj; ++j)
            line(frame, Point(faceList[i][j-1]), Point(faceList[i][j]), green);
}

void onMouse(int event, int x, int y, int flags, void *param){
    if(event == EVENT_LBUTTONUP){
        points.push_back(Point(x, y));
        test.insert(Point2f(x, y));
        clear_frame(frame);
        draw_delaunay(frame, test);
        draw_voronoi(frame, test);
        draw_points(frame, points);
        imshow(wnd_name, frame);
    }
}