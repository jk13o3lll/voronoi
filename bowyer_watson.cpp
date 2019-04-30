#include "Bowyer_watson.h"

using namespace std;
using namespace cv;

Edge::Edge(){}
Edge::Edge(const Point &a, const Point &b): p1(a), p2(b){}
Edge::~Edge(){}
Edge& Edge::operator=(const Edge &x){
    p1 = x.p1;  p2 = x.p2;
    return *this;
}
bool Edge::operator==(const Edge &x) const {
    return (p1 == x.p1 && p2 == x.p2) || (p1 == x.p2 && p2 == x.p1);
}


Triangle::Triangle(){}
Triangle::Triangle(const Point &a, const Point &b, const Point &c): p1(a), p2(b), p3(c){
    // Find circumcentre
    // Ax+By=E, Cx+Dy=F
    double A = p2.x - p1.x, B = p2.y - p1.y, C = p3.x - p1.x, D = p3.y - p1.y;
    double E = 0.5 * ((p2.x - p1.x) * (p2.x + p1.x) + (p2.y - p1.y) * (p2.y + p1.y));
    double F = 0.5 * ((p3.x - p1.x) * (p3.x + p1.x) + (p3.y - p1.y) * (p3.y + p1.y));
    circum_centre.x = (E * D - B * F) / (A * D - B * C);
    circum_centre.y = (A * F - E * C) / (A * D - B * C);
    circum_radius_sq = (p1.x-circum_centre.x) * (p1.x-circum_centre.x)
                     + (p1.y-circum_centre.y) * (p1.y-circum_centre.y);
}
Triangle::~Triangle(){}
Triangle& Triangle::operator=(const Triangle &x){
    p1 = x.p1;  p2 = x.p2;  p3 = x.p3;
    circum_centre = x.circum_centre;
    circum_radius_sq = x.circum_radius_sq;
    return *this;
}
void Triangle::toEdges(Edge edges[3]) const{
    edges[0].p1 = p1;   edges[0].p2 = p2;
    edges[1].p1 = p1;   edges[1].p2 = p3;
    edges[2].p1 = p2;   edges[2].p2 = p3;
}
bool Triangle::circum_contain(const Point &p) const{
    return (p.x - circum_centre.x) * (p.x - circum_centre.x)
         + (p.y - circum_centre.y) * (p.y - circum_centre.y) < circum_radius_sq;
}
bool Triangle::contain(const Edge &e) const{
    return (p1 == e.p1 && p2 == e.p2) || (p2 == e.p1 && p1 == e.p2) ||
           (p1 == e.p1 && p3 == e.p2) || (p3 == e.p1 && p1 == e.p2) ||
           (p2 == e.p1 && p3 == e.p2) || (p3 == e.p1 && p2 == e.p2);
}
bool Triangle::nextto(const Triangle &t) const{
    return (p1 == t.p1 && p2 == t.p2) || (p1 == t.p2 && p2 == t.p1) ||// edge1
           (p1 == t.p1 && p2 == t.p3) || (p1 == t.p3 && p2 == t.p1) ||
           (p1 == t.p2 && p2 == t.p3) || (p1 == t.p3 && p2 == t.p2) ||
           (p1 == t.p1 && p3 == t.p2) || (p1 == t.p2 && p3 == t.p1) ||// edge2
           (p1 == t.p1 && p3 == t.p3) || (p1 == t.p3 && p3 == t.p1) ||
           (p1 == t.p2 && p3 == t.p3) || (p1 == t.p3 && p3 == t.p2) ||
           (p2 == t.p1 && p3 == t.p2) || (p2 == t.p2 && p3 == t.p1) ||// edge3
           (p2 == t.p1 && p3 == t.p3) || (p2 == t.p3 && p3 == t.p1) ||
           (p2 == t.p2 && p3 == t.p3) || (p2 == t.p3 && p3 == t.p2);
}
void Triangle::draw(Mat &frame) const{
    Scalar red(0, 0, 255);
    line(frame, p1, p2, red);
    line(frame, p1, p3, red);
    line(frame, p2, p3, red);
}


void BowyerWatson::init(const Size &sz){
    wndsz = sz;
    triangulation.reserve(100);
    triangulation.push_back(Triangle(Point(-.5*wndsz.width,  wndsz.height),
                                        Point( .5*wndsz.width, -wndsz.height),
                                        Point(1.5*wndsz.width,  wndsz.height)));
    draw_flag = 0x10;
}
void BowyerWatson::add_point(const Point &p){
    size_t i, j, k, n;
    vector<int> bad_traiangles_ind;
    vector<Edge> polygon_edges;
    Edge edges[3];

    // find bad triangles
    bad_traiangles_ind.reserve(20);
    for(i = 0, n = triangulation.size(); i < n; ++i)
        if(triangulation[i].circum_contain(p))
            bad_traiangles_ind.push_back(i);
    // find polygon edges
    polygon_edges.reserve(20);
    for(i = 0, n = bad_traiangles_ind.size(); i < n; ++i){
        triangulation[bad_traiangles_ind[i]].toEdges(edges);
        for(j = 0; j < 3; ++j){
            for(k = 0; k < n; ++k)
                if(i != k && triangulation[bad_traiangles_ind[k]].contain(edges[j]))
                    break;
            if(k == n)
                polygon_edges.push_back(edges[j]);
        }
    }
    // remove bad triangles and add new triangles (use new one to replace)
    for(i = 0, n = bad_traiangles_ind.size(); i < n; ++i)
        triangulation[bad_traiangles_ind[i]] = Triangle(p, polygon_edges[i].p1, polygon_edges[i].p2);
    for(n = polygon_edges.size(); i < n; ++i)
        triangulation.push_back(Triangle(p, polygon_edges[i].p1, polygon_edges[i].p2));
}

void BowyerWatson::draw(Mat &frame){
    size_t i, j, k, n;

    // draw
    frame.create(wndsz.height, wndsz.width, CV_32FC3); // if already exist then no new create
    frame.setTo(Scalar(0));
    if(draw_flag & 0x1)
        for(i = 0, n = triangulation.size(); i < n; ++i)
            triangulation[i].draw(frame);
    if(draw_flag & 0x10){
        // connect circum centre with neighbor
        Scalar green(0, 255, 0), red(0, 0, 255);
        for(i = 0, n = triangulation.size(); i < n; ++i){
            for(j = i + 1, k = 0; j < n && k < 3; ++j)
                if(triangulation[i].nextto(triangulation[j])){
                    line(frame, triangulation[i].circum_centre, triangulation[j].circum_centre, green);
                    ++k;
                }
        }
    }
}