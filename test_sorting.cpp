#include "Wordlist.h"

/*
    test_read() is a helper function that reads words, one at a time, from cin,
    into a Wordlist and print the stats. You can use it to help test your Wordlist class, e.g.:

    > make test_sorting
    g++ -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g   test_sorting.cpp   -o test_sorting
   
    > ./test_sorting < testText.txt
    Number of different words: 6
    Total number of words: 6
    Most frequent word: Hello, 1
    Number of singletons: 6 (100%)
*/

 void test_read()
{
    Wordlist lst;
    string w;
    while (cin >> w)
    {
        lst.add_word(w);
    }

    lst.print_stats();
}

int main()
{
    Wordlist lst("testText.txt");
    lst.print_words();
}