#define GLFW_INCLUDE_GLU
#include "rasterization.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <random>
#include <GLFW/glfw3.h>
#include "triangulation.h"

#define LINE_COLOR GREEN
#define BACKGR_BRIGHTNESS 100
#define FOREGROUND_COLOR RED

const GLubyte background[4] = {BACKGR_BRIGHTNESS, BACKGR_BRIGHTNESS, BACKGR_BRIGHTNESS, 255};

Framebuffer::Framebuffer(const char *output_filename)
    : polygon(output_filename)
{
    size = 0;
    canvas = nullptr;
}

Framebuffer::Framebuffer(GLsizei width, GLsizei height, const char *output_filename)
    : polygon(output_filename)
{
    this->width = width;
    this->height = height;
    size = width*height;
    canvas = (Pixel *)calloc(sizeof(Pixel), size);
    memset(canvas, BACKGR_BRIGHTNESS, size * sizeof(Pixel));
}

Framebuffer::~Framebuffer()
{
    if (this->size) {
        free(this->canvas);
        size = 0;
        width = height = 0;
    }
}

void Framebuffer::reinitBuffer(GLsizei width, GLsizei height)
{
    this->width = width;
    this->height = height;
    GLsizei new_size = width*height;
    if (size == 0) {
        canvas = (Pixel *)calloc(sizeof(Pixel), new_size);
        size = new_size;
    }
    else if (size < new_size) {
        free(canvas);
        canvas = (Pixel *)calloc(sizeof(Pixel), new_size);
        size = new_size;
    } //Если кадр меньше исходного, буфер не трогаем

    clearCanvas();

}

void Framebuffer::clearCanvas()
{
    if (size) {
        memset(canvas, BACKGR_BRIGHTNESS, width * height * sizeof(Pixel));
    }
}

/*___________________________________*/

Pixel& Framebuffer::access(GLint x, GLint y)
{
    if (x >= width || y >= height)
    {
        printf("Framebuffer::operator[]: pixel {%d, %d} is out of range %dx%d\n", x, y, width, height);
        exit(1);
    }
    return canvas[width*y + x];
}

/* ************************************************************ */

/*COMPARATORS, COUT*/

inline bool Framebuffer::inBuffer(GLint x, GLint y)
{
    return x >= 0 && x < width && y >=0 && y < height;
}

bool Framebuffer::inBuffer(const Point& p)
{
    if (p.x < 0 || p.y < 0)
    {
        printf("[Framebuffer::inBuffer]: OUT OF RANGE %f, %f not in %d x %d\n", p.x, p.y, width, height);
        exit(1);
    }
    return inBuffer(p.x, p.y);
}


bool operator<(const Edge& e1, const Edge& e2)
{ // Comparison by the less point (less by coords)
    Point e1_left  = std::min(e1.a, e1.b);
    Point e1_right = std::max(e1.a, e1.b);
    Point e2_left  = std::min(e2.a, e2.b);
    Point e2_right = std::max(e2.a, e2.b);
    return e1_left < e2_left
            || (e1_left == e2_left && e1_right < e2_right);
//    return (e1.a < e1.b && (e1.a < e2.a || e1.a < e2.b))
//            || (e1.b < e1.a && (e1.b < e2.a || e1.b < e2.b));
//    return (e1.a == e2.a)
//            ? e1.b < e2.b
//            : e1.a < e2.a;
}


std::ostream& operator<<(std::ostream& os, const std::list<Edge>& edges)
{
    os << "EDGES: {\n";
    for(auto e: edges)
    {
        os << e << "\n";
    }
    return os << "}"<<std::endl;
}

std::ostream& operator<< (std::ostream& os, const Edge& e)
{
    return os << e.a << "--->" << e.b;
}

