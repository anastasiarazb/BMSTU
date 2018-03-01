#ifndef TRIANGULATION_H
#define TRIANGULATION_H
#include "rasterization.h"
#include <set>

class Triangle {
public:
    Triangle(const Point &a, const Point &b, const Point &c)
        :	a (a),
            b (b),
            c (c),
            edge {{a, b}, {b, c}, {c, a}},
            guides{glm::normalize(b-a), glm::normalize(c-b), glm::normalize(a-c)}
    {}
    Triangle() = default;

    bool sharesVertexWith(Triangle const &triangle) const;

    bool circumCircleContains(Point const &v) const;


    bool operator==(Triangle const &rhs) const;


    Point a, b, c;
    Edge edge[3];
    glm::vec3 guides[3];
    Triangle *neighbour[3] = {nullptr};

    float maxCos() const;
    bool isInside(const Point &p) const;
    void set(const Point &a, const Point &b, const Point &c);

    static void setNeigbours(Triangle &M, int NtoMidx, Triangle &N, int MtoNidx) {
        M.neighbour[NtoMidx] = &N;
        N.neighbour[MtoNidx] = &M;
    }
};

struct Edge;

class Triangulation: public std::vector<Triangle> {
public:
    Triangulation(const Triangulation &other) : std::vector<Triangle>(other) {}
    Triangulation(Triangulation&& other) : std::vector<Triangle>(std::move(other)) {}
    Triangulation() : std::vector<Triangle>() {}
    Triangulation(std::initializer_list<Triangle> init): std::vector<Triangle>(init) {}
    std::set<Edge> edges();
};

class BB {
public:
    BB(const std::vector<Point> &points);
    PointSet::SplitType getOrientation() const {return orientation;}
    const char* orientation_to_string();
private:
    PointSet::SplitType orientation;
    float min_x, max_x, min_y, max_y;
};

Point calcNormal(std::vector<Triangle> &triangles, Triangle triangle, Point a);
Point interpolate(std::vector<Triangle> &triangles, Triangle triangle, Point a, Point b, float param);
std::vector<Triangle> addDetail(std::vector<Triangle> &triangles);

#endif // TRIANGULATION_H
