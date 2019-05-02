#ifndef _DIRECT_H_
#define _DIRECT_H_

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <cmath>

// time invariant, indepedent to location
double my_distance(int dx, int dy);


struct DirectVoronoi{   // direct compute 2d voronoi for discrete pixels
    DirectVoronoi();
    ~DirectVoronoi();
    void init(const cv::Size &sz, double (*d)(int, int) = my_distance);
    void add_point(const cv::Point &p);
    void draw(cv::Mat &frame);
    void release();

    int width, height, size, step;
    double *dmap; // distance map
    int *min_i;
    double *min_d;
    std::vector<cv::Point> points;
    std::vector<cv::Vec3b> colors;
};

#endif