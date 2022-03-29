#include <iostream>
#include <stdexcept>
using namespace std;
#include <string>
#include <array>
#include <algorithm>
#include "Direction.hpp"
using ariel::Direction;

namespace ariel {
    class Row {

        public:
            int row_number;
            const  int row_length = 100;
            string *row_data;
            Row(int row_number);
            ~Row();
            string get_row() const;
            void set_row( int start, const string& text) const;
    };
    class Page {

        public:
            int page_number;
            vector<ariel::Row*> rows;
            const  int row_length = 100;
            Page(int page_number);
            ~Page();
            string get_row( int row );
            void set_row( int row,  int start, const string& text)  ;
    };
    class Notebook
    {
        public:
            const  int row_length = 100;
            vector<ariel::Page*> pages;
            Notebook();
            ~Notebook();
            void write( int page, int row, int column, Direction direction, const string& text)  ;
            string read( int page, int row, int column, Direction direction,  int length)  ;
            void erase( int page, int row, int column, Direction direction,  int length)  ;
            void show( int page) const ;
    };

    
}
