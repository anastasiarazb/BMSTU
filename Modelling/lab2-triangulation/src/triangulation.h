#ifndef TRIANGULATION_H
#define TRIANGULATION_H

//#include <list>
//#include <pair>
#include <functional>
#include <set>
#include "rasterization.h"

class Triangle {
public:
    enum class Orientation {
        CW, CCW
    };

    Triangle(const Point &a, const Point &b, const Point &c)
        :	points {a, b, c},
            edges {{a, b}, {b, c}, {c, a}},
            guides{glm::normalize(b-a), glm::normalize(c-b), glm::normalize(a-c)}
    {}
    Triangle() = default;

    Triangle(const Triangle &o)
        : points   {o.points[0],    o.points[1],    o.points[2]},
          edges    {o.edges [0],    o.edges [1],    o.edges [2]},
          guides   {o.guides[0],    o.guides[1],    o.guides[2]},
          neighbour{o.neighbour[0], o.neighbour[1], o.neighbour[2]} {}
    Triangle &operator=(const Triangle &o) {
        for (int i = 0; i < 3; ++i) {
            points[i] = o.points[i];
            edges[i] = o.edges[i];
            guides[i] = o.guides[i];
            neighbour[i] = o.neighbour[i];
        }
        return *this;

    }

    bool sharesVertexWith(Triangle const &triangle) const;

    bool circumCircleContains(Point const &v) const;

    Triangle& make_CCW();

    bool operator==(Triangle const &rhs) const;

//    Triangle &operator=(Triangle &other) {
//        set(other.a(), other.b(), other.c());
//        return *this;
//    }


//    Point a, b, c;
    const Point &a() const {return points[0];}
    const Point &b() const {return points[1];}
    const Point &c() const {return points[2];}
    Point points[3];
    Edge edges[3];
    glm::vec3 guides[3];
    Triangle *neighbour[3] = {nullptr};

    float maxCos() const;
    bool isInside(const Point &p) const;
    void set(const Point &a, const Point &b, const Point &c);

    static void setNeigbours(Triangle &M, int NtoMidx, Triangle &N, int MtoNidx) {
        M.neighbour[NtoMidx] = &N;
        N.neighbour[MtoNidx] = &M;
    }

    Triangle *getNext(const Point &p, Point &next_p, Orientation dir);
    Point maxPointByX() const;
    Point maxPointByY() const;
    Point minPointByX() const;
    Point minPointByY() const;
};
std::ostream& operator<< (std::ostream& os, const Triangle& x);
struct Edge;
class  Triangle;

std::ostream& operator<<(std::ostream& os, const Triangulation &T);

struct EdgeTriPair {
    Edge *edge = nullptr;
    Triangle *tri = nullptr;
    EdgeTriPair(Edge *edge, Triangle *tri): edge(edge), tri(tri) {}
    EdgeTriPair() = default;
    EdgeTriPair(EdgeTriPair &&other) : edge(other.edge), tri(other.tri) {}
    EdgeTriPair(const EdgeTriPair &other) : edge(other.edge), tri(other.tri) {}
};

class Triangulation: public std::vector<Triangle> {
public:
    Triangulation(const Triangulation &other) : std::vector<Triangle>(other) {initialize();}
    Triangulation(Triangulation&& other) : std::vector<Triangle>(std::move(other)) {initialize();}
    Triangulation() : std::vector<Triangle>() {}
    Triangulation(std::initializer_list<Triangle> init): std::vector<Triangle>(init) {initialize();}
    void initialize() {
        make_CCW();
        std::cout << *this << std::endl;
    }

    std::set<Edge> edges();
    void make_CCW() {
        for (Triangle &p : *this) {
            p.make_CCW();
        }
    }
    using Contour = std::vector<EdgeTriPair>;
    Contour contour(Point::Comparator compare = Point::lessByX);
    static Edge findTopTangent(Triangulation::Contour &L_contour,
                               Triangulation::Contour &R_contour,
                               std::vector<EdgeTriPair>::const_iterator &L_it,
                               std::vector<EdgeTriPair>::const_reverse_iterator &R_it
                               );
    static Edge findLowTangent(Triangulation::Contour &L_contour,
                               Triangulation::Contour &R_contour,
                               std::vector<EdgeTriPair>::const_reverse_iterator &L_it,
                               std::vector<EdgeTriPair>::const_iterator &R_it
                               );
};

class BB {
public:
    BB(const std::vector<Point> &points);
    BB(const Triangle &tr);
    PointSet::SplitType getOrientation() const {return orientation;}
    const char* orientation_to_string();
    float min_x, max_x, min_y, max_y;
private:
    PointSet::SplitType orientation;

};

Point calcNormal(std::vector<Triangle> &triangles, Triangle triangle, Point a);
Point interpolate(std::vector<Triangle> &triangles, Triangle triangle, Point a, Point b, float param);
std::vector<Triangle> addDetail(std::vector<Triangle> &triangles);

#endif // TRIANGULATION_H
