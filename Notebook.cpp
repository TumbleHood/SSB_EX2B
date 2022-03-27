#include "Notebook.hpp"
#include "Direction.hpp"
#include <string>

using ariel::Direction;
using namespace ariel;
using namespace std;



Notebook::Notebook(){
    Page value;
    Notebook *next = NULL;
}

void Notebook::write(int page, int row, int column, Direction direction, const string& text){
}

string Notebook::read(int page, int row, int column, Direction direction, int length){
    return "";
}

void Notebook::erase(int page, int row, int column, Direction direction, int length){
}

void Notebook::show(int page){
}


Page::Page(){
    int pageNum;
    char line[100];
    Page *nextLine = NULL;
}