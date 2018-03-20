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
    bool hasInside(const Point &p) const;
    Triangle &set(const Point &a, const Point &b, const Point &c);

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
    bool is_degenerate() const {
        return maxCos() > 0.9;
    }

};

std::ostream& operator<< (std::ostream& os, const Triangle& x);
struct Edge;

class Contour: public std::vector<Edge>
{
    using Iterator = std::vector<Edge>::iterator;
    using ReverseIterator = std::vector<Edge>::reverse_iterator;

public:
    class iterator: public Iterator
    {
        Contour *src = nullptr;
    public:
        iterator(Iterator iter, Contour *contour) : Iterator(iter), src(contour) {}
        iterator() = default;
        iterator(iterator &&other) = default;
        iterator& operator=(const iterator&) = default;
        iterator(const iterator &other) = default;
        iterator &operator++() {
            if (*this == src->end()) {
                *this = src->begin();
            }
            Iterator::operator++();
            return *this;
        }
        iterator operator+(size_t i) const {
            size_t delta = src->end()-*this;
            return i <= delta
                    ? iterator((Iterator)*this + i, src)
                    : iterator((Iterator)src->begin() + (i - delta), src);
        }
        iterator operator-(size_t i) const {
            size_t delta = *this - src->begin();
            return i <= delta
                    ? iterator((Iterator)*this - i, src)
                    : iterator((Iterator)src->end() - (i - delta), src);
        }

        iterator &operator--() {
            if ((Iterator)*this == src->begin()) {
                *this = src->end();
            }
            Iterator::operator--();
            return *this;
        }

        Edge &operator*() {
            return (Iterator)*this == src->end()
                    ? *src->begin()
                    : Iterator::operator*();
        }
        Edge *operator->() const {
            return (Iterator)*this == src->end()
                    ? src->begin().operator->()
                    : Iterator::operator->();
        }
    };
    class reverse_iterator: public ReverseIterator
    {
        Contour *src = nullptr;
    public:
        reverse_iterator(ReverseIterator iter, Contour *contour) : ReverseIterator(iter), src(contour) {}
        reverse_iterator(reverse_iterator &&other) = default;
        reverse_iterator() = default;
        reverse_iterator(const reverse_iterator &other) = default;
        reverse_iterator& operator=(const reverse_iterator&) = default;
        reverse_iterator &operator++() {
            if ((ReverseIterator)(*this) == src->rend()) {
                *this = reverse_iterator(src->rbegin(), src);
            }
            ReverseIterator::operator++();
            return *this;
        }
        reverse_iterator &operator--() {
            if ((ReverseIterator)(*this) == src->rbegin()) {
                *this = reverse_iterator(src->rend(), src);
            }
            ReverseIterator::operator--();
            return *this;
        }
        reverse_iterator operator+(size_t i) const {
            size_t delta = src->rend()-*this;
            return i <= delta
                    ? reverse_iterator((ReverseIterator)*this + i, src)
                    : reverse_iterator((ReverseIterator)src->rbegin() + (i - delta), src);
        }
        reverse_iterator operator-(size_t i) const {
            size_t delta = *this - src->rbegin();
            return i <= delta
                    ? reverse_iterator((ReverseIterator)*this - i, src)
                    : reverse_iterator((ReverseIterator)src->rend() - (i - delta), src);
        }
        Edge &operator*() {
            return *this == src->rend()
                    ? *src->rbegin()
                    : ReverseIterator::operator*();
        }
        Edge *operator->() const {
            return *this == src->rend()
                    ? src->rbegin().operator->()
                    : ReverseIterator::operator->();
        }
    };
    Contour(const Contour &) = default;
    Contour(Contour &&) = default;
    Contour &operator=(const Contour &) = default;
    Contour() : std::vector<Edge>() {}
    Contour(std::initializer_list<Edge> init): std::vector<Edge>(init) {}
    Contour &reverse() {
        for (Edge &e: *this) {
            std::swap(e.a, e.b);
        }
        return *this;
    }

    iterator begin() {
        return iterator(std::vector<Edge>::begin(), this);
    }
    iterator end() {
        return iterator(std::vector<Edge>::end(), this);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(std::vector<Edge>::rbegin(), this);
    }
    reverse_iterator rend() {
        return reverse_iterator(std::vector<Edge>::rend(), this);
    }

};

std::ostream& operator<<(std::ostream& os, const Triangulation &T);

class Triangulation: public std::vector<Triangle> {
public:
    Triangulation(const Triangulation &other) : std::vector<Triangle>(other) {initialize();}
    Triangulation(Triangulation&& other) : std::vector<Triangle>(std::move(other)) {initialize();}
    Triangulation() : std::vector<Triangle>() {}
    Triangulation(std::initializer_list<Triangle> init): std::vector<Triangle>(init) {initialize();}
    void initialize() {
        make_CCW();
        std::cout << "Triangulation::initialize()" << *this << std::endl;
    }

    std::set<Edge> edges();
    void make_CCW() {
        for (Triangle &p : *this) {
            p.make_CCW();
        }
    }
    Contour contour(Point::Comparator compare = Point::lessByX);
    static Edge findTopTangent(Contour &L_contour,
                               Contour &R_contour,
                               Contour::reverse_iterator &L_it,
                               Contour::iterator &R_it
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
