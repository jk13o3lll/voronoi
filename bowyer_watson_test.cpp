// g++ -IC:/OpenCV/build/opencv-4.0.1_omp_tbb_mkl/install/include bowyer_watson.cpp bowyer_watson_test.cpp -o test.exe -LC:/OpenCV/build/opencv-4.0.1_omp_tbb_mkl/install/x64/mingw/lib -lopencv_core401 -lopencv_imgproc401 -lopencv_highgui401

#include "bowyer_watson.h"
#include <vector>

using namespace std;
using namespace cv;

void onMouse(int event, int x, int y, int flags, void *param);

// Global variables
const char wnd_name[] = "Display";
BowyerWatson test;
Mat frame;
int key;

int main(){
    test.init(Size(400, 300));
    
    namedWindow(wnd_name);
    setMouseCallback(wnd_name, onMouse);

    test.draw(frame);
    imshow(wnd_name, frame);
    while(1){
        key = waitKey(33);
        if(key == 27)  // ESC
            break;
        else if(key == 32){  // space
            if(test.draw_flag == 0x1)       test.draw_flag = 0x10;
            else if(test.draw_flag == 0x10) test.draw_flag = 0x11;
            else                            test.draw_flag = 0x1;
            test.draw(frame);
            imshow(wnd_name, frame);
        }
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