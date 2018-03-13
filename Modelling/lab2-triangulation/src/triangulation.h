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

    bool sharesVertexWith(Triangle const &triangle) const;

    Edge getNextEdge(const Edge &e);

    bool circumCircleContains(Point const &v) const;

    Triangle& make_CCW();

    bool operator==(Triangle const &rhs) const;

//    Point a, b, c;
    const Point &a() const {return points[0];}
    const Point &b() const {return points[1];}
    const Point &c() const {return points[2];}
    Point points[3];
    Edge edges[3];
    glm::vec3 guides[3];
    bool has_neighbour[3] = {false};

    float maxCos() const;
    bool isInside(const Point &p) const;
    void set(const Point &a, const Point &b, const Point &c);

    static void setNeigbours(Triangle &M, int NtoMidx, Triangle &N, int MtoNidx) {
        M.has_neighbour[NtoMidx] = true;
        N.has_neighbour[MtoNidx] = true;
    }

    void setNeigbours(bool ab, bool bc, bool ca) {
        has_neighbour[0] = ab;
        has_neighbour[1] = bc;
        has_neighbour[2] = ca;
    }

    bool contains(const Edge &e) const {
        return e == edges[0] || e == edges[1] || e == edges[2];
    }

    bool contains(const Point &p) const {
        return p == points[0] || p == points[1] || p == points[2];
    }

    using MarkedEdge = std::pair<Edge, bool>;

    static void flip(Triangle &A, Triangle &B);

    MarkedEdge getMarkedEdge(int i) {
        return MarkedEdge {edges[i], has_neighbour[i]};
    }

    static void check_and_flip(Triangle &A, Triangle &B);

    static bool exists(const Point &a, const Point &b, const Point &c) {
        return Edge::pseudoscalar(a-b, c-b) > 0;
    }
};

std::ostream& operator<< (std::ostream& os, const Triangle& x);
struct Edge;
class  Triangle;

std::ostream& operator<<(std::ostream& os, const Triangulation &T);

class Triangulation: public std::vector<Triangle> {
public:
    Triangulation(const Triangulation &other) : std::vector<Triangle>(other) {initialize();}
    Triangulation(Triangulation&& other) : std::vector<Triangle>(std::move(other)) {initialize();}
    Triangulation() : std::vector<Triangle>() {}
    Triangulation(std::initializer_list<Triangle> init): std::vector<Triangle>(init) {initialize();}
    void initialize() {
        make_CCW();
//        std::cout << "Triangulation::initialize()" << std::endl;
//        std::cout << *this << std::endl;
    }

    std::set<Edge> edges();
    void make_CCW() {
        for (Triangle &p : *this) {
            p.make_CCW();
        }
    }
    using Contour = std::vector<Edge>;
    Contour contour(Point::Comparator compare = Point::lessByX);
    static Edge findTopTangent(Contour &L_contour,
                               Contour &R_contour,
                               Contour::iterator &L_it,
                               Contour::reverse_iterator &R_it
                               );
    static Edge findLowTangent(Contour &L_contour,
                               Contour &R_contour,
                               Contour::reverse_iterator &L_it,
                               Contour::iterator &R_it
                               );
    std::vector<Triangle>::iterator find_adjacent(const Edge &e);
    using Iterator = std::vector<Triangle>::iterator;
    std::pair<Iterator, Iterator> find_both_adjacent(const Edge &e);
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
