#include "triangulation.h"
#include <algorithm>
#include <set>
#include <map>



BB::BB(const std::vector<Point> &points) {
    std::set<float> x_set, y_set;
    const Point &start_point = points.front();
    min_x = max_x = start_point.x;
    min_y = max_y = start_point.y;
    for (const Point &p: points) {
        x_set.insert(p.x);
        y_set.insert(p.y);
    }
    min_x = *(x_set.begin());
    max_x = *(x_set.rbegin());
    min_y = *(y_set.begin());
    max_y = *(y_set.rbegin());
    if (x_set.size() < y_set.size()) {
        orientation = PointSet::SplitType::VERTICAL;
    } else if (x_set.size() > y_set.size()) {
        orientation = PointSet::SplitType::HORIZONTAL;
    } else {
        float x_len = max_x - min_x;
        float y_len = max_y - min_y;
        if (x_len < y_len) {
            orientation = PointSet::SplitType::VERTICAL;
        } else {
            orientation = PointSet::SplitType::HORIZONTAL;
        }
    }
}

BB::BB(const Triangle &tr)
{
    max_x = std::max(std::max(tr.a().x, tr.b().x), tr.c().x);
    max_y = std::max(std::max(tr.a().y, tr.b().y), tr.c().y);
    min_x = std::min(std::min(tr.a().x, tr.b().x), tr.c().x);
    min_y = std::min(std::min(tr.a().y, tr.b().y), tr.c().y);
    float x_len = max_x - min_x;
    float y_len = max_y - min_y;
    if (x_len < y_len) {
        orientation = PointSet::SplitType::VERTICAL;
    } else {
        orientation = PointSet::SplitType::HORIZONTAL;
    }
}

