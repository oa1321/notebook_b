/**
 * @file Test.cpp
 * @author Ofri Tavor
 * @brief Test cases for the assignment
 * @date 2022-03
 */

#include "Direction.hpp"
using ariel::Direction;
#include "Notebook.hpp"
#include "doctest.h"
#include <string>
#define EMPTY_CHAR '_'
#define REMOVED_CHAR '~'
#define INVALID_LONG_STRING "GFDGDFGJDFGJKFDHGFDUGFDJKGFDLGJFDAAIGLHFDAGUJADFJSDABFDSFOSDJGUJFDHGFDGJFSD;LAGISFDOGFDIGHFDGJDFGFDA"

/**
 * This test case will check how the function deals with bad input.
 * The inputs might be invalid by themselves, like negative index of a row for example,
 * and they can also be valid inputs that together make the whole input invalid, like a word that
 * starts in the middle of the row and end after its end.
 */
TEST_CASE("Bad inputs")
{
    ariel::Notebook notebook;
    SUBCASE("Invalid Numbers")
    {
        SUBCASE("negative numeric inputs")
        {
            SUBCASE("Negative page index")
            {
                CHECK_THROWS(notebook.show(-1));
                CHECK_THROWS(notebook.write(-1, 1, 1, Direction::Horizontal, "rrrr"));
                CHECK_THROWS(notebook.write(-1, 1, 1, Direction::Vertical, "rrrr"));
                CHECK_THROWS(notebook.erase(-1, 1, 1, Direction::Vertical, 3));
                CHECK_THROWS(notebook.erase(-1, 1, 1, Direction::Horizontal, 3));
                CHECK_THROWS(notebook.read(-1, 1, 1, Direction::Horizontal, 5));
                CHECK_THROWS(notebook.read(-1, 1, 1, Direction::Vertical, 5));
            }
            SUBCASE("Negative row index")
            {
                CHECK_THROWS(notebook.write(1, -1, 1, Direction::Horizontal, "aaa"));
                CHECK_THROWS(notebook.write(1, -1, 1, Direction::Vertical, "aaa"));
                CHECK_THROWS(notebook.erase(1, -1, 1, Direction::Vertical, 3));
                CHECK_THROWS(notebook.erase(1, -1, 1, Direction::Horizontal, 3));
                CHECK_THROWS(notebook.read(1, -1, 1, Direction::Horizontal, 5));
                CHECK_THROWS(notebook.read(1, -1, 1, Direction::Vertical, 5));
            }
            SUBCASE("Negative column index")
            {
                CHECK_THROWS(notebook.write(1, 1, -1, Direction::Horizontal, "rrrr"));
                CHECK_THROWS(notebook.write(1, 1, -1, Direction::Vertical, "rrrr"));
                CHECK_THROWS(notebook.erase(1, 1, -1, Direction::Vertical, 3));
                CHECK_THROWS(notebook.erase(1, 1, -1, Direction::Horizontal, 3));
                CHECK_THROWS(notebook.read(1, 1, -1, Direction::Horizontal, 5));
                CHECK_THROWS(notebook.read(1, 1, -1, Direction::Vertical, 5));
            }
            SUBCASE("Negative Length")
            {
                CHECK_THROWS(notebook.read(1, 1, 1, Direction::Vertical, -5));
                CHECK_THROWS(notebook.read(1, 1, 1, Direction::Horizontal, -5));
                CHECK_THROWS(notebook.erase(1, 1, 1, Direction::Vertical, -5));
                CHECK_THROWS(notebook.erase(1, 1, 1, Direction::Horizontal, -5));
            }
        }
        /**
         * This sub case check the case that the column index is positive and within the given bounds.
         */
        SUBCASE("Invalid positive column check")
        {
            SUBCASE("Invalid column, everything else is valid")
            {
                CHECK_THROWS(notebook.write(1, 1, 150, Direction::Horizontal, "rrrr"));
                CHECK_THROWS(notebook.write(1, 1, 150, Direction::Vertical, "rrrr"));
                CHECK_THROWS(notebook.erase(1, 1, 150, Direction::Vertical, 3));
                CHECK_THROWS(notebook.erase(1, 1, 101, Direction::Horizontal, 3));
                CHECK_THROWS(notebook.read(1, 1, 101, Direction::Horizontal, 5));
                CHECK_THROWS(notebook.read(1, 1, 101, Direction::Vertical, 5));
            }
        }
        /**
         * This sub case will check if the function throw an exception in case that the column
         * and the length together will reach out of the given bounds.
         */
        SUBCASE("Bounds test-> the output should be out of bounds")
        {
            SUBCASE("Valid position and length: full row case")
            {
                CHECK_NOTHROW(notebook.read(1, 1, 0, Direction::Vertical, 101));
                CHECK_THROWS(notebook.read(1, 1, 0, Direction::Horizontal, 101));
                CHECK_NOTHROW(notebook.erase(1, 1, 0, Direction::Vertical, 101));
                CHECK_THROWS(notebook.erase(1, 1, 0, Direction::Horizontal, 101));
                CHECK_THROWS(notebook.write(1,1,0,Direction::Horizontal, INVALID_LONG_STRING));
            }
            SUBCASE("Valid column, valid length, but it will be out of bounds")
            {
                CHECK_THROWS(notebook.write(1, 1, 98, Direction::Horizontal, "rrrr"));
                CHECK_THROWS(notebook.erase(1, 1, 98, Direction::Horizontal, 3));
                CHECK_THROWS(notebook.read(1, 1, 98, Direction::Horizontal, 5));
            }
        }
    }
    SUBCASE("Invalid Characters -> ~"){
        ariel::Notebook notebook;
        CHECK_THROWS(notebook.write(1,1,0,Direction::Horizontal, "~~~~~"));
        CHECK_THROWS(notebook.write(1,1,0,Direction::Vertical, "~~~~~"));
        SUBCASE("Overwriting using ~ is not allowd")
        ariel::Notebook notebook;
        notebook.write(1,1,0,Direction::Horizontal, "hello world");
        CHECK_THROWS(notebook.write(1,1,0,Direction::Vertical, "~~~~~~~~~~~"));
        CHECK_NOTHROW(notebook.erase(1,1,0,Direction::Horizontal,11));
    }
}

