#ifndef TRIANGULATION_H
#define TRIANGULATION_H
#include "rasterization.h"

class Triangle {
public:
    Triangle(Point const &a, const Point &b, const Point &c)
        :	a (a),
            b (b),
            c (c),
            edge {{a, b}, {b, c}, {c, a}}
    {}

    bool sharesVertexWith(Triangle const &triangle) const;

    bool circumCircleContains(Point const &v) const;


    bool operator==(Triangle const &rhs) const;


    Point a, b, c;
    Edge edge[3];
};

class Triangulation: private std::vector<Triangle> {
};

Point calcNormal(std::vector<Triangle> &triangles, Triangle triangle, Point a);
Point interpolate(std::vector<Triangle> &triangles, Triangle triangle, Point a, Point b, float param);
std::vector<Triangle> addDetail(std::vector<Triangle> &triangles);

#endif // TRIANGULATION_H