/*
3 cases:               .p
      /                \/              a
   c /\              a /\              /\
1)  /  \ .p      2)   /  \        3)  /.p\
   /____\_          _/____\_         /____\
  a      b          b      c        b      c

1) p is in angle cab (vector (p-a) lies between (c-a) and (b-a)) => add 1 new triangle pcb
2) p is not in any angle => add 2 new triangles pab & pca
3) p is in all angles => delete triangle abc and add 3 triangles pab, pbc, pca
*/
Triangulation triangulate_4(std::vector<Point> &verteces)
{
    auto between = [](const glm::vec3 &ap, const glm::vec3 &ab, const glm::vec3 &ac) {
        return Edge::pseudoscalar(ab, ap)*Edge::pseudoscalar(ap, ac) >= 0;
//        std::cout << glm::dot(glm::cross(ab, ap), glm::cross(ap, ac)) << std::endl;
//        return glm::dot(glm::cross(ab, ap), glm::cross(ap, ac)) >= 0;
    };

    auto is_inside = [](const glm::vec3 &u, const glm::vec3 &v) {
        return glm::dot(u, v) > 0;
//        return (u.x*v.x + u.y*v.y) > 0;
    };

    const Point &p = verteces[0];
    const Point &a = verteces[1];
    const Point &b = verteces[2];
    const Point &c = verteces[3];
    std::cout << "triangulate_4:" << std::endl;
    std::cout << "P = " << p << "; A = " << a
              << "B = " << b << "; C = " << c << std::endl;
    int count = 0;
    Triangle ABC(a, b, c), PAB, PCA, PBC;
    std::cout << "between(p-a, b-a, c-a) = ";
    if (between(p-a, b-a, c-a)) {
        glm::vec3 med = glm::normalize(b-a) + glm::normalize(c-a);
        if (is_inside(p-a, med)) { // case 1 or 3
            std::cout << "between(p-a, b-a, c-a)" << std::endl;
            ++count;
            PBC.set(p, b, c);
            Triangle::setNeigbours(ABC, 1, PBC, 1);
        } else {  // case 2 => return 3 triangles
            PAB.set(p, a, b);
            PCA.set(p, c, a);
            Triangle::setNeigbours(ABC, 2, PCA, 1); // share CA
            Triangle::setNeigbours(PCA, 2, PAB, 0); // share PA
            Triangle::setNeigbours(ABC, 1, PAB, 1); // share AB
            return Triangulation {ABC, PAB, PCA};
        }
    }
    std::cout << "between(p-b, a-b, c-b) = ";
    if (between(p-b, a-b, c-b)) {
        glm::vec3 med = glm::normalize(a-b) + glm::normalize(c-b);
        if (is_inside(p-b, med)) { // case 1 or 3
            std::cout << "between(p-b, a-b, c-b)" << std::endl;
            ++count;
            PCA.set(a, p, c);
            Triangle::setNeigbours(ABC, 2, PCA, 2); // share CA
        } else {
            std::cout << "out(p-b, a-b, c-b)" << std::endl;
            PAB.set(p, a, b);
            PBC.set(p, b, c);
            Triangle::setNeigbours(ABC, 1, PBC, 1); // share BC
            Triangle::setNeigbours(PBC, 0, PAB, 2); // share PB
            Triangle::setNeigbours(ABC, 0, PAB, 1); // share AB
            return Triangulation {ABC, PAB, PBC};
        }
    }
    std::cout << "between(p-c, a-c, b-c) = ";
    if (between(p-c, a-c, b-c)) {
        glm::vec3 med = glm::normalize(a-c) + glm::normalize(b-c);
        if (is_inside(p-c, med)) { // case 1 or 3
            std::cout << "between(p-c, a-c, b-c)" << std::endl;
            ++count;
            PAB.set(p, a, b);
            Triangle::setNeigbours(ABC, 0, PAB, 1); // share AB
        } else {
            PBC.set(p, b, c);
            PCA.set(p, c, a);
            Triangle::setNeigbours(ABC, 1, PBC, 1); // share BC
            Triangle::setNeigbours(PBC, 2, PCA, 0); // share PC
            Triangle::setNeigbours(ABC, 2, PCA, 1); // share AC
            return Triangulation {ABC, PBC, PCA};
        }
    }
    // Choose the right case
    if (count == 1) { // case 1 => 2 triangles
        // Check Delaunay condition
        if (ABC.circumCircleContains(p)) {
            if (ABC.neighbour[0] != nullptr) {
                PCA.set(p, c, a);
                PBC.set(p, c, b);
                PCA.neighbour[0] = &PBC;
                PBC.neighbour[0] = &PCA;
                std::cout << "count = 1, neighbour = 0";
                return Triangulation {PCA, PBC};
            } else if (ABC.neighbour[1] != nullptr) {
                PAB.set(p, a, b);
                PCA.set(p, a, c);
                PAB.neighbour[0] = &PCA;
                PCA.neighbour[0] = &PAB;
                return Triangulation {PAB, PCA};
            } else {
                PAB.set(p, b, a);
                PBC.set(p, b, c);
                PAB.neighbour[0] = &PBC;
                PBC.neighbour[0] = &PAB;
                return Triangulation {PAB, PBC};
            }
        } else { // the pair satisfies Delaunay's condition
            for (int i = 0; i < 3; ++i) {
                if (ABC.neighbour[i] != nullptr) {
                    return Triangulation {ABC, *ABC.neighbour[i]};
                }
            }
        } // ABC.circumCircleContains(p)
    } // if (count == 1) // case 1 => 2 triangles
    if (count != 3) {
        std::cout << "ACHTUNG: unhandled case, count = " << count << std::endl;
        throw std::exception();
    }
    // count == 3 => case 3, swap p with any point and get case 2
    std::swap(verteces.front(), verteces.back());
    return triangulate_4(verteces);
}

Triangulation merge(Triangulation &&part1, Triangulation &&part2, PointSet::SplitType orientation)
{
    Triangulation result;
    result.reserve(part1.size() + part2.size());
    result.insert(result.begin(), part1.begin(), part1.end());
    result.insert(result.end(),   part2.begin(), part2.end());
    return result;
}

