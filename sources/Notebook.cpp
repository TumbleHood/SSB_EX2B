#include "Notebook.hpp"
#include "Direction.hpp"
#include <iostream>
#include <string>
#include <tr1/unordered_map>
#include <array>

using ariel::Direction;
using namespace std;
using namespace tr1;
using namespace ariel;

Notebook::Notebook(){
    nb = unordered_map<unsigned int, unordered_map<unsigned int, array<char, MAX_LINE>>>();
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
    unsigned int ui_page = (unsigned int)page;
    unsigned int ui_row = (unsigned int)row;
    unsigned int ui_column = (unsigned int)column;
    //check if the given arguments exceed the 100 chars limit
    if (ui_column >= MAX_LINE || (direction == Direction::Horizontal && ui_column + text.length() > MAX_LINE)){
        throw out_of_range("You are trying to write past the 100 characters limit!");
    }
    //if the notebook doesn't contain the page we create it
    if(nb.find(ui_page) == nb.end()){
        unordered_map<unsigned int, array<char, MAX_LINE>> m;
        nb.insert({ui_page, m});
    }
    //horizontal
    if (direction == Direction::Horizontal){
        //if the page doesn't contain the row we create it
        if (nb[ui_page].find(ui_row) == nb[ui_page].end()){
            array<char, MAX_LINE> a = {};
            a.fill('_');
            nb[ui_page].insert({ui_row, a});
        }
        //go over the text
        for (unsigned int i = 0; i < text.length(); i++){
            //check if the characters are empty
            if (nb[ui_page][ui_row][ui_column + i] != '_'){
                throw overflow_error("You are trying to write on existing text!");
            }
            nb[ui_page][ui_row][ui_column + i] = text[i];
        }
    }
    //vertical
    else{
        //go over the text
        for (unsigned int i = 0; i < text.length(); i++){
            //if the page doesn't contain the row we create it
            if (nb[ui_page].find(ui_row + i) == nb[ui_page].end()){
                array<char, MAX_LINE> a = {};
                a.fill('_');
                nb[ui_page].insert({ui_row + i, a});
            }
            if (nb[ui_page][ui_row + i][ui_column] != '_'){
                throw overflow_error("You are trying to write on existing text!");
            }
            nb[ui_page][ui_row + i][ui_column] = text[i];
        }
    }
}

string Notebook::read(int page, int row, int column, Direction direction, int length){
    //check if the given page, row, column and length are non-negative
    if (page < 0 || row < 0 || column < 0 || length < 0){
        throw invalid_argument("Page, Row, Column and Length arguments must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page;
    unsigned int ui_row = (unsigned int)row;
    unsigned int ui_column = (unsigned int)column;
    unsigned int ui_length = (unsigned int)length;
    //check if the given arguments exceed the 100 chars limit
    if (ui_column >= MAX_LINE || (direction == Direction::Horizontal && ui_column + ui_length > MAX_LINE)){
        throw out_of_range("You are trying to read past the 100 characters limit!");
    }
    //setting the default string to 'length' times the character '_'
    string str;
    for (unsigned int i = 0; i < length; i++){
        str += '_';
    }
    //if the notebook doesn't contain the page we return the default string
    if(nb.find(ui_page) == nb.end()){
        return str;
    }
    //horizontal
    if (direction == Direction::Horizontal){
        //if the page doesn't contain the row we return the default string
        if (nb[ui_page].find(ui_row) == nb[ui_page].end()){
            return str;
        }
        //go over the text
        for (unsigned int i = 0; i < length; i++){
            str[i] = nb[ui_page][ui_row][ui_column + i];
        }
    }
    //vertical
    else{
        //go over the text
        for (unsigned int i = 0; i < length; i++){
            //if the page doesn't contain the row we create it
            if (nb[ui_page].find(ui_row + i) == nb[ui_page].end()){
                str[i] = '_';
            }
            else{
                str[i] = nb[ui_page][ui_row + i][ui_column];
            }
        }
    }
    return str;
}

void Notebook::erase(int page, int row, int column, Direction direction, int length){
    //check if the given page, row, column and length are non-negative
    if (page < 0 || row < 0 || column < 0 || length < 0){
        throw invalid_argument("Page, Row, Column and Length arguments must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page;
    unsigned int ui_row = (unsigned int)row;
    unsigned int ui_column = (unsigned int)column;
    unsigned int ui_length = (unsigned int)length;
    //check if the given arguments exceed the 100 chars limit
    if (ui_column >= MAX_LINE || (direction == Direction::Horizontal && ui_column + ui_length > MAX_LINE)){
        throw out_of_range("You are trying to erase past the 100 characters limit!");
    }
    //if the notebook doesn't contain the page we create it
    if(nb.find(ui_page) == nb.end()){
        unordered_map<unsigned int, array<char, MAX_LINE>> m;
        nb.insert({ui_page, m});
    }
    //horizontal
    if (direction == Direction::Horizontal){
        //if the page doesn't contain the row we create it
        if (nb[ui_page].find(ui_row) == nb[ui_page].end()){
            array<char, MAX_LINE> a = {};
            a.fill('_');
            nb[ui_page].insert({ui_row, a});
        }
        //go over the text
        for (unsigned int i = 0; i < ui_length; i++){
            nb[ui_page][ui_row][ui_column + i] = '~';
        }
    }
    //vertical
    else{
        //go over the text
        for (unsigned int i = 0; i < ui_length; i++){
            //if the page doesn't contain the row we create it
            if (nb[ui_page].find(ui_row + i) == nb[ui_page].end()){
                array<char, MAX_LINE> a = {};
                a.fill('_');
                nb[ui_page].insert({ui_row + i, a});
            }
            nb[ui_page][ui_row + i][ui_column] = '~';
        }
    }
}

void Notebook::show(int page){
    if (page < 0){
        throw invalid_argument("Page variable must be non-negative!");
    }
    unsigned int ui_page = (unsigned int)page;
    if (nb.find(ui_page) != nb.end()){
        for (unsigned int i = 0, ctr = 0; ctr < nb[ui_page].size() || i < MAX_LINE; i++){
            if (nb[ui_page].find(i) != nb[ui_page].end()){
                ctr++;
            }
            cout << read(page, (int)i, 0, Direction::Horizontal, MAX_LINE) << endl;
        }
    }
    else{
        for (unsigned int i = 0; i < MAX_LINE; i++){
            cout << "____________________________________________________________________________________________________" << endl;
        }
    }
}