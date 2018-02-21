#define GLFW_INCLUDE_GLU
#include "rasterization.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <random>
#include <GLFW/glfw3.h>

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
        Pixel *temp = (Pixel *)calloc(sizeof(Pixel), new_size);
        free(canvas);
        canvas = temp;
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

bool Point::lessByX(Point const &p1, Point const &p2) {
    return p1.x < p2.x;
}

bool Point::lessByY(Point const &p1, Point const &p2) {
    return p1.y < p2.y;
}

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
{
    return e1.a < e2.b;
}


std::ostream& operator<<(std::ostream& os, const std::list<Edge>& edges)
{
    os << "EDGES: {\n";
    for(auto e: edges)
    {
        os << " (" << e.a.x << ", " << e.a.y<< ", " << e.a.z
           << ")--->(" << e.b.x << ", " << e.b.y<< ", " << e.b.z << ")\n";
    }
    return os << "}"<<std::endl;
}

std::ostream& operator<< (std::ostream& os, const Edge& e)
{
    return os << " (" << e.a.x << ", " << e.a.y<< ", " << e.a.z
              << ")--->(" << e.b.x << ", " << e.b.y<< ", " << e.b.z << ")" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Framebuffer& F)
{
    os << "Framebuffer:\n " << F.width << "x" << F.height <<
          ";  size = " << F.size << std::endl;
    return os;
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
    printf("PUSH %d,  %d\n", x, height - y);
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
    addPoint(992, 72, 3);
    addPoint(129, 243, 2);
    addPoint(732, 43, 1);
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
                    access(dot.x - i, dot.y + j).set(GREEN);
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

    polygon.foregroundColor = polygon.contrast ? red : green;

    if (polygon.filled)
    {
        fillPolygon();
    }
    if (polygon.need_to_redraw) {
        polygon.edges.clear();
        Point a, b;
        for (size_t i = 1; i < polygon.verteces.size(); ++i)
        {
            a = polygon.verteces.at(i-1);
            b = polygon.verteces.at(i);
            polygon.addEdge(a, b);
        }
        if (polygon.verteces.size() > 2)
        {   //дорисовать замыкающую линию
            polygon.addEdge(polygon.verteces.back(), polygon.verteces.front());
        }
        polygon.need_to_redraw = false;
    }
    if (polygon.lined)
    {
        for(const auto& e: polygon.edges)
        {
            Bresenham(e);
        }
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

#define SWAP(a, b) { \
    temp = a; \
    a = b;    \
    b = temp; \
 }

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