Triangulation PointSet::triangulate(std::vector<Point> &verteces)
{
    switch (verteces.size()) {
    case 1: //Algorithm assumes that we can split point set to subsets of 3 and 4 points
    case 2:
    case 5:
        return Triangulation();
    case 3:
        return Triangulation {Triangle(verteces[0], verteces[1], verteces[2])};
    case 4:
        return triangulate_4(verteces);
    }
    std::vector<Point> part1;
    std::vector<Point> part2;
    PointSet::SplitType orientation = PointSet::split(verteces, part1, part2);
    if (orientation == PointSet::SplitType::SINGLE) {
        return Triangulation(triangulate(part1));
    }
triangulate(part1).contour();
triangulate(part2).contour();
    return merge(std::move(triangulate(part1)), std::move(triangulate(part2)), orientation);
}

PointSet::SplitType PointSet::split(std::vector<Point> &verteces
                    , std::vector<Point> &part1, std::vector<Point> &part2) {
    switch (verteces.size()) {
    case 3:
    case 4:
        part1 = verteces;
        part2.clear();
        return PointSet::SplitType::SINGLE;
    case 1:
    case 2:
    case 5: //Algorithm assume, that we can split triangulation for subsets of 3 or 4 points
        part1.clear();
        part2.clear();
        return PointSet::SplitType::ZERO;
    }
    BB bounding_box(verteces);
    PointSet::SplitType orientation = bounding_box.getOrientation();
    if (orientation == PointSet::SplitType::HORIZONTAL) {
        std::sort(verteces.begin(), verteces.end(), Point::lessByX);
    } else {
        std::sort(verteces.begin(), verteces.end(), Point::lessByY);
    }
    size_t N = verteces.size();
    size_t part_size = 0;
    if (N >= 12 || N == 8) { // Split to equal parts: floor(N/2) + ceil(N/2)
        part_size = verteces.size()/2; // floor(N/2)
    } else { // N < 12 : split to 3 + N-3
        part_size = 3;
    }
    std::vector<Point>::iterator pivot = verteces.begin() + part_size;
    part1.clear();
    part2.clear();
    part1.reserve(part_size);
    part2.reserve(N - part_size);
    part1.assign(verteces.begin(), pivot);
    part2.assign(pivot, verteces.end());
    return orientation;
}

bool Triangle::sharesVertexWith(Triangle const &triangle) const {
    if ( a().x == triangle.a().x && a().y == triangle.a().y ) return true;
    if ( a().x == triangle.b().x && a().y == triangle.b().y ) return true;
    if ( a().x == triangle.c().x && a().y == triangle.c().y ) return true;
    if ( b().x == triangle.a().x && b().y == triangle.a().y ) return true;
    if ( b().x == triangle.b().x && b().y == triangle.b().y ) return true;
    if ( b().x == triangle.c().x && b().y == triangle.c().y ) return true;
    if ( c().x == triangle.a().x && c().y == triangle.a().y ) return true;
    if ( c().x == triangle.b().x && c().y == triangle.b().y ) return true;
    if ( c().x == triangle.c().x && c().y == triangle.c().y ) return true;
    return false;
}

bool Triangle::circumCircleContains(Point const &v) const {
    const Point &A = a();
    const Point &B = b();
    const Point &C = c();
    float qq = (A.x * A.x) + (A.y * A.y);
    float qqv = (B.x * B.x) + (B.y * B.y);
    float qqc = (C.x * C.x) + (C.y * C.y);
    glm::vec2 M = glm::vec2(((qq * (C.y - B.y) + qqv * (A.y - C.y) + qqc * (B.y - A.y))
                             / (A.x * (C.y - B.y) + B.x * (A.y - C.y) + C.x * (B.y - A.y))/ 2.f),
                            ((qq * (C.x - B.x)+ qqv * (A.x - C.x) + qqc * (B.x - A.x))
                             / (A.y * (C.x - B.x)+ B.y * (A.x - C.x)+ C.y * (B.x - A.x))/ 2.f));
    float R = glm::distance(glm::vec2(A.x, A.y), M);
    float D = glm::distance(M, glm::vec2(v.x, v.y));
    return D <= R;
}