std::ostream& operator<<(std::ostream& os, const Framebuffer& F)
{
    os << "Framebuffer:\n " << F.width << "x" << F.height <<
          ";  size = " << F.size << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
//    return os << "(" << p.x << ")";
    return os << "(" << p.x << ", " << p.y << ")";
//    return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}

/* ___________________ADD___________________ */

void PointSet::addMousePoint(GLFWwindow *window) //Передача координат мыши c учетом отражения оси y
{
    double xpos, ypos;
    glfwGetCursorPos (window, &xpos, &ypos);
    int x = (int)floor(xpos);
    int y = (int)floor(ypos);
    int minz = 0;
    int maxz = 50;
    int z = (int)random()%(maxz-minz+1) + minz;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    //Координаты мыши в с.к. буфера кадра
    //Корректная работа в случае, когда "самодельный" буфер по размеру не совпадает с размером окна
    addPoint(x, height - y, z);
    printf("addPoint(%d,  %d, %d);\n", x, height - y, z);
    std::fprintf(output, "addPoint(%d,  %d, %d);\n", x, height - y, z);
    std::fflush(output);
    need_to_redraw = true;
}

void PointSet::addPoint(GLfloat x, GLfloat y, GLfloat z)
{
    auto f = std::find_if(verteces.begin(), verteces.end(),
                      [&](const Point &p) {return p.x == x && p.y == y;});
    if (f == verteces.end()) {
        verteces.emplace_back(x, y, z);
        return;
    } else {
        std::cout << "found same point " << *f << ", ignore it\n";
    }
}

void PointSet::addEdge(const Point& a, const Point& b)
{
    edges.emplace_back(a, b);
}

void PointSet::testPolygon()
{
    clear();
#define TEST_POLYGON

    addPoint(10.95,    0.37,   0);
    addPoint(9.72,     5.12,   4.11);
    addPoint(9.24,     5.32,   5.24);
    addPoint(7.33,     5.35,   6.15);
    addPoint(7.50,     6.13,   7.16);
    addPoint(5.62,     0.29,   5.27);
    addPoint(6.75,     0.31,   5.49);
    addPoint(7.10,     0.82,   4.53);
    addPoint(7.05,     1.97,   3.12);
    addPoint(10.03,    1.81,   2.15);

    for (Point &p: verteces) {
        p.x *= 50;
        p.y *= 50;
    }

//    addPoint(34,  263, 8);
////    addPoint(98,  336, 10);
//    addPoint(160,  265, 34);
////    addPoint(89,  258, 29);

//    addPoint(98,  336, 10);
//    addPoint(185,  408, 49);
//    addPoint(226,  338, 9);
//    addPoint(160,  265, 34);
//    addPoint(206,  255, 32);
//    addPoint(240,  276, 37);
//    addPoint(270,  365, 37);
//    addPoint(309,  378, 48);
//    addPoint(326,  322, 45);
//    addPoint(320,  292, 19);
//    addPoint(267,  272, 5);
//    addPoint(223,  234, 31);
//    addPoint(204,  194, 20);
//    addPoint(139,  199, 19);
//    addPoint(89,  258, 29);
//    addPoint(81,  349, 22);
//    addPoint(106,  424, 45);
//    addPoint(157,  450, 30);
////    addPoint(272,  457, 40);
////    addPoint(389,  405, 31);
////    addPoint(433,  330, 35);
////    addPoint(413,  198, 8);
////    addPoint(374,  157, 36);
//    addPoint(267,  146, 39);
//    addPoint(158,  137, 14);
////    addPoint(95,  181, 5);
////    addPoint(34,  263, 8);
////    addPoint(40,  368, 4);
////    addPoint(61,  466, 31);
////    addPoint(151,  510, 28);

//    addPoint(51,  377, 10);
//    addPoint(143,  417, 49);
//    addPoint(164,  365, 9);
//    addPoint(143,  284, 34);
//    addPoint(236,  287, 32);
//    addPoint(255,  344, 37);
//    addPoint(318,  457, 37);
//    addPoint(412,  354, 48);
//    addPoint(387,  309, 45);
//    addPoint(351,  273, 19);
//    addPoint(581,  241, 5);
//    addPoint(583,  414, 31);
//    addPoint(520,  442, 20);
//    addPoint(441,  480, 19);

//    addPoint(241,  376, 10);
//    addPoint(358,  473, 49);
//    addPoint(131,  461, 9);
//    addPoint(129,  352, 34);
//    addPoint(185,  310, 32);
//    addPoint(378,  300, 37);
//    addPoint(470,  371, 37);
//    addPoint(512,  475, 48);
//    addPoint(626,  345, 45);
//    addPoint(516,  232, 19);
//    addPoint(368,  190, 5);
//    addPoint(318,  195, 31);
//    addPoint(239,  222, 20);
//    addPoint(162,  230, 19);
//    addPoint(223,  179, 29);
//    addPoint(372,  122, 22);
//    addPoint(473,  138, 45);
//    addPoint(548,  146, 30);
//    addPoint(605,  195, 40);
//    addPoint(663,  275, 31);
//    addPoint(706,  376, 35);
//    addPoint(657,  463, 8);
//    addPoint(673,  222, 36);
//    addPoint(662,  165, 39);
//    addPoint(584,  129, 14);
//    addPoint(484,  100, 5);
//    addPoint(415,  96, 8);
//    addPoint(270,  90, 4);
//    addPoint(166,  87, 31);
//    addPoint(115,  134, 28);

//    addPoint(200,  422, 49);
//    addPoint(401,  328, 34);
//    addPoint(490,  318, 37);
//    addPoint(213,  240, 19);
//    addPoint(297,  322, 10);
//    addPoint(310,  372, 49);
//    addPoint(482,  158, 48);
//    addPoint(564,  237, 34);
//    addPoint(414,  250, 9);
//    addPoint(581,  336, 32);
//    addPoint(515,  419, 37);

//        addPoint(165,  333, 10);
//        addPoint(200,  422, 49);
//        addPoint(279,  338, 9);
//        addPoint(401,  328, 34);
//        addPoint(432,  427, 32);
//        addPoint(490,  318, 37);
//        addPoint(322,  287, 37);
//        addPoint(273,  465, 48);
//        addPoint(503,  502, 45);

//        addPoint(213,  240, 19);
//        addPoint(297,  322, 10);
//        addPoint(310,  372, 49);

//        addPoint(320,  148, 45);
//        addPoint(482,  158, 48);
//        addPoint(564,  237, 34);

//        addPoint(414,  250, 9);
//        addPoint(581,  336, 32);
//        addPoint(515,  419, 37);
//        addPoint(474,  300, 37);




//    addPoint(123,  372, 10);
//    addPoint(217,  402, 49);
//    addPoint(214,  341, 9);
//    addPoint(443,  332, 34);
//    addPoint(504,  429, 32);
//    addPoint(563,  377, 37);


//    addPoint(340,  286, 10);
//    addPoint(395,  384, 49);
//    addPoint(416,  313, 9);
//    addPoint(418,  273, 34);
//    addPoint(580,  265, 32);
//    addPoint(541,  356, 37);
//    addPoint(595,  415, 37);

//    addPoint(100, 370, 31);
//    addPoint(191, 356, 19);
//    addPoint(273, 365, 48);
//    addPoint(296, 283, 37);

//    addPoint(223, 255, 45);
//    addPoint(309, 256, 9);
//    addPoint(338, 258, 24);
//    addPoint(338, 258, 31);

//    addPoint(61, 368, 32);
//    addPoint(310, 544, 48);
//    addPoint(317, 455, 37);
//    addPoint(345, 206, 37);

//    addPoint(242, 382, 32);
//    addPoint(337, 394, 48);
//    addPoint(344, 457, 37);
//    addPoint(336, 537, 37);

    need_to_redraw = true;
}
void PointSet::clear()
{
    verteces.clear();
    edges.clear();
#ifndef TEST_POLYGON
    std::fprintf(output, "\n--------------------------\n");
#endif
//    y_max = 0; //ВСЕ РАВНО РАБОТАЕТ НА ТОМ, ЧТО СПИСОК АКТИВНЫХ РЕБЕР ПУСТ
}
/* ___________________PRINT___________________ */

void Framebuffer::loadBuf()
{
    if (size)
    {
        printPolygon();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // выравнивание
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, canvas);
    }
}


