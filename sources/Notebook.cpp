#include <iostream>
#include <stdexcept>
#include "Notebook.hpp"
using namespace std;
#include <string>
#include <array>
#include <algorithm>
#include "Direction.hpp"
using ariel::Direction;


const char start_of_valid_char = 32;
const char end_of_valid_char = 126;
bool is_symbol(char c)
{

    if (c >= start_of_valid_char && c <= end_of_valid_char){
        return true;
    }
    if (c == '\n'){
        return false;
    }
    if (c == 0){
        return false;
    }
    return false;
}
/*----------------------------------------
                line
---------------------------------------*/
ariel::Row::Row(int row_number) {
    ariel::Row::row_number = row_number;
    //set the raw to default valuew of "_" times 100
    ariel::Row::row_data = new string((unsigned int)ariel::Row::row_length, '_');

}

string ariel::Row::get_row() const{
    //return the raw_data
    return *row_data;
}
void ariel::Row::set_row( int start, const string& text) const{
    
    //set the raw_data to the text from the start position catch thr out of bound exception
    try{
        //place the string in the right place
        (*ariel::Row::row_data).replace((unsigned int)start, text.length(), text);
    }
    catch(out_of_range& e) {
        throw out_of_range("out of range");
    }
}
ariel::Row::~Row() {
    delete row_data;
}


/*----------------------------------------
                page
---------------------------------------*/
ariel::Page::Page(int page_number) {
    ariel::Page::page_number = page_number;
}
string ariel::Page::get_row( int row) {
    /*search in rows vector for a row with the given row number
    if found return the row data
    if not found create a new row with the given row number and return the row data
    */
    for(int i = 0; i < rows.size(); i++) {
        if((*rows.at((unsigned int)i)).row_number == row) {
            return *(*rows.at((unsigned int)i)).row_data;
        }
    }
    ariel::Row* new_row = new ariel::Row(row);
    rows.push_back(new_row);
    return *(*rows.at(rows.size()-1)).row_data;
}
void ariel::Page::set_row( int row,  int start, const string& text) {
    /*search in rows vector for a row with the given row number
    if found set the row data to the given text
    if not found create a new row with the given row number and set the row data to the given text
    */
    for(int i = 0; i < rows.size(); i++) {
        if((*rows.at((unsigned int)i)).row_number == row) {
            (*rows.at((unsigned int)i)).set_row(start,text);
            return;
        }
    }
    ariel::Row* new_row = new ariel::Row(row);
    rows.push_back(new_row);
    (*rows.at(rows.size()-1)).set_row(start,text);
}
ariel::Page::~Page() {
    delete rows.at(0);
}

/*----------------------------------------
            notebook
note book have pages that contains rows
---------------------------------------*/
ariel::Notebook::Notebook() {
    cout << "Notebook constructor" << endl;
}

ariel::Notebook::~Notebook() {
    cout << "Notebook destructor" << endl;
}

string ariel::Notebook::read( int page, int row, int column, Direction direction, int length)  {
    //chek if page or row are not bigger thean 0
    if(page < 0 || row < 0) {
        throw out_of_range("out of range");
    }

    if (column < 0 || column >= row_length ) {
        throw out_of_range("out of range");
    }
    if (direction == Direction::Horizontal){
        if (length <= 0 || length+column> row_length) {
            throw out_of_range("out of range");
        }
        //look for the page with the page number
        for (int i = 0; i < pages.size(); i++) {
            if ((*pages.at((unsigned int)i)).page_number == page) {
                return (*pages.at((unsigned int)i)).get_row(row).substr((unsigned int)column,(unsigned int)length);
            }
        }
        //if page not exist create new page
        ariel::Page *new_page = new ariel::Page(page);
        pages.push_back(new_page);
        return (*new_page).get_row(row).substr((unsigned int)column,(unsigned int)length);
        
    }
    
    if (length <= 0) {
            throw out_of_range("out of range");
    }
    //read vertical means that every char is in diffrent row but same column
    //look for the page with the page number
    //if page not exist create new page
    //if the line does not exist create new line
    //return the string
    string result;
    for (int i = 0; i < pages.size(); i++) {
        if ((*pages.at((unsigned int)i)).page_number == page) {
            for (int j=0; j<length; j++) {
                result.append((*pages.at((unsigned int)i)).get_row(row+j).substr((unsigned int)column,1));
            
            }
            return result;
        }
    }
    ariel::Page *new_page = new ariel::Page(page);
    pages.push_back(new_page);
    for (int j=0; j<length; j++) {
        result.append((*pages.at(pages.size()-1)).get_row(row+j).substr((unsigned int)column,1));
    
    }
    return result;
}