bool Triangle::operator==(Triangle const &rhs) const {
    return	(a() == rhs.a() || a() == rhs.b() || a() == rhs.c()) &&
            (b() == rhs.a() || b() == rhs.b() || b() == rhs.c()) &&
            (c() == rhs.a() || c() == rhs.b() || c() == rhs.c());
}

Point calcNormal(std::vector<Triangle> &triangles, Triangle triangle, Point a) {
    Point totalNormal;
    for (size_t j = 0; j < triangles.size(); j++){
        Point normal;
        Edge chosenEdge;
        bool found = false;
        if (triangles[j].a() == a
            && !(triangle == triangles[j])){
            normal = glm::cross((Point)triangles[j].edges[2], (Point)triangles[j].edges[0]);
            chosenEdge = triangles[j].edges[1];
            found = true;
        }

        if (triangles[j].b() == a
            && !(triangle == triangles[j])){
            normal = glm::cross((Point)triangles[j].edges[0], (Point)triangles[j].edges[1]);
            chosenEdge = triangles[j].edges[2];
            found = true;
        }

        if (triangles[j].c() == a
            && !(triangle == triangles[j])){
            normal = glm::cross((Point)triangles[j].edges[1], (Point)triangles[j].edges[2]);
            chosenEdge = triangles[j].edges[0];
            found = true;
        }

        if (found){
            float mm = 0.5 * (glm::length(triangles[j].a() - triangles[j].b()) +
                             glm::length(triangles[j].b() - triangles[j].c()) +
                             glm::length(triangles[j].c() - triangles[j].a()));
            float KK = sqrt(mm * (mm - glm::length(triangles[j].a() - triangles[j].b()))
                            * (mm - glm::length(triangles[j].b() - triangles[j].c()))
                            * (mm - glm::length(triangles[j].c() - triangles[j].a())));
            float weight = 2 * KK / glm::length(chosenEdge.a - chosenEdge.b) * log(mm / (mm-glm::length(chosenEdge.a - chosenEdge.b)));
            normal *= weight;
            totalNormal = totalNormal + normal;
        }
    }
    return totalNormal;
}



Point interpolate(std::vector<Triangle> &triangles, Triangle triangle, Point a, Point b, float param){
    Point Da = glm::cross({b.x - a.x, b.y - a.y, b.z - a.z}, calcNormal(triangles, triangle, a));
    Point Db = glm::cross(Point(b.x - a.x, b.y - a.y, b.z - a.z), (calcNormal(triangles, triangle, b)));
    float length = glm::length(a - b);
    float t = param * length;
    float x = a.x + (b.x - a.x) * t / length;
    float y = a.y + (b.y - a.y) * t / length;
    float c0 = a.z;
    float sqrt1 = sqrt(Da.x * Da.x + Da.y * Da.y);
    float sqrt2 = sqrt(Db.x * Db.x + Db.y * Db.y);
    float za = (sqrt1 > 0)? Da.z / sqrt1 : 0;
    float zb = (sqrt2 > 0)? Db.z / sqrt2 : 0;
    float c1 = za;
    float c2 = (3* (b.z - a.z) - (2 * za + zb) * length) / length / length;
    float c3 = ((za + zb)*length - 2*(b.z - a.z))/length/length/length;
    float z = c3 * t * t * t + c2 * t * t + c1 * t + c0;
    return Point(x, y, z);
}