void Framebuffer::printVerteces()
{
    int rad = 2;
    int rad_x, rad_y;
    for (auto dot: polygon.verteces)
    {
        if(inBuffer(dot)){
            rad_x = std::min((int)dot.x, rad); //Расширение влево от мыши => можно выйти за границу только при x-i<0
            rad_y = std::min(height - (int)dot.y, rad); //Расширение вверх от мыши => может быть только y+j > height
            for (int i = 0; i < rad_x; ++i)
            {
                for (int j = 0; j < rad_y; ++j)
                {
                    access(dot.x - i, dot.y + j).set(RED);
                }
            }
        }
    }
}

void Framebuffer::printPolygon()
{
    clearCanvas();
#ifdef TEST_POLYGON
    polygon.testPolygon();
#endif
//    switch(polygon.verteces.size()){
//    case 0: //Пустое поле, нечего рисовать
//        return;
//    case 1: //Нарисовать жирную стартовую точку
//        printVerteces();
//        return;
//    case 2: //Отрисовать прямую
//        Bresenham(polygon.edges.front());
//        return;
//    }

    if (polygon.verteces.size() == 0) {
        return;
    }
    printVerteces();
//    BB bb(polygon.verteces);
//    std::cout << bb.orientation_to_string() << std::endl;
//    std::vector<Point> part1, part2;
//    PointSet::SplitType parts_num = PointSet::split(polygon.verteces, part1, part2);
//    if (parts_num == PointSet::SplitType::VERTICAL || parts_num == PointSet::SplitType::HORIZONTAL) {
//        std::cout << "PART1:\n";
//        for (const Point &p : part1) {
//            std::cout << p << "\n";
//        }
//        std::cout << "PART2:\n";
//        for (const Point &p : part2) {
//            std::cout << p << "\n";
//        }
//    }

    polygon.foregroundColor = polygon.contrast ? red : green;

    if (polygon.need_to_redraw) {
        polygon.edges.clear();
        while (polygon.verteces.size() < 100) {
            Triangulation triangulation = PointSet::triangulate(polygon.verteces);
            for (const Triangle &T: triangulation) {
                polygon.verteces.push_back(T.centerOfGravity());
            }
            polygon.need_to_redraw = false;
        }
        Triangulation triangulation = PointSet::triangulate(polygon.verteces);
        std::set<Edge> tr_edges = triangulation.edges();
        polygon.edges.insert(polygon.edges.begin(), tr_edges.begin(), tr_edges.end());
    }
    if (polygon.lined)
    {
        for(const auto& e: polygon.edges)
        {
            Bresenham(e);
        }
//        printf("printPolygon: %lu edges\n", polygon.edges.size());
    }
}

