#include "Notebook.hpp"
#include "Direction.hpp"
#include <iostream>
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
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page, ui_row = (unsigned int)row, ui_column = (unsigned int)column;
    //check if the given arguments exceed the 100 chars limit
    if (direction == Direction::Horizontal && ui_column + text.size() > 100){
        throw out_of_range("You are trying to write past the 100 characters limit!");
    }
    unsigned int i = 0;
    //getting to the matching page or right after it (since the pages are stored)
    while (i < pages.size() && pages[i]->num < ui_page){
        i++;
    }
    //if we reached the page number it means the page exists
    if (pages.size() > 0 && pages[i]->num == ui_page){
        unsigned int j = 0;
        //getting to the matching line or right after it (since the lines are stored)
        while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row){
            j++;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
            if (pages[i]->lines.size() > 0 && pages[i]->lines[j]->num != ui_row){
                Line *l = new Line(ui_row);
                pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
            }
            //go over the given text and insert it in the correct positions
            for (unsigned int k = 0; k < text.size(); k++){
                pages[i]->lines[j]->chars[ui_column + k] = text[k];
            }
        }
        //cases for vertical
        else{
            //go over the given text
            for (unsigned int k = 0; k < text.size(); k++){
                //each time, find the matching line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row + k){
                        j++;
                }
                //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
                if (pages[i]->lines.size() > 0 && pages[i]->lines[j + k]->num != ui_row){
                    Line *l = new Line(ui_row + k);
                    pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
                }
                //insert the current characted at the matching position
                pages[i]->lines[j + k]->chars[ui_column] = text[k];
            }
        }
    }
    //if the page doesn't exist we create a new one
    else{
        Page *p = new Page(ui_page);
        pages.insert(pages.begin() + i, p);
        //horizontal case
        if (direction == Direction::Horizontal){
            //since the page didn't exist, it has to lines, so we create a new one at position 0 and insert the text
            Line *l = new Line(ui_row);
            pages[i]->lines.insert(pages[i]->lines.begin(), l);
            for (unsigned int k = 0; k < text.size(); k++){
                pages[i]->lines[0]->chars[ui_column + k] = text[k];
            }
        }
        //vertical case
        else{
            for (unsigned int k = 0; k < text.size(); k++){
                //since the page didn't exist and it has no lines, each time we create a new line and insert
                Line *l = new Line(ui_row + k);
                pages[i]->lines.insert(pages[i]->lines.begin() + k, l);
                pages[i]->lines[k]->chars[ui_column] = text[k];
            }
        }
    }
}

string Notebook::read(int page, int row, int column, Direction direction, int length){
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0 || length < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page, ui_row = (unsigned int)row, ui_column = (unsigned int)column, ui_length = (unsigned int)length;
    //check if the given arguments exceed the 100 chars limit
    if (direction == Direction::Horizontal && ui_column + ui_length > 100){
        throw out_of_range("You are trying to read past the 100 characters limit!");
    }
    string str;
    unsigned int i = 0;
    //getting to the matching page or right after it (since the pages are stored)
    while (i < pages.size() && pages[i]->num < ui_page){
        i++;
    }
    //checking if we arrived at the given page (since if it doesn't exist we passed it)
    if (pages.size() > 0 && pages[i]->num == ui_page){
        unsigned int j = 0;
        //getting to the matching line (row) or right after it (since the lines are stored)
        while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row){
            j++;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //if we arrived at the given line (row)
            if (pages[i]->lines.size() > 0 && pages[i]->lines[j]->num == ui_row){
                //read the content of the line from 'column' to 'column + length'
                for (unsigned int k = 0; k < ui_length; k++){
                    str += pages[i]->lines[j]->chars[k + ui_column];
                }
            }
            //the line doesn't exists therefore it's empty
            else{
                //so we read 'length' times underscores '_'
                for (unsigned int k = 0; k < ui_length; k++){
                    str += '_';
                }
            }
        }
        //cases for vertical
        else{
            //we will increase the row value 'length' times
            for (unsigned int k = 0; k < ui_length; k++){
                //checking if we are at the correct line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row + k){
                        j++;
                }
                //we read the character at 'column' of the line
                if (pages[i]->lines.size() > 0 && pages[i]->lines[j + k]->num == ui_row){
                    str += pages[i]->lines[j + k]->chars[ui_column];
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
        for (unsigned int k = 0; k < ui_length; k++){
                str += '_';
            }
    }
    return str;
}

void Notebook::erase(int page, int row, int column, Direction direction, int length){
    //check if the given arguments exceed the 100 chars limit
    if (direction == Direction::Horizontal && column + length > 100){
        throw out_of_range("You are trying to erase past the 100 characters limit!");
    }
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0 || length < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page, ui_row = (unsigned int)row, ui_column = (unsigned int)column, ui_length = (unsigned int)length;
    unsigned int i = 0;
    //getting to the matching page or right after it (since the pages are stored)
    while (i < pages.size() && pages[i]->num < ui_page){
        i++;
    }
    //if we reached the page number it means the page exists
    if (pages.size() > 0 && pages[i]->num == ui_page){
        unsigned int j = 0;
        //getting to the matching line or right after it (since the lines are stored)
        while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row){
            j++;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
            if (pages[i]->lines.size() > 0 && pages[i]->lines[j]->num != ui_row){
                Line *l = new Line(ui_row);
                pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
            }
            //go over the given text and insert it in the correct positions
            for (unsigned int k = 0; k < ui_length; k++){
                pages[i]->lines[j]->chars[ui_column + k] = '~';
            }
        }
        //cases for vertical
        else{
            //go over the given text
            for (unsigned int k = 0; k < ui_length; k++){
                //each time, find the matching line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row + k){
                        j++;
                }
                //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
                if (pages[i]->lines.size() > 0 && pages[i]->lines[j + k]->num != ui_row){
                    Line *l = new Line(ui_row + k);
                    pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
                }
                //insert the current characted at the matching position
                pages[i]->lines[j + k]->chars[ui_column] = '~';
            }
        }
    }
    //if the page doesn't exist we create a new one
    else{
        Page *p = new Page(ui_page);
        pages.insert(pages.begin() + i, p);
        //horizontal case
        if (direction == Direction::Horizontal){
            //since the page didn't exist, it has to lines, so we create a new one at position 0 and insert the text
            Line *l = new Line(ui_row);
            pages[i]->lines.insert(pages[i]->lines.begin(), l);
            for (unsigned int k = 0; k < ui_length; k++){
                pages[i]->lines[0]->chars[ui_column + k] = '~';
            }
        }
        //vertical case
        else{
            for (unsigned int k = 0; k < ui_length; k++){
                //since the page didn't exist and it has no lines, each time we create a new line and insert
                Line *l = new Line(ui_row + k);
                pages[i]->lines.insert(pages[i]->lines.begin() + k, l);
                pages[i]->lines[k]->chars[ui_column] = '~';
            }
        }
    }
}

void Notebook::show(int page){
    
}


Page::Page(unsigned int pageNum){
    num = pageNum;
    lines = {};
}

Line::Line(unsigned int lineNum){
    num = lineNum;
    chars.fill('_');
}