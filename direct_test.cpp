// g++ -IC:/OpenCV/build/opencv-4.0.1_omp_tbb_mkl/install/include direct.cpp direct_test.cpp -o test.exe -LC:/OpenCV/build/opencv-4.0.1_omp_tbb_mkl/install/x64/mingw/lib -lopencv_core401 -lopencv_imgproc401 -lopencv_highgui401

#include "direct.h"

using namespace std;
using namespace cv;

double my_distance2(int dx, int dy){
    return abs(dx) + abs(dy);
}


void onMouse(int event, int x, int y, int flags, void *param);

// Global variables
const char wnd_name[] = "Display";
DirectVoronoi test;
Mat frame;
int key;

int main(){
    test.init(Size(600, 400), my_distance2);
    // test.init(Size(600, 400));
    
    namedWindow(wnd_name);
    setMouseCallback(wnd_name, onMouse);

    test.draw(frame);
    imshow(wnd_name, frame);
    while(1){

        key = waitKey(33);
        if(key == 27)  // ESC
            break;
    }
    destroyAllWindows();

    return 0;
}

void onMouse(int event, int x, int y, int flags, void *param){
    if(event == EVENT_LBUTTONUP){
        test.add_point(Point(x, y));
        test.draw(frame);
        imshow(wnd_name, frame);
    }
}