#include "Notebook.hpp"
#include "Direction.hpp"
#include <string>
#include <vector>
#include <array>

using ariel::Direction;
using namespace ariel;
using namespace std;



Notebook::Notebook(){
    pages = {};
}

void Notebook::write(int page, int row, int column, Direction direction, const string& text){
    int i = 0;
    while (i < pages.size() && pages[i]->num < page){
        i++;
    }
    if (pages[i]->num == page){
        //write
    }
    else{
        //create
    }
    Page *p = new Page(page);
    p->write(row, column, direction, text);
}

string Notebook::read(int page, int row, int column, Direction direction, int length){
    //check if the given arguments exceed the 100 chars limit or go to the negative (in case of negative length)
    if ((direction == Direction::Horizontal && (column + length > 100 || column + length < -1)) || 
         direction == Direction::Vertical && row + length < -1){
        throw out_of_range("You are trying to read past line limits!");
    }
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    string str;
    int i = 0;
    //getting to the matching page or right after it (since the pages are stored sorted)
    while (i < pages.size() && pages[i]->num < page){
        i++;
    }
    //checking if we arrived at the given page (since if it doesn't exist we passed it)
    if (pages[i]->num == page){
        int j = 0;
        //getting to the matching line (row) or right after it (since the lines are stored sorted)
        while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < row){
            j++;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //if we arrived at the given line (row)
            if (pages[i]->lines[j]->num == row){
                //read the content of the line from 'column' to 'column + length'
                for (int k = 0; k < length; k++){
                    str += pages[i]->lines[j]->chars[k + column];
                }
            }
            //the line doesn't exists therefore it's empty
            else{
                //so we read 'length' times underscores '_'
                for (int k = 0; k < length; k++){
                    str += '_';
                }
            }
        }
        //cases for vertical
        else{
            //we will increase the row value 'length' times
            for (int k = 0; k < length; k++){
                //checking if we are at the correct line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < row + k){
                        j++;
                }
                //we read the character at 'column' of the line
                if (pages[i]->lines[j + k]->num == row){
                    str += pages[i]->lines[j + k]->chars[column];
                }
                //the line doesn't exist therefore we read '_' this time
                else{
                    str += '_';
                }
            }
        }
    }
    //the page doesn't exist therefore it's empty
    else{
        for (int k = 0; k < length; k++){
                str += '_';
            }
    }
    return str;
}

void Notebook::erase(int page, int row, int column, Direction direction, int length){
}

void Notebook::show(int page){
}


Page::Page(int pageNum){
    num = pageNum;
    lines = {};
}

void Page::write(int row, int column, Direction direction, const string& text){
    int i = 0;
    while (i < lines.size() && lines[i]->num < row){
        i++;
    }
    if (direction == Direction::Horizontal){
        Line *l = new Line(row);
        for (int j = column, t = 0; j < text.size(); j++, t++){
            l->chars[j] = text[t];
        }
    }
}

Line::Line(int lineNum){
    num = lineNum;
    chars.fill('_');
}