void ariel::Notebook::write( int page, int row, int column, Direction direction, const string& text)  {
    //check that text doesnot contain ~
    if (text.find('~') != string::npos) {
        throw invalid_argument("invalid argument");
    }
    //check legel symbol in text
    for (unsigned int i = 0; i < text.length(); i++) {
        if (!is_symbol(text.at(i))) {
            throw invalid_argument("invalid argument");
        }
    }
    int length = text.length();
    //check if the text is valid
    if(page < 0 || row < 0) {
        throw out_of_range("out of range");
    }
    for (unsigned int i = 0; i < length; i++) {
        if (!is_symbol(text[i])) {
           cout << "invalid symbol" << (int)text[i] <<endl;
        }
    }
    if (column < 0 || column >= row_length ) {
        throw out_of_range("out of range");
    }
    if (direction == Direction::Horizontal){
        if (length <= 0 || length+column> row_length) {
            throw out_of_range("out of range");
        }
        //check if row text is only _
        //if it isnt raise an error
        for (int i = 0; i < pages.size(); i++) {
            if ((*pages.at((unsigned int)i)).page_number == page) {
                string line = (*pages.at((unsigned int)i)).get_row(row);
                for (int j = column; j < column+length; j++) {
                    if (line.at((unsigned int)j) != '_') {
                        throw invalid_argument("cant write to this line");
                    }
                }
            }
        }
        //look for the page with the page number
        for (int i = 0; i < pages.size(); i++) {
            if ((*pages.at((unsigned int)i)).page_number == page) {
                (*pages.at((unsigned int)i)).set_row(row,column,text);
                return;
            }
        }
        //if page not exist create new page
        ariel::Page *new_page = new ariel::Page(page);
        pages.push_back(new_page);
        (*new_page).set_row(row,column,text);
        
    }
    else{
        if (length <= 0) {
             throw out_of_range("out of range");
        }
         //check if row text is only _
        //if it isnt raise an error
        for (int i = 0; i < pages.size(); i++) {
            if ((*pages.at((unsigned int)i)).page_number == page) {
                for (int j = 0; j < length; j++) {
                    string line = (*pages.at((unsigned int)i)).get_row(row+j);
                    //check only if col is _
                    if (line[(unsigned)column] != '_') {
                        throw invalid_argument("cant write to this col");
                    }
                }
            }
        }
        //write vertical means that every char is in diffrent row but same column
        //look for the page with the page number
        //if page not exist create new page
        //if the line does not exist create new line
        //return the string
        for (int i = 0; i < pages.size(); i++) {
            if ((*pages.at((unsigned int)i)).page_number == page) {
                for (int j=0; j<length; j++) {
                    (*pages.at((unsigned int)i)).set_row(row+j,column,text.substr((unsigned)j,1));
                }
                return;
            }
        }
        //if page not exist create new page
        ariel::Page *new_page = new ariel::Page(page);
        pages.push_back(new_page);
        for (int j=0; j<length; j++) {
            (*new_page).set_row(row+j,column,text.substr((unsigned)j,1));
        }

    }
}

void ariel::Notebook::erase( int page, int row, int column, Direction direction, int length)  {
    if(page < 0 || row < 0) {
        throw out_of_range("out of range");
    }
    if (column < 0 || column >= row_length ) {
        throw out_of_range("out of range");
    }
    if (direction == Direction::Horizontal){
        if (length <= 0 || length+column> row_length) {
            throw out_of_range("out of range");
        }
        //set a string var to be ~ timea len
        string text;
        text = string((unsigned)length, '~');
        //look for the page with the page number
        for (int i = 0; i < pages.size(); i++) {
            if ((*pages.at((unsigned int)i)).page_number == page) {
                (*pages.at((unsigned int)i)).set_row(row,column,text);
                return;
            }
        }
        //if page not exist create new page
        ariel::Page *new_page = new ariel::Page(page);
        pages.push_back(new_page);
        (*new_page).set_row(row,column,text);
        
    }
    else{
        if (length <= 0) {
             throw out_of_range("out of range");
        }
        //erase vertical means that every char is in diffrent row but same column
        //look for the page with the page number
        //if page not exist create new page
        //if the line does not exist create new line
        //return the string
        string text;
        text = string((unsigned int)length, '~');
        for (int i = 0; i < pages.size(); i++) {
            if ((*pages.at((unsigned int)i)).page_number == page) {
                for (int j=0; j<length; j++) {
                    (*pages.at((unsigned int)i)).set_row(row+j,column,text.substr((unsigned)j,1));
                }
                return;
            }
        }
        //if page not exist create new page
        ariel::Page *new_page = new ariel::Page(page);
        pages.push_back(new_page);
        for (int j=0; j<length; j++) {
            (*new_page).set_row(row+j,column,text.substr((unsigned)j,1));
        }

    }
}
void ariel::Notebook::show( int page) const {
    //check if page is positive
    if (page < 0) {
        throw invalid_argument("invalid page");
    }
    cout << "Page " << row_length << ":" << endl;
}


