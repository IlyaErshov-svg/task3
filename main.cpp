#include <iostream>
#include <vector>
#include <string>
#include <limits>

struct Point{
    double x;
    double y;
};

Point operator+(Point point_1, Point point_2)
{
    return Point{point_1.x + point_2.x, point_1.y + point_2.y};
}

Point operator-(Point point_1, Point point_2)
{
    return Point{point_1.x - point_2.x, point_1.y - point_2.y};
}

/// Парсинг вершин
/// \param input строка типа: coord coord | coord coord | ... | coord coord
/// \return набор точек
std::vector<Point> point_parser(const std::string& input)
{
    std::vector<Point> result;
    std::string temp_num;
    Point temp;
    bool x_exist = false;
    for(char i : input)
    {
        if(i >= '0' && i <= '9' || i == '.' || i == '-')
        {
            temp_num.push_back(i);
        }
        else if(i == ' ')
        {
            if(!temp_num.empty()) {
                if (x_exist) {
                    temp.y = std::stof(temp_num);
                    temp_num = "";
                    x_exist = false;
                } else {
                    temp.x = std::stof(temp_num);
                    temp_num = "";
                    x_exist = true;
                }
            }
        }
        else if(i == '|' && !x_exist)
        {
            result.push_back(temp);
        }else{
            std::cerr << "Invalid arguments";
            return {};
        }
    }
    temp.y = std::stof(temp_num);
    result.emplace_back(temp);
    return result;
}


double cross(const Point& p1, const Point& p2)
{
    return p1.x * p2.y - p2.x * p1.y;
}


double dot(const Point& p1, const Point& p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}


/// Алгоритм проверки пересечения между отрезками
/// \param p1 Первая точка первого отрезка
/// \param p2 Вторая точка первого отрезка
/// \param q1 Первая точка второго отрезка
/// \param q2 Вторая точка второго отрезка
/// \return true, если пересечение есть, false, если пересечения нет
bool check_intersection(const Point& p1, const Point& p2, const Point& q1, const Point& q2)
{
    Point r = p2 - p1;
    Point s = q2 - q1;
    double t0 = cross(q1 - p1, r) / dot(r, r);
    double t1 = t0 + dot(s, r) / dot(r, r);
    double t = cross(q1 - p1 , s) / cross(r, s);
    double u = cross(p1 - q1, r) / cross(s,r );
    /*
    if(t1 >= 0 && t1 <= 1 || t0 >= 0 && t0 <= 1 )
    {
        return false;
    }
     */
    if(cross(r, s) < std::numeric_limits<double>::epsilon() &&
    cross(q1 - p1, r) > std::numeric_limits<double>::epsilon())
    {
        return false;
    }
    if(cross(r, s) > std::numeric_limits<double>::epsilon() &&
    t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        return true;
    }
    return false;
}

/// Алгоритм валидации самопересечений путем попарной проверки пересечений между отрезками не идущих друг за другом
/// \param polygon набор точек,описывающий полигон
/// \return true, если пересечений нет, false, если пересечения есть
bool isValid(std::vector<Point> polygon)
{

    for(int i = 0; i < polygon.size() - 1; ++i)
    {
        for(int j = 0; j < polygon.size() - 1; ++j)
        {
            if(std::abs(i - j) > 1 && check_intersection(polygon[i], polygon[i + 1], polygon[j], polygon[j +1]))
            {
                return false;
            }
        }
        if(i != 0 && i != polygon.size() - 2 && check_intersection(polygon[i], polygon[i + 1], polygon[polygon.size() - 1], polygon[0]))
        {
            return false;
        }
    }
    for(int i = 1; i < polygon.size() - 2; ++i){
        if(check_intersection(polygon[i], polygon[i + 1], polygon[polygon.size() - 1], polygon[0]))
        {
            return false;
        }
    }
    return true;
}

/// Вычисление площади по цепочке точек
/// \param board набор точек
/// \return площадь
double area(const std::vector<Point>& board){
    double result = 0;
    for(std::size_t i = 0; i < board.size() - 1; ++i)
    {
        result += board[i].x * board[i + 1].y;
        result -= board[i + 1].x * board[i].y;
    }
    result += board[board.size() - 1].x * board[0].y - board[board.size() - 1].y * board[0].x;
    return std::abs(result) / 2.;
}



int main() {

    std::string input;
    std::cout << "area " << std::flush;
    getline(std::cin, input);
    std::vector<Point> points = point_parser(input);
    if(points.empty())
    {
        return 0;
    }
    if(!isValid(points))
    {
        std::cerr << "Polygon with self intersection";
        return 0;
    }
    std::cout << area(points) << std::endl;
    return 0;
}
// 3 4 | 5 11 | 12 8 | 9 5 | 5 6 -> 30
// 1 1 | -1 -1 | -1 1 | 1 -1 -> Polygon with self intersection
// 1 1 | a 1 | -1 | 1 | -1 -> Invalid arguments