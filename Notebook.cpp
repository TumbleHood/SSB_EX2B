#include "Notebook.hpp"
#include "sources/Direction.hpp"
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
    //check if the given arguments exceed the 100 chars limit
    if (direction == Direction::Horizontal && column + text.size() > 100){
        throw out_of_range("You are trying to write past the 100 characters limit!");
    }
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    int i = 0;
    //getting to the matching page or right after it (since the pages are stored)
    while (i < pages.size() && pages[i]->num < page){
        i++;
    }
    //if we reached the page number it means the page exists
    if (pages.size() > 0 && pages[i]->num == page){
        int j = 0;
        //getting to the matching line or right after it (since the lines are stored)
        while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < row){
            j++;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
            if (pages[i]->lines.size() > 0 && pages[i]->lines[j]->num != row){
                Line *l = new Line(row);
                pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
            }
            //go over the given text and insert it in the correct positions
            for (int k = 0; k < text.size(); k++){
                pages[i]->lines[j]->chars[column + k] = text[k];
            }
        }
        //cases for vertical
        else{
            //go over the given text
            for (int k = 0; k < text.size(); k++){
                //each time, find the matching line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < row + k){
                        j++;
                }
                //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
                if (pages[i]->lines.size() > 0 && pages[i]->lines[j + k]->num != row){
                    Line *l = new Line(row + k);
                    pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
                }
                //insert the current characted at the matching position
                pages[i]->lines[j + k]->chars[column] = text[k];
            }
        }
    }
    //if the page doesn't exist we create a new one
    else{
        Page *p = new Page(page);
        pages.insert(pages.begin() + i, p);
        //horizontal case
        if (direction == Direction::Horizontal){
            //since the page didn't exist, it has to lines, so we create a new one at position 0 and insert the text
            Line *l = new Line(row);
            pages[i]->lines.insert(pages[i]->lines.begin(), l);
            for (int k = 0; k < text.size(); k++){
                pages[i]->lines[0]->chars[column + k] = text[k];
            }
        }
        //vertical case
        else{
            for (int k = 0; k < text.size(); k++){
                //since the page didn't exist and it has no lines, each time we create a new line and insert
                Line *l = new Line(row + k);
                pages[i]->lines.insert(pages[i]->lines.begin() + k, l);
                pages[i]->lines[k]->chars[column] = text[k];
            }
        }
    }
}

string Notebook::read(int page, int row, int column, Direction direction, int length){
    //check if the given arguments exceed the 100 chars limit or go to the negative (in case of negative length)
    if ((direction == Direction::Horizontal && (column + length > 100 || column + length < -1)) || 
        (direction == Direction::Vertical && row + length < -1)){
        throw out_of_range("You are trying to read past line limits!");
    }
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    string str;
    int i = 0;
    //getting to the matching page or right after it (since the pages are stored)
    while (i < pages.size() && pages[i]->num < page){
        i++;
    }
    //checking if we arrived at the given page (since if it doesn't exist we passed it)
    if (pages.size() > 0 && pages[i]->num == page){
        int j = 0;
        //getting to the matching line (row) or right after it (since the lines are stored)
        while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < row){
            j++;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //if we arrived at the given line (row)
            if (pages[i]->lines.size() > 0 && pages[i]->lines[j]->num == row){
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
                if (pages[i]->lines.size() > 0 && pages[i]->lines[j + k]->num == row){
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
    //check if the given arguments exceed the 100 chars limit or go to the negative (in case of negative length)
    if ((direction == Direction::Horizontal && (column + length > 100 || column + length < -1)) || 
        (direction == Direction::Vertical && row + length < -1)){
        throw out_of_range("You are trying to erase past line limits!");
    }
    //check if the given page, row, and column are non-negative
    if (page < 0 || row < 0 || column < 0){
        throw invalid_argument("Page, Row, and Column arguments must be non-negative!");
    }
    int i = 0;
    //getting to the matching page or right after it (since the pages are stored)
    while (i < pages.size() && pages[i]->num < page){
        i++;
    }
    //if we reached the page number it means the page exists
    if (pages.size() > 0 && pages[i]->num == page){
        int j = 0;
        //getting to the matching line or right after it (since the lines are stored)
        while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < row){
            j++;
        }
        //cases for horizontal
        if (direction == Direction::Horizontal){
            //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
            if (pages[i]->lines.size() > 0 && pages[i]->lines[j]->num != row){
                Line *l = new Line(row);
                pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
            }
            //go over the given text and insert it in the correct positions
            for (int k = 0; k < length; k++){
                pages[i]->lines[j]->chars[column + k] = '~';
            }
        }
        //cases for vertical
        else{
            //go over the given text
            for (int k = 0; k < length; k++){
                //each time, find the matching line
                while (j < pages[i]->lines.size() && pages[i]->lines[j]->num < row + k){
                        j++;
                }
                //if the line doesn't exist we create a new one and insert it in the current position (since it is sorted)
                if (pages[i]->lines.size() > 0 && pages[i]->lines[j + k]->num != row){
                    Line *l = new Line(row + k);
                    pages[i]->lines.insert(pages[i]->lines.begin() + j, l);
                }
                //insert the current characted at the matching position
                pages[i]->lines[j + k]->chars[column] = '~';
            }
        }
    }
    //if the page doesn't exist we create a new one
    else{
        Page *p = new Page(page);
        pages.insert(pages.begin() + i, p);
        //horizontal case
        if (direction == Direction::Horizontal){
            //since the page didn't exist, it has to lines, so we create a new one at position 0 and insert the text
            Line *l = new Line(row);
            pages[i]->lines.insert(pages[i]->lines.begin(), l);
            for (int k = 0; k < length; k++){
                pages[i]->lines[0]->chars[column + k] = '~';
            }
        }
        //vertical case
        else{
            for (int k = 0; k < length; k++){
                //since the page didn't exist and it has no lines, each time we create a new line and insert
                Line *l = new Line(row + k);
                pages[i]->lines.insert(pages[i]->lines.begin() + k, l);
                pages[i]->lines[k]->chars[column] = '~';
            }
        }
    }
}

void Notebook::show(int page){
    int i = 0;
    while (i < pages.size() && pages[i]->num < page){
        i++;
    }
    if (pages.size() > 0 && pages[i]->num == page){
        int k = 0;
        for (int j = 0; j < pages[i]->lines.size(); j++){
            while (k < pages[i]->lines[j]->num){
                cout << read(page, k, 0, Direction::Horizontal, 100);
                k++;
            }
        }
    }
}


Page::Page(int pageNum){
    num = pageNum;
    lines = {};
}

Line::Line(int lineNum){
    num = lineNum;
    chars.fill('_');
}