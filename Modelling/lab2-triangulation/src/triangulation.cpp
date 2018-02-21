#include "triangulation.h"

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
