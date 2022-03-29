/**
 *
 * AUTHORS: Aviad Gilboa
 * 
 * Date: 03-2022
**/ 

#include "doctest.h"
#include "Notebook.hpp"
using namespace ariel;
#include <string>
#include <algorithm>

/* Test cases for write function */

TEST_CASE("length of the line is between 0-99"){
    
    for ( int i=100; i<200;i++){
        Notebook note;
        try
        {
            note.write(1,i-100,0,Direction::Vertical,"Look at me");
            note.write(1,i-100,2,Direction::Horizontal,"I'm Mr. meeseeks");
        }
        catch(const std::exception& e)
        {
            CHECK_EQ(true,false);
        }
        
        CHECK_THROWS(note.write(1,i-100,i,Direction::Vertical,"o"));
        CHECK_THROWS(note.write(1,i-100,2,Direction::Vertical,"o"));
        CHECK_THROWS(note.write(1,i-100,98,Direction::Horizontal,"abc"));
    }
}
TEST_CASE("case: special chars"){
    Notebook note;
    note.write(1,0,0,Direction::Horizontal,"n____n");
    try{note.write(1,0,1,Direction::Horizontal,"abcd");}
    catch(std::exception &){
        CHECK_EQ(true,false);
    }
    CHECK_THROWS(note.write(1,1,1,Direction::Horizontal,"\n"));
}
TEST_CASE("Case: writing twice on the same position"){
    Notebook note;
     for (int i=0; i<96;i++){
        note.write(1,i,i,Direction::Horizontal,"abcd");
     }
     for (int i=1; i<96;i++){
         CHECK_THROWS(note.write(1,i-1,i,Direction::Vertical,"ab"));
         CHECK_THROWS(note.write(1,i-1,i,Direction::Vertical,"ab"));
     }
}

TEST_CASE("case: there is infinate rows"){
    Notebook note;
    std::string s;
    for (int i=1; i<10000;i++){s+="a";}
        try{
            note.write(0,0,35,Direction::Vertical,s);}
        catch(std::exception&){
            CHECK_EQ(true,false);
        }   
}

TEST_CASE("case: there is infinate pages"){
    Notebook note;
        try{
            note.write(5000,0,0,Direction::Horizontal,"%#!#");}
        catch(std::exception&){
            CHECK_EQ(true, false);
        }
}

/*Test cases for earse function  */
TEST_CASE("case: earse outside the line"){
    Notebook note;
    CHECK_THROWS(note.erase(0,4,98,Direction::Horizontal,3));
    CHECK_THROWS(note.erase(1,2,102,Direction::Vertical, 50));
    CHECK_THROWS(note.erase(1,2,100,Direction::Vertical, 5));
}

TEST_CASE("case: replace the current char to '~' "){
    Notebook note;
    note.write(1,2,55,Direction::Horizontal,"hello world");
    note.erase(1,2,60,Direction::Horizontal,6);
    CHECK_EQ(note.read(1,2,55,Direction::Horizontal,11),"hello~~~~~~");
    note.erase(1,2,53,Direction::Horizontal,2);
    CHECK_EQ(note.read(1,2,50,Direction::Horizontal,16),"___~~hello~~~~~~"); 
}
TEST_CASE("can't write again after earse"){
    Notebook note;
    try{
        note.write(1,2,55,Direction::Horizontal,"morty and jessica forever");
        note.erase(1,2,66,Direction::Horizontal,7);}
    catch(std::exception){
        CHECK_EQ(true,false);
    }
    CHECK_THROWS(note.write(1,0,55,Direction::Vertical,"this is not gonna work"));
        note.write(0,30,40,Direction::Horizontal,"my name is bird person");
    note.erase(0,30,51,Direction::Horizontal,11);
    note.write(0,30,62,Direction::Horizontal,"Phoenixperson");
    CHECK_EQ(note.read(0,30,40,Direction::Horizontal,35),"my name is ~~~~~~~~~~~Phoenixperson");
    CHECK_EQ(note.read(0,30,38,Direction::Horizontal,39),"__my name is ~~~~~~~~~~~Phoenixperson__");
}

/* Test cases for read function */
TEST_CASE("case: the empty cube represent by '_' "){
    Notebook note;
    std::string st;
    st = note.read(0,0,0,Direction::Horizontal,100);
    bool flag;
    for(unsigned long i=0; i<100;i++){
        flag = st[i]=='_';
        CHECK_FALSE(!flag);} 
    note.write(0,1,40,Direction::Vertical,"my name is bird person");
    note.erase(0,1,51,Direction::Vertical,11);
    note.write(0,1,62,Direction::Vertical,"Phoenixperson");
    st = note.read(0,1,0,Direction::Vertical,100);
    for(unsigned long i=0; i<40;i++){
        flag =st[i]=='_';
        CHECK_FALSE(!flag);}
    for(unsigned long i=76;i<100;i++){
        flag = st[i]=='_';
        CHECK_FALSE(!flag);}
}
TEST_CASE("case: want to read above the limits of the line"){
    Notebook note;
    CHECK_THROWS(note.read(0,1,100,Direction::Vertical,5));
    CHECK_THROWS(note.read(1,1,98,Direction::Horizontal,5));
}
