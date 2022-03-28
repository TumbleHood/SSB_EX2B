#include "Notebook.hpp"
#include "Direction.hpp"
#include <iostream>
#include <string>

using ariel::Direction;
using namespace ariel;
using namespace std;

int main(void){
    Notebook nb = Notebook();
    Direction h = Direction::Horizontal, v = Direction::Vertical;
    cout << nb.read(0, 0, 0, h, 10) << endl;
    nb.write(0, 0, 0, h, "Guy Gur-Arieh - System Software B - Exercise 2B - 2022THIS TEXT SHOULD NOT SHOW UP");
    cout << nb.read(0, 0, 0, h, 54) << endl;
    cout << nb.read(0, 0, 54, h, 17) << nb.read(0, 0, 75, h, 7) << endl;
    nb.erase(0, 0, 1, h, 3);
    nb.erase(0, 0, 5, h, 3);
    nb.erase(0, 0, 9, h, 7);
    nb.erase(0, 0, 17, h, 6);
    nb.erase(0, 0, 24, h, 8);
    nb.erase(0, 0, 33, h, 3);
    nb.erase(0, 0, 37, h, 8);
    nb.erase(0, 0, 47, h, 7);
    cout << nb.read(0, 0, 0, h, 54) << endl;
    nb.write(2, 0, 0, h, "mind");
    nb.write(2, 1, 0, h, "idea");
    nb.write(2, 2, 0, h, "neat");
    nb.write(2, 3, 0, h, "data");
    cout << nb.read(2, 0, 0, v, 4) << endl;
    cout << nb.read(2, 0, 1, v, 4) << endl;
    cout << nb.read(2, 0, 2, v, 4) << endl;
    cout << nb.read(2, 0, 3, v, 4) << endl;
    nb.write(100, 0, 0, h, "            .-\"\"\"-.");
    nb.write(100, 1, 0, h, "           /       \\");
    nb.write(100, 2, 0, h, "           \\       /");
    nb.write(100, 3, 0, h, "    .-\"\"\"-.-`.-.-.<  _");
    nb.write(100, 4, 0, h, "   /      _,-\\ ()()_/:)");
    nb.write(100, 5, 0, h, "   \\     / ,  `     `|");
    nb.write(100, 6, 0, h, "   '-..-| \\-.,___,  /");
    nb.write(100, 7, 0, h, "         \\ `-.__/  /");
    nb.write(100, 8, 0, h, "          `-.__.-'`");
    nb.show(100);
}