std::vector<Triangle> addDetail(std::vector<Triangle> &triangles){
    std::vector<Triangle> newtriangles;
    for (size_t i = 0; i < triangles.size(); i++){
        Point ab = interpolate(triangles, triangles[i], triangles[i].a(), triangles[i].b(), 0.5);
        Point bc = interpolate(triangles, triangles[i], triangles[i].b(), triangles[i].c(), 0.5);
        Point ac = interpolate(triangles, triangles[i], triangles[i].a(), triangles[i].c(), 0.5);
        newtriangles.push_back(Triangle(triangles[i].a(), ab, ac));
        newtriangles.push_back(Triangle(ab, bc, ac));
        newtriangles.push_back(Triangle(ab, triangles[i].b(), bc));
        newtriangles.push_back(Triangle(ac, bc, triangles[i].c()));
    }
    return newtriangles;
}

std::set<Edge> Triangulation::edges()
{
    std::set<Edge> edges;
    for (const Triangle &tr: *this) {
        edges.insert(tr.edges, tr.edges + 3);
        std::cout << "Triangulation::edges(): "
                  << tr.edges[0] << " " << tr.edges[1] << " " << tr.edges[2] << std::endl;
    }
    return edges;
}

float Triangle::maxCos() const
{
    float cosA = glm::dot(guides[0], guides[1]);
    float cosB = glm::dot(guides[1], guides[2]);
    float cosC = glm::dot(guides[2], guides[0]);
    return std::max(cosA, std::max(cosB, cosC));
}

bool Triangle::isInside(const Point &p) const
{
    bool sign1 = Edge::pseudoscalar(p-a(), b()-a()) > 0;
    bool sign2 = Edge::pseudoscalar(p-b(), c()-b()) > 0;
    bool sign3 = Edge::pseudoscalar(p-c(), c()-a()) > 0;
    if (sign1 == sign2 && sign2 == sign3) {
        return true;
    }
    /// TODO: process special cases, when vectors are collinear
    return false;
}

void Triangle::set(const Point &a, const Point &b, const Point &c)
{
    points[0] = a;
    points[1] = b;
    points[2] = c;
    edges[0] = Edge(a, b);
    edges[1] = Edge(b, c);
    edges[2] = Edge(c, a);
    guides[0] = glm::normalize(b-a);
    guides[1] = glm::normalize(c-b);
    guides[2] = glm::normalize(a-c);
}

const char* BB::orientation_to_string()
{
    switch (orientation) {
    case PointSet::SplitType::ZERO:
        return "ZERO";
    case PointSet::SplitType::SINGLE:
        return "ONE";
    case PointSet::SplitType::VERTICAL:
        return "VERTICAL";
    case PointSet::SplitType::HORIZONTAL:
        return "HORIZONTAL";
    default: return "undefined";
    }
}

Triangle &Triangle::make_CCW()
{
    const Point &a = points[0];
    const Point &b = points[1];
    const Point &c = points[2];
    if (Edge::pseudoscalar(b-a, c-a) > 0) {
        return *this;
    }
    std::swap(points[1], points[2]);
    set(points[0], points[1], points[2]);
    std::swap(neighbour[0], neighbour[2]);
    return *this;
}

Triangle *Triangle::getNext(const Point &p, Point &next_p, Orientation dir)
{
    if (dir == Orientation::CCW) {
        for (int i = 0; i < 3; ++i) {
            if (points[i] == p) {
                next_p = points[(i + 1) % 3];
                return (neighbour[i] == nullptr) ? this : neighbour[i];
            }
        }
    } else {
        for (int i = 2; i >= 0; --i) {
            if (points[i] == p) {
                next_p = points[(i-1) % 3];
                Triangle *next_neighbour = neighbour[(i-1) % 3];
                return (next_neighbour == nullptr) ? this : next_neighbour ;
            }
        }
    }
    return nullptr; // Point does not belong to the triangle
}

Point Triangle::maxPointByX() const
{
    Point P = points[0];
    if (points[1].x > P.x) P = points[1];
    if (points[2].x > P.x) P = points[2];
    return P;
}

