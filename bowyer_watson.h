#ifndef __BOWYER_WATSON_H__
#define __BOWYER_WATSON_H__

#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

struct Edge{
    Edge();
    Edge(const cv::Point &a, const cv::Point &b);
    ~Edge();
    Edge& operator=(const Edge &x);
    bool operator==(const Edge &x) const;
    
    cv::Point p1, p2;
};

struct Triangle{
    Triangle();
    Triangle(const cv::Point &a, const cv::Point &b, const cv::Point &c);
    ~Triangle();
    Triangle& operator=(const Triangle &x);

    void toEdges(Edge edges[3]) const;
    bool circum_contain(const cv::Point &p) const;  // the circum of triangle contain the cv::Point
    bool contain(const Edge &e) const ;
    bool nextto(const Triangle &t) const;  // share one edge
    void draw(cv::Mat &frame) const;

    cv::Point p1, p2, p3;
    cv::Point2d circum_centre;
    double circum_radius_sq;
};

struct BowyerWatson{
    void init(const cv::Size &sz);
    void add_point(const cv::Point &p);
    void draw(cv::Mat &frame);

    std::vector<Triangle> triangulation;
    cv::Size wndsz;
    unsigned char draw_flag; // 0x0: nothing, 0x1: delaunay triangulation, 0x10: voroni diagram, 0x11: both
};

#endif