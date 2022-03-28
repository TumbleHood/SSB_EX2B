#include "Notebook.hpp"
#include "Direction.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <array>

#define MAX_ASCII 125
#define MIN_ASCII 32

using ariel::Direction;
using namespace ariel;
using namespace std;

Line::Line(unsigned int lineNum){
    num = lineNum;
    chars.fill('_');
}

Page::Page(unsigned int pageNum){
    num = pageNum;
    lines = {};
}

Notebook::Notebook(){
    pages = {};
}

void Notebook::write(int page, int row, int column, Direction direction, const string& text){
    //check if the string contains illegal characters
    for (unsigned int t = 0; t < text.length(); t++){
        if (text[t] < MIN_ASCII || text[t] > MAX_ASCII){
            throw invalid_argument("You are trying to write illegal character!");
        }
    }
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page, ui_row = (unsigned int)row, ui_column = (unsigned int)column;
    //check if the given arguments exceed the 100 chars limit
    if (ui_column >= 100 || (direction == Direction::Horizontal && ui_column + text.length() >= 100)){
        throw out_of_range("You are trying to write past the 100 characters limit!");
    }
    //if pages is empty we create a new one
    if (pages.empty()){
        Page *p = new Page(ui_page);
        pages.insert(pages.begin(), p);
    }
    //getting to the matching page or right after it (since the pages are stored)
    unsigned int i = 0;
    while (i < pages.size() && pages[i]->num < ui_page){
        i++;
    }
    //we went over all pages and the given page doesn't exist so we create a new one
    if (i >= pages.size()){
        Page *p = new Page(ui_page);
        pages.insert(pages.end(), p);
    }
    //if we reached the page number it means the page exists
    if (pages[i]->num == ui_page){
        //if there are no lines we create a new one
        if (pages[i]->lines.empty()){
            Line *l = new Line(ui_row);
            pages[i]->lines.insert(pages[i]->lines.begin(), l);
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //going over all lines
            unsigned int j = 0;
            while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row){
                j++;
            }
            //we went over all the lines and the given row doesn't exist so we create it
            if (j >= pages[i]->lines.size()){
                Line *l = new Line(ui_row);
                pages[i]->lines.insert(pages[i]->lines.end(), l);
            }
            //write the text in the correct place
            for (unsigned int k = 0; k < text.length(); k++){
                if (pages[i]->lines[j]->chars[k + ui_column] != '_'){
                    throw overflow_error("You are trying to write on existing text!");
                }
                if (text[k] == ' '){
                    pages[i]->lines[j]->chars[k + ui_column] = '_';
                }
                else{
                    pages[i]->lines[j]->chars[k + ui_column] = text[k];
                }
            }
            pages[i]->lines.shrink_to_fit();
        }
        //cases for vertical
        else{
            unsigned int j = 0;
            //go over the given text
            for (unsigned int k = 0; k < text.length(); k++){
                //each time, find the matching line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row + k){
                    j++;
                }
                //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
                if (j >= pages[i]->lines.size()){
                    Line *l = new Line(ui_row + k);
                    pages[i]->lines.insert(pages[i]->lines.end(), l);
                }
                //insert the current characted at the matching position
                if (pages[i]->lines[j]->chars[k + ui_column] != '_'){
                    throw overflow_error("You are trying to write on existing text!");
                }
                if (text[k] == ' '){
                    pages[i]->lines[j]->chars[k + ui_column] = '_';
                }
                else{
                    pages[i]->lines[j]->chars[k + ui_column] = text[k];
                }
                pages[i]->lines.shrink_to_fit();
            }
        }
    }
}

