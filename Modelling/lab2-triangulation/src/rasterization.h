#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include <glm/glm.hpp>

#define WHITE_A 255, 255, 255, 255
#define BLACK_A 0,    0,   0,  255
#define RED_A   255, 0,   0,   255
#define GREEN_A 25,  255, 25,  255
#define BLUE_A  0,   0,   255, 255
#define RED 255, 0, 0
#define GREEN   25, 255, 25
#define BLUE 0, 0, 255
#define WHITE 255, 255, 255

#define BLACK 0, 0, 0

static GLubyte red[4] = {RED_A};
static GLubyte green[4] = {GREEN_A};
//static GLubyte blue[4] = {BLUE_A};
//static GLubyte white[4] = {WHITE_A};
//static GLubyte black[4] = {BLACK_A};

struct Framebuffer;

#pragma pack(push, 1)
struct Pixel{
    GLubyte r = 255, g = 255, b = 255, alpha = 255; //GLubyte == 8-bit unsigned integer
    void set(u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t a) {
        float alpha = (float)a/255.f;
        this->r += (int)roundf((float)(r - this->r)*alpha);
        this->g += (int)roundf((float)(g - this->g)*alpha);
        this->b += (int)roundf((float)(b - this->b)*alpha);
        this->alpha = a;
    }

    void set(u_int8_t r, u_int8_t g, u_int8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->alpha = 255;
    }

    void setv3(const u_int8_t *v) {
        set(v[0], v[1], v[2]);
    }

    void setv4(const u_int8_t *v) {
        set(v[0], v[1], v[2], v[3]);
    }

    void setPlusBackgrv4(const u_int8_t *v, const u_int8_t *back) {
        setPlusBackgrv3(v, v[3], back);
    }
    void setPlusBackgrv3(const u_int8_t *v, u_int8_t alpha, const u_int8_t *back)
    {
        float a = (float)alpha/255.f;
        this->r = back[0]+(int)roundf((float)(v[0] - back[0])*a);
        this->g = back[1]+(int)roundf((float)(v[1] - back[1])*a);
        this->b = back[2]+(int)roundf((float)(v[2] - back[2])*a);
        this->alpha = alpha;
    }
};
#pragma pack(pop)

class Edge;

class Point : public glm::vec3 {
public:
    static bool lessByX(const Point &a, const Point &b) {
            return a.x < b.x || (a.x == b.x && (a.y < b.y || (a.y == b.y && a.z < b.z)));
    }
    static bool lessByY(const Point &a, const Point &b) {
            return a.y < b.y || (a.y == b.y && (a.x < b.x || (a.x == b.x && a.z < b.z)));
    }
    static bool greaterByX(const Point &a, const Point &b) {
            return a.x > b.x || (a.x == b.x && (a.y > b.y || (a.y == b.y && a.z > b.z)));
    }
    static bool greaterByY(const Point &a, const Point &b) {
            return a.y > b.y || (a.y == b.y && (a.x > b.x || (a.x == b.x && a.z > b.z)));
    }

    using glm::vec3::vec3;

    using Comparator = bool (*)(const Point &, const Point &);
    bool operator==(Point const &v2) const {
        return (x == v2.x) && (y == v2.y) && (z == v2.z);
    }
    bool operator<(Point const &v2) const {
        return lessByX(*this, v2);
    }
};

std::ostream& operator<<(std::ostream& os, const Point& p);

class Edge {
    public:
        Edge(){}
        Edge(const Point &a, const Point &b): a(a), b(b) {}

        Edge(const Edge &e) : a(e.a), b(e.b){}

        Edge &set(const Point &a, const Point &b) {
            this->a = a;
            this->b = b;
            return *this;
        }

        Point a;
        Point b;

        operator glm::vec3() const {
            return b - a;
        }

        double length() const {
            return glm::length(b - a);
        }

        void reverse();

        bool operator == (Edge const &rhs) const {
            return 	(a == rhs.a && b == rhs.b) ||
                    (a == rhs.b && b == rhs.a);
        }

        bool operator != (Edge const &rhs) const {
            return ! (*this == rhs);
        }

        static bool intersect(const Point &a, const Point &b, const Point &c, const Point &d);
        static float cross2d(const glm::vec3 &u, const glm::vec3 &v) {
            return u.x*v.y - v.x*u.y;
        }

        static bool less180(const glm::vec3 &u, const glm::vec3 &v) {
            return cross2d(u, v) > 0;
        }
};


bool operator<(const Edge& a, const Edge& b); //Функция для сортировки перед заливкой: сравнение по y
std::ostream& operator<< (std::ostream& os, const std::list<Edge>& edges);
std::ostream& operator<< (std::ostream& os, const Edge& x);

struct ActiveEdge
{
    int y_max, x_start;
    double x_current, dx;
    ActiveEdge() {}
    ActiveEdge(int y_max, int x_start, double dx):
        y_max(y_max), x_start(x_start), x_current((double)x_start), dx(dx){}
};

class Triangulation;

struct PointSet{

    enum class SplitType {
        ZERO, SINGLE, VERTICAL, HORIZONTAL
    };

    std::vector<Point> verteces;
    std::FILE *output;
    std::list<Edge> edges;
    int y_max = 0;
    bool lined = true;
    bool concentrate = false;
    bool need_to_redraw = false;

    GLubyte *lineColor = green;
    GLubyte *foregroundColor = red;

    void clear();
    void testPolygon();
    void addPoint(GLfloat x, GLfloat y, GLfloat z);
    void addMousePoint(GLFWwindow* window);
    void addEdge(const Point& a, const Point& b);
    static SplitType split(std::vector<Point> &verteces
                     , std::vector<Point> &part1, std::vector<Point> &part2
                     );
    static Triangulation triangulate(std::vector<Point> &verteces);
    PointSet(const char *output_filename) {
        output = std::fopen(output_filename, "a"); // открыть файл для записи в конец файла (append)
        std::fputs("\n----------------------\n", output);
        std::fflush(output);

    }
    ~PointSet() {
        std::fflush(output);
        std::fclose(output);
    }
};



struct Framebuffer {
    GLsizei width = 0;
    GLsizei height = 0; //Начало системы координат - в левом нижнем углу
    GLsizei size = 0;
    PointSet polygon;
    Pixel *canvas = nullptr;

    Framebuffer(const char *output_filename);
    std::list<ActiveEdge> AET; //Active Edge Table - Список Активных Ребер
    Framebuffer(GLsizei width, GLsizei height, const char *output_filename);
    ~Framebuffer();
    void reinitBuffer(GLsizei width, GLsizei height);

    void loadBuf();
    Pixel& access(GLint x, GLint y);
    void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    void printPolygon();
    void drawPoint(GLint x, GLint y);
    void drawPoint(GLint x, GLint y, GLubyte *v3color);
    void drawPoint(GLint x, GLint y, GLfloat z);
    void drawPointPlusBackgr(GLint x, GLint y, const GLubyte *v3color, int alpha, const GLubyte *backgr_color);
    void drawLine(GLint y, GLint x1, GLint x2);
    void printVerteces();
    void refresh_AET();
    inline void updateX();
    void fillLines();

    void Bresenham(GLint x1, GLint y1, GLfloat z1, GLint x2, GLint y2, GLfloat z2);
    void Bresenham(const Edge& e);
    void clearCanvas();
    void deletePoint();
    bool inBuffer(GLint x, GLint y);
    bool inBuffer(const Point& p);
};

std::ostream& operator<<(std::ostream& os, const Framebuffer& F);

#endif // RASTERIZATION_H
