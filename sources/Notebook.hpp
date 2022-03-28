#include "Direction.hpp"
#include <string>
#include <vector>
#include <array>

using ariel::Direction;
using namespace std;

namespace ariel{

    class Line{
        public:
            unsigned int num;
            array<char, 100> chars;
            Line(unsigned int lineNum);
    };

    class Page{
        public:
            unsigned int num;
            vector<Line*> lines;
            Page(unsigned int pageNum);
    };

    class Notebook{
        public:
            vector<Page*> pages;
            Notebook();
            void write(int page, int row, int column, Direction direction, const string& text);
            string read(int page, int row, int column, Direction direction, int length);
            void erase(int page, int row, int column, Direction direction, int length);
            void show(int page);
    };
}