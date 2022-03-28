#include "Direction.hpp"
#include <string>
#include <tr1/unordered_map>
#include <array>

#define MIN_ASCII 32
#define MAX_ASCII 125
#define MAX_LINE 100

using ariel::Direction;
using namespace std;
using namespace tr1;

namespace ariel{
    class Notebook{
        public:
            unordered_map<unsigned int, unordered_map<unsigned int, array<char, 100>>> nb;
            Notebook();
            void write(int page, int row, int column, Direction direction, const string& text);
            string read(int page, int row, int column, Direction direction, int length);
            void erase(int page, int row, int column, Direction direction, int length);
            void show(int page);
    };
}