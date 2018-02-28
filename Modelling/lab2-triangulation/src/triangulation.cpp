#include "triangulation.h"
#include <algorithm>
#include <set>



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
    printf("BB: x = [%f; %f], y = [%f; %f]\n",  min_x, max_x, min_y, max_y);
    if (x_set.size() < y_set.size()) {
        orientation = Orientation::VERTICAL;
    } else if (x_set.size() > y_set.size()) {
        orientation = Orientation::HORIZONTAL;
    } else {
        float x_len = max_x - min_x;
        float y_len = max_y - min_y;
        if (x_len < y_len) {
            orientation = Orientation::VERTICAL;
        } else {
            orientation = Orientation::HORIZONTAL;
        }
    }
}

Triangulation triangulate_4(std::vector<Point> verteces)
{
    return Triangulation();
}

Triangulation merge(Triangulation &&part1, Triangulation &&part2)
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
    int parts_num = PointSet::split(verteces, part1, part2);
    if (parts_num == 1) {
        return Triangulation(triangulate(part1));
    }
    return merge(std::move(triangulate(part1)), std::move(triangulate(part2)));
}

int PointSet::split(std::vector<Point> &verteces
                    , std::vector<Point> &part1, std::vector<Point> &part2) {
    switch (verteces.size()) {
    case 3:
    case 4:
        part1 = verteces;
        part2.clear();
        return 1;
    case 1:
    case 2:
    case 5: //Algorithm assume, that we can split triangulation for subsets of 3 or 4 points
        part1.clear();
        part2.clear();
        return 0;
    }
    BB bounding_box(verteces);
    if (bounding_box.getOrientation() == BB::Orientation::HORIZONTAL) {
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
    return 2;
}

bool Triangle::sharesVertexWith(Triangle const &triangle) const {
    if ( a.x == triangle.a.x && a.y == triangle.a.y ) return true;
    if ( a.x == triangle.b.x && a.y == triangle.b.y ) return true;
    if ( a.x == triangle.c.x && a.y == triangle.c.y ) return true;
    if ( b.x == triangle.a.x && b.y == triangle.a.y ) return true;
    if ( b.x == triangle.b.x && b.y == triangle.b.y ) return true;
    if ( b.x == triangle.c.x && b.y == triangle.c.y ) return true;
    if ( c.x == triangle.a.x && c.y == triangle.a.y ) return true;
    if ( c.x == triangle.b.x && c.y == triangle.b.y ) return true;
    if ( c.x == triangle.c.x && c.y == triangle.c.y ) return true;
    return false;
}

bool Triangle::circumCircleContains(Point const &v) const {
    float qq = (a.x * a.x) + (a.y * a.y);
    float qqv = (b.x * b.x) + (b.y * b.y);
    float qqc = (c.x * c.x) + (c.y * c.y);
    glm::vec2 C = glm::vec2(((qq * (c.y - b.y) + qqv * (a.y - c.y) + qqc * (b.y - a.y))
                             / (a.x * (c.y - b.y) + b.x * (a.y - c.y) + c.x * (b.y - a.y))/ 2.f),
                            ((qq * (c.x - b.x)+ qqv * (a.x - c.x) + qqc * (b.x - a.x))
                             / (a.y * (c.x - b.x)+ b.y * (a.x - c.x)+ c.y * (b.x - a.x))/ 2.f));
    float R = glm::distance(glm::vec2(a.x, a.y), C);
    float D = glm::distance(C, glm::vec2(v.x, v.y));
    return D <= R;
}

bool Triangle::operator==(Triangle const &rhs) const {
    return	(a == rhs.a || a == rhs.b || a == rhs.c) &&
            (b == rhs.a || b == rhs.b || b == rhs.c) &&
            (c == rhs.a || c == rhs.b || c == rhs.c);
}

Point calcNormal(std::vector<Triangle> &triangles, Triangle triangle, Point a) {
    Point totalNormal;
    for (size_t j = 0; j < triangles.size(); j++){
        Point normal;
        Edge chosenEdge;
        bool found = false;
        if (triangles[j].a == a
            && !(triangle == triangles[j])){
            normal = glm::cross((Point)triangles[j].edge[2], (Point)triangles[j].edge[0]);
            chosenEdge = triangles[j].edge[1];
            found = true;
        }

        if (triangles[j].b == a
            && !(triangle == triangles[j])){
            normal = glm::cross((Point)triangles[j].edge[0], (Point)triangles[j].edge[1]);
            chosenEdge = triangles[j].edge[2];
            found = true;
        }

        if (triangles[j].c == a
            && !(triangle == triangles[j])){
            normal = glm::cross((Point)triangles[j].edge[1], (Point)triangles[j].edge[2]);
            chosenEdge = triangles[j].edge[0];
            found = true;
        }

        if (found){
            float mm = 0.5 * (glm::length(triangles[j].a - triangles[j].b) +
                             glm::length(triangles[j].b - triangles[j].c) +
                             glm::length(triangles[j].c - triangles[j].a));
            float KK = sqrt(mm * (mm - glm::length(triangles[j].a - triangles[j].b))
                            * (mm - glm::length(triangles[j].b - triangles[j].c))
                            * (mm - glm::length(triangles[j].c - triangles[j].a)));
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
        Point ab = interpolate(triangles, triangles[i], triangles[i].a, triangles[i].b, 0.5);
        Point bc = interpolate(triangles, triangles[i], triangles[i].b, triangles[i].c, 0.5);
        Point ac = interpolate(triangles, triangles[i], triangles[i].a, triangles[i].c, 0.5);
        newtriangles.push_back(Triangle(triangles[i].a, ab, ac));
        newtriangles.push_back(Triangle(ab, bc, ac));
        newtriangles.push_back(Triangle(ab, triangles[i].b, bc));
        newtriangles.push_back(Triangle(ac, bc, triangles[i].c));
    }
    return newtriangles;
}

std::set<Edge> Triangulation::edges()
{
    std::set<Edge> edges;
    for (const Triangle &tr: *this) {
        edges.insert(tr.edge, tr.edge + 3);
        std::cout << "Triangulation::edges(): "
                  << tr.edge[0] << " " << tr.edge[1] << " " << tr.edge[2] << std::endl;
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
    float sign1 = glm::cross(p-a, b-a);
    float sign2 = glm::cross(p-b, c-b);
    float sign3 = glm::cross(p-c, c-a);
    if ((sign1 > 0 && sign2 > 0 && sign3 > 0)
         || (sign1 < 0 && sign2 < 0 && sign3 < 0)) {
        return true;
    }
    /// TODO: process special cases, when vectors are collinear
    return false;
}