void Framebuffer::drawPoint(GLint x, GLint y)
{
    if (inBuffer(x, y)) {
        access(x, y).set(FOREGROUND_COLOR);
    }
}

void Framebuffer::drawPoint(GLint x, GLint y, GLubyte *v3color)
{
    if (inBuffer(x, y)) {
        access(x, y).setv3(v3color);
    }
}

void Framebuffer::drawPointPlusBackgr(GLint x, GLint y, const GLubyte *v3color, int alpha, const GLubyte *backgr_color)
{
    if (inBuffer(x, y)) {
        access(x, y).setPlusBackgrv3(v3color, alpha, backgr_color);
    }
}

void Framebuffer::drawLine(GLint y, GLint x1, GLint x2)
{
    if (x1 > x2) std::swap(x1, x2);
    if (x1 >= width /*|| y >= height*/) //Нечего рисовать
    {
        return;
    }
    if (x2 >= width) x2 = width - 1;

//    printf("*************[drawLine] : y = %d; from %d to %d\n", y, x1, x2);
    while(x1 <= x2)
    {
        access(x1, y).setv4(polygon.foregroundColor);
        ++x1;
    }
}

/* *********************************************** */
inline void shiftXY(int &x, int &y, int &error, int signX, int signY, int max_error, int delta_error)
{
    x += signX;
    if(error < max_error)
    {//Двигаться линейно, пока не накопилась максимальная ошибка
        error += delta_error;
    } else {
        y += signY;
        error -= max_error;
    }
}

void Framebuffer::Bresenham(const Edge& e)
{
    Bresenham(e.a.x, e.a.y, e.b.x, e.b.y);
}

void Framebuffer::Bresenham(GLint x1, GLint y1, GLint x2, GLint y2)
{
    int dX = std::abs(x2 - x1);
    int dY = std::abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1; //Изменение в зависимости от квадранта
    const int signY = y1 < y2 ? 1 : -1;
    bool swap = false;

    if( dY > dX){
        std::swap(dX, dY);
        swap = true;
    }
    int error = - dX;
    drawPoint(x1, y1, polygon.lineColor);
    while(x1 != x2 || y1 != y2) { //наивный алгоритм, где выбор изменяемых величин производится на месте
        error += 2*dY;
        if (swap) {
            y1 += signY;
        } else {
            x1 += signX;
        }
        if(error >= 0) {
            if (swap) {
                x1 += signX;
            } else {
                y1 += signY;
            }
            error -= 2*dX;
        }

        drawPoint(x1, y1, polygon.lineColor);
    }
}

bool Edge::intersect(const Point &a, const Point &b, const Point &c, const Point &d)
{
    auto area = [](const Point &a, const Point &b, const Point &c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    };
    auto intersect_1 = [](float a, float b, float c, float d) {
        if (a > b)  std::swap (a, b);
        if (c > d)  std::swap (c, d);
        return std::max(a,c) <= std::min(b,d);
    };
    return intersect_1 (a.x, b.x, c.x, d.x)      // bounding box intersetcion
            && intersect_1 (a.y, b.y, c.y, d.y)
            && area(a,b,c) * area(a,b,d) <= 0    // pseudoscalar multiplication
            && area(c,d,a) * area(c,d,b) <= 0;
}
