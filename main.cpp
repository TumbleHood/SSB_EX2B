#include "Notebook.hpp"
#include "sources/Direction.hpp"
#include <iostream>
#include <string>

using ariel::Direction;
using namespace ariel;
using namespace std;

int main(void){
    Notebook nb = Notebook();
    Direction h = Direction::Horizontal, v = Direction::Vertical;
    nb.write(0, 0, 0, h, "Guy Gur-Arieh - System Software B - Exercise 2B - 2022THIS TEXT SHOULD NOT SHOW UP");
    cout << nb.read(0, 0, 0, h, 54) << endl;
    cout << nb.read(0, 0, 54, h, 16) << " " << nb.read(0, 0, 75, h, 7) << endl;
}