Point Triangle::maxPointByY() const
{
    Point P = points[0];
    if (points[1].y > P.y) P = points[1];
    if (points[2].y > P.y) P = points[2];
    return P;
}

Point Triangle::minPointByX() const
{
    Point P = points[0];
    if (points[1].x < P.x) P = points[1];
    if (points[2].x < P.x) P = points[2];
    return P;
}

Point Triangle::minPointByY() const
{
    Point P = points[0];
    if (points[1].y < P.y) P = points[1];
    if (points[2].y < P.y) P = points[2];
    return P;
}

Edge Triangulation::findTopTangent(Triangulation &L, Triangulation &R,
                                          PointSet::SplitType orientation)
{
    Triangle::Orientation L_circumvention, R_circumvention;
    Point P0;
    Point P1;
    Triangle T0;
    Triangle T1;
    using edge_tri_pair = std::pair<Edge &, Triangle &>;
    if (orientation == PointSet::SplitType::HORIZONTAL) { // find top
//        std::vector<edge_tri_pair> L_contour(L.contour());
//        std::vector<edge_tri_pair> R_contour(R.contour());
        // Find the most right point of left part and the most left point of the right one
        // and move up until contour is convex

//        edge_tri_pair left_edge = std::max_element(L_contour.begin(), L_contour.end(), less_by_right_bound);
//        edge_tri_pair right_edge = std::;
        auto less_by_right_bound = [](const edge_tri_pair &X, const edge_tri_pair &Y)
        {
            return (BB(X.second).max_x < BB(Y.second).max_x);
        };
        auto less_by_left_bound = [](const edge_tri_pair &X, const edge_tri_pair &Y)
        {
            return (BB(X.second).min_x < BB(Y.second).min_x);
        };

//        L_circumvention = Triangle::Orientation::CCW;
//        R_circumvention = Triangle::Orientation::CW;
//        std::sort(L.begin(), L.end(), less_by_right_bound);
//        std::sort(R.begin(), R.end(), less_by_left_bound);
//        T0 = L.back();  // the most right point of left part
//        T1 = R.front(); // the most left point of right part
//        P0 = T0.maxPointByX();
//        P1 = T1.minPointByX();
//        Point nextP0, nextP1;
//        Triangle nextT0, nextT1;
//        nextT0 = *T0.getNext(P0, nextP0, L_circumvention);
//        nextT1 = *T1.getNext(P1, nextP1, R_circumvention);
//        while(Edge::pseudoscalar(P1-P0, ))
    } else {
        auto less_by_min_x = [](const Triangle &X, const Triangle &Y) //find left
        {
            return (BB(X).min_x < BB(Y).min_x);
        };
        std::sort(L.begin(), L.end(), less_by_min_x);
        std::sort(R.begin(), L.end(), less_by_min_x);
    }

    return Edge(L.front().a(), R.front().a());
}

std::vector<EdgeTriPair> Triangulation::contour(Point::Comparator compare)
{
    // std::map: search, removal, and insertion operations have logarithmic complexity
    // tie edge with its start point
    std::map<Point, EdgeTriPair, Point::Comparator> edge_set(compare);
    for (Triangle &T : *this) {
        for (int i = 0; i < 3; ++i) {
            if (T.neighbour[i] == nullptr) {
                edge_set.emplace(T.edges[i].a, EdgeTriPair(&(T.edges[i]), &T));
            }
        }
    }
    std::vector<EdgeTriPair> result;
    result.reserve(edge_set.size());
    // find start point of contour with point_comp condition
    result.push_back(edge_set.begin()->second);
    Point *next_point = nullptr;
    // for every edge get its end point and push next the edge starting with that point
    while (result.size() != edge_set.size()) {
        next_point = &result.back().edge->b;
        result.push_back(edge_set[*next_point]);
    }
    std::cout << "Triangulation::contour:" << std::endl;
    for (auto &p: result) {
        std::cout << *p.edge << std::endl;
    }

    return result;
}