string Notebook::read(int page, int row, int column, Direction direction, int length){
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0 || length < 0){
        throw invalid_argument("Page, Row, Column and Length arguments must be non-negative!");
    }
    //turn all arguments into unsigned int since they are non-negative (and we need to check size)
    unsigned int ui_page = (unsigned int)page, ui_row = (unsigned int)row, ui_column = (unsigned int)column, ui_length = (unsigned int)length;
    //check if the given arguments exceed the 100 chars limit
    if (direction == Direction::Horizontal && ui_column + ui_length > 100){
        throw out_of_range("You are trying to read past the 100 characters limit!");
    }
    string str;
    //dummy_str = 'length' times the character '_' - we save it in case of missing page / line (horizontal)
    string dummy_str;
    for (unsigned int k = 0; k < length; k++){
        dummy_str += '_';
    }
    //if there are no pages we read the dummy string
    if (pages.empty()){
        return dummy_str;
    }
    //getting to the matching page or right after it (since the pages are stored)
    unsigned int i = 0;
    while (i < pages.size() && pages[i]->num < ui_page){
        i++;
    }
    //we went over all the pages and the given page doesn't exist so we read the dummy string
    if (i >= pages.size()){
        return dummy_str;
    }
    //checking if we arrived at the given page (since if it doesn't exist we passed it)
    if (pages[i]->num == ui_page){
        //there are no lines so we read the dummy string
        if (pages[i]->lines.empty()){
            return dummy_str;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //going over all lines
            unsigned int j = 0;
            while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row){
                j++;
            }
            //we went over all the lines and the given row doesn't exist so we read the dummy string
            if (j >= pages[i]->lines.size()){
                return dummy_str;
            }
            for (unsigned int k = 0; k < ui_length; k++){
                str += pages[i]->lines[j]->chars[k + ui_column];
            }
            return str;
        }
        //cases for vertical
        unsigned int j = 0;
        for (unsigned int k = 0; k < ui_length; k++){
            //checking if we are at the correct line
            while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row + k){
                j++;
            }
            //we read the character at 'column' of the line
            if (j >= pages[i]->lines.size()){
                str += '_';
            }
            //the line doesn't exist therefore we read '_' this time
            else{
                str += pages[i]->lines[j]->chars[ui_column];
            }
        }
        return str;
    }
    return dummy_str;
}

void Notebook::erase(int page, int row, int column, Direction direction, int length){
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0 || length < 0){
        throw invalid_argument("Page, Row, Column and Length arguments must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page, ui_row = (unsigned int)row, ui_column = (unsigned int)column, ui_length = (unsigned int)length;
    //check if the given arguments exceed the 100 chars limit
    if (direction == Direction::Horizontal && ui_column + ui_length > 100){
        throw out_of_range("You are trying to erase past the 100 characters limit!");
    }
    //if pages is empty we create a new one
    if (pages.empty()){
        Page *p = new Page(ui_page);
        pages.insert(pages.begin(), p);
    }
    //getting to the matching page or right after it (since the pages are stored)
    unsigned int i = 0;
    while (i < pages.size() && pages[i]->num < ui_page){
        i++;
    }
    //we went over all pages and the given page doesn't exist so we create a new one
    if (i >= pages.size()){
        Page *p = new Page(ui_page);
        pages.insert(pages.end(), p);
    }
    //if we reached the page number it means the page exists
    if (pages[i]->num == ui_page){
        //if there are no lines we create a new one
        if (pages[i]->lines.empty()){
            Line *l = new Line(ui_row);
            pages[i]->lines.insert(pages[i]->lines.begin(), l);
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //going over all lines
            unsigned int j = 0;
            while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row){
                j++;
            }
            //we went over all the lines and the given row doesn't exist so we create it
            if (j >= pages[i]->lines.size()){
                Line *l = new Line(ui_row);
                pages[i]->lines.insert(pages[i]->lines.end(), l);
            }
            //erase in the correct place
            for (unsigned int k = 0; k < ui_length; k++){
                pages[i]->lines[j]->chars[k + ui_column] = '~';
            }
            pages[i]->lines.shrink_to_fit();
        }
        //cases for vertical
        else{
            unsigned int j = 0;
            //go over the given text
            for (unsigned int k = 0; k < ui_length; k++){
                //each time, find the matching line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < ui_row + k){
                    j++;
                }
                //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
                if (j >= pages[i]->lines.size()){
                    Line *l = new Line(ui_row + k);
                    pages[i]->lines.insert(pages[i]->lines.end(), l);
                }
                //erase at the matching position
                pages[i]->lines[j]->chars[ui_column] = '~';
                
                pages[i]->lines.shrink_to_fit();
            }
        }
    }
}

void Notebook::show(int page){
    for (int i = 0; i < 10; i++){
        cout << read(page, i, 0, Direction::Horizontal, 10) << endl;
    }
}