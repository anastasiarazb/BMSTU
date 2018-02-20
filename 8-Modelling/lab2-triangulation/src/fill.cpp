#include "rasterization.h"

//struct ActiveEdge
//{
//    int y_max, x_start;
//    double x_current, dx;
//    ActiveEdge() {}
//    ActiveEdge(int y_max, int x_start, double dx):
//        y_max(y_max), x_start(x_start), x_current((double)x_start), dx(dx){}
//};

bool operator<(const ActiveEdge& a, const ActiveEdge &b)
{
    return a.x_current < b.x_current;
}

std::ostream& operator<<(std::ostream &os, const std::list<ActiveEdge>& lst)
{
    os << "AET: {\n";
    for ( auto el: lst)
    {
        os << " x_start = " << el.x_start << " y_max = " << el.y_max << "\n";
    }
    return os << "}" << std::endl;
}

std::ostream& operator<<(std::ostream &os, const ActiveEdge& el)
{
    return os << " x_start = " << el.x_start << " y_max = " << el.y_max << std::endl;
}

//std::list<ActiveEdge> AET; //Active Edge Table - Список Активных Ребер
int Y_next;
int Y_max;
int Y_current;
std::list<Edge>::iterator last_edge;

//Удалить ребра, заканчивающиеся на y_current
//Добавить новые ребра
//Вычислить новый Y_next - минимальный из y_max в списке
//Код возврата - не достигли ли конца буфера
void Framebuffer::refresh_AET()
{
    if(Y_current != Y_next){
//        printf("[refresh_AET]: Y_current = %d, Y_next = %d\n", Y_current, Y_next);
        exit(1);
    }
    std::cout << "-------------------\n  [Refresh AET]\n" << AET;
    std::cout << polygon.edges;
    std::cout << AET;

    std::list<ActiveEdge>::iterator it = AET.begin();
    while(it!= AET.end())
    {
//        std::cout << "       Y_current = " << Y_current << " it->y_max = " <<it->y_max << "\n";
        if(it->y_max == Y_current)
        { //Для всех ребер из САР: если конец ребра совпадает с текущей строкой - удалить его
            it = AET.erase(it);
//            std::cout << "       erase\n";
        } else {
           ++it;
//            std::cout << "       skip\n";
        }
    }
    //Добавление новых.
    double dx;
    while(last_edge != polygon.edges.end() && last_edge->a.y == Y_current)
    {
        if (last_edge->b.y != last_edge->a.y)
        {//Если ребро не горизонтально и начинается на строке сканирования - добавить в Список
            dx = (double)(last_edge->b.x - last_edge->a.x)/
                 (double)(last_edge->b.y - last_edge->a.y);
//            std::cout << "[refresh_AET]: ADD" << *(last_edge);
            AET.emplace_back(last_edge->b.y, last_edge->a.x, dx);
        }
        else //Горизонтальное ребро
        {
            drawLine(Y_current, last_edge->a.x, last_edge->b.x);
        }
        ++last_edge;
    }

    //Y_next корректируется так, чтобы содержать минимальную координату в списке
    Y_next = Y_max;
    for (const auto& e: AET)
    {//Хотя можно было корректировать и в цикле удаления закончившихся, но так яснее
        if(Y_next > e.y_max)
        {
            Y_next = e.y_max;
        }
    }

    //Выяснить, не появится ли новое ребро раньше, чем закончатся добавленные
    //(следующий кандидат на добавление)
    if(last_edge != polygon.edges.end() && last_edge->a.y < Y_next)
    {
        Y_next = last_edge->a.y;
    }

    if(Y_next > Y_max)
    {//Чтобы не выйти за границы буфера в цикле while(Y_current < Y_next)
        Y_next = Y_max;
    }

    printf("END refresh\n");
}

inline void assertAETend(const std::list<ActiveEdge>::iterator &it, const std::list<ActiveEdge> &AET)
{
    if (it == AET.end())
    {
        printf("[Framebuffer::fillLines]: wrong edges amount/\n");
        exit(1);
    }
}

inline void Framebuffer::fillLines()
{
    std::list<ActiveEdge>::iterator it = AET.begin();
    int x1, x2;
//    std::cout << "[fillLines()]:" << AET;
    while (it != AET.end())
    {
        x1 = (int)round(it->x_current);
        ++it;                 //Переход к ребру правее
        assertAETend(it, AET);
        x2 = std::max(x1, (int)round(it->x_current));
        if (x1 <= x2)
        { //Чтобы не было лишних точек на углах. Пусть они покрываются только граничной линией
            drawLine(Y_current, x1, x2);
        }
        ++it;
    }
}

inline void Framebuffer::updateX()
{
    for (auto& e: AET)
    {//Вычисление координат пресечения со строкой с использованием свойства когерентности
        e.x_current += e.dx;
    }
}

void Framebuffer::fillPolygon()
{
    int Y_min = polygon.edges.front().a.y;
    Y_max = std::min(polygon.y_max, height - 1);
    Y_next = Y_min;

    if(polygon.edges.size() < 3)
    {
//        printf("Singular polygon\n"); //Вырожденный случай
        return;
    }

//    printf("*****************\n [FILL POLYGON]\nY_MAX = %d, Y_MIN = %d\n", Y_max, Y_min);
//    std::cout << polygon.edges ;

    last_edge = polygon.edges.begin();
    for(Y_current = Y_min; Y_current < Y_max; /*++Y_current*/)
    {
        refresh_AET();
//        printf("Y_next = %d\n", Y_next);
        while(Y_current < Y_next)
        {
            AET.sort();
            fillLines();
            ++Y_current;

            updateX();
        }
    }
    AET.clear();
//    std::cout << "[fillPolygon]: end" << std::endl;
}


