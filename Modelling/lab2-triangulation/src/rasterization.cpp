#define GLFW_INCLUDE_GLU
#include "rasterization.h"

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

Framebuffer::Framebuffer()
{
    size = 0;
    canvas = nullptr;
}

Framebuffer::Framebuffer(GLsizei width, GLsizei height)
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
    return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
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
    printf("PUSH %d,  %d, %d\n", x, height - y, z);
    need_to_redraw = true;
}

void PointSet::addPoint(GLint x, GLint y, GLint z)
{
    verteces.emplace_back(x, y, z);

}

void PointSet::addEdge(const Point& a, const Point& b)
{
    edges.emplace_back(a, b);
}

void PointSet::clear()
{
    verteces.clear();
    edges.clear();
//    y_max = 0; //ВСЕ РАВНО РАБОТАЕТ НА ТОМ, ЧТО СПИСОК АКТИВНЫХ РЕБЕР ПУСТ
}

void PointSet::testPolygon()
{
    clear();

    addPoint(216, 451, 10);
    addPoint(520, 222, 9);
    addPoint(520, 222, 49);
    addPoint(306, 185, 34);

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

//    polygon.testPolygon();
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
    std::vector<Point> part1, part2;
    PointSet::SplitType parts_num = PointSet::split(polygon.verteces, part1, part2);
    if (parts_num == PointSet::SplitType::VERTICAL || parts_num == PointSet::SplitType::HORIZONTAL) {
        std::cout << "PART1:\n";
        for (const Point &p : part1) {
            std::cout << p << "\n";
        }
        std::cout << "PART2:\n";
        for (const Point &p : part2) {
            std::cout << p << "\n";
        }
    }

    polygon.foregroundColor = polygon.contrast ? red : green;

    if (polygon.need_to_redraw) {
        polygon.edges.clear();
        Triangulation triangulation = PointSet::triangulate(polygon.verteces);
        std::set<Edge> tr_edges = triangulation.edges();
        polygon.edges.insert(polygon.edges.begin(), tr_edges.begin(), tr_edges.end());
//        Point a, b;
//        for (size_t i = 1; i < polygon.verteces.size(); ++i)
//        {
//            a = polygon.verteces.at(i-1);
//            b = polygon.verteces.at(i);
//            polygon.addEdge(a, b);
//        }
//        if (polygon.verteces.size() > 2)
//        {   //дорисовать замыкающую линию
//            polygon.addEdge(polygon.verteces.back(), polygon.verteces.front());
//        }
        polygon.need_to_redraw = false;
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