/**
 * This test case will check how the functions will deal with problematic scenarios.
 */

TEST_CASE("Problematic scenarios")
{
    SUBCASE("Write in inavailable columns")
    {
        ariel::Notebook notebook;
        notebook.write(1, 1, 10, Direction::Horizontal, "hello world");
        CHECK_THROWS(notebook.write(1, 1, 10, Direction::Horizontal, "hello hello"));
        CHECK_THROWS(notebook.write(1, 1, 10, Direction::Vertical, "hello hello"));
        CHECK_THROWS(notebook.write(1, 1, 16, Direction::Horizontal, "hello"));
        CHECK_THROWS(notebook.write(1, 1, 16, Direction::Vertical, "hello"));
        CHECK_THROWS(notebook.write(1, 1, 8, Direction::Horizontal, "bye"));
        CHECK_THROWS(notebook.write(1, 1, 20, Direction::Horizontal, "bye"));
    }
    SUBCASE("Write on a removed column")
    {
        ariel::Notebook notebook;
        notebook.write(1, 1, 10, Direction::Horizontal, "hello world");
        notebook.erase(1, 1, 10, Direction::Horizontal, 11);
        CHECK_THROWS(notebook.write(1, 1, 10, Direction::Horizontal, "hello hello"));
        CHECK_THROWS(notebook.write(1, 1, 10, Direction::Vertical, "hello hello"));
        CHECK_THROWS(notebook.write(1, 1, 16, Direction::Horizontal, "hello"));
        CHECK_THROWS(notebook.write(1, 1, 16, Direction::Vertical, "hello"));
        CHECK_THROWS(notebook.write(1, 1, 8, Direction::Horizontal, "bye"));
        CHECK_THROWS(notebook.write(1, 1, 20, Direction::Horizontal, "bye"));
    }
    SUBCASE("Overwriting an 'empty' character - '_' is allowed"){
        ariel::Notebook notebook;
            notebook.write(1, 6, 0, Direction::Horizontal, "hello_hello");
            CHECK_NOTHROW(notebook.write(1,6,5,Direction::Horizontal, "^"));
            notebook.write(1, 7, 0, Direction::Horizontal, "hello_world");
            CHECK_NOTHROW(notebook.write(1,7,5,Direction::Vertical,"ABCD"));
    }
    SUBCASE("Erase an erased column throw an Unnecessary exception")
    {
        ariel::Notebook notebook;
        notebook.write(1, 1, 10, Direction::Horizontal, "hello world");
        notebook.erase(1, 1, 10, Direction::Horizontal, 11);
        CHECK_NOTHROW(notebook.erase(1, 1, 10, Direction::Horizontal, 11));
        CHECK_NOTHROW(notebook.erase(1, 1, 16, Direction::Horizontal, 5));
        CHECK_NOTHROW(notebook.erase(1, 1, 8, Direction::Horizontal, 3));
        CHECK_NOTHROW(notebook.erase(1, 1, 20, Direction::Horizontal, 3));
    }

    /**
     * This sub case check if the function can deal with "infinite" number of pages which contains "infinite"
     * number of rows. The function should not throw an exception.
     */
    
    SUBCASE("Dealing with rows or pages that does not exist->they should be since there are infinite rows and pages, even if they are not defined.")
    {
        ariel::Notebook notebook;
        CHECK_NOTHROW(notebook.erase(1, 1, 10, Direction::Horizontal, 23));
        CHECK_NOTHROW(notebook.erase(1, 2, 16, Direction::Horizontal, 12));
        CHECK_NOTHROW(notebook.erase(1, 3, 8, Direction::Horizontal, 30));
        CHECK_NOTHROW(notebook.write(1, 5, 25, Direction::Horizontal, "hello hello"));
        CHECK_NOTHROW(notebook.read(2, 1, 20, Direction::Horizontal, 30));
        CHECK_NOTHROW(notebook.read(1, 1, 25, Direction::Vertical, 30));
        CHECK_NOTHROW(notebook.show(3));
    }
}
