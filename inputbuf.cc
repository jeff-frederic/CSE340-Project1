/*
    Name: Jeffery Frederic
    ASU ID: 1218834552
    Course: CSE340 - Principles of Programming Languages
    Professor: James Gordon
    Semester: Spring 2023
    Due: Feb 12th, 2023
*/

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cstdio>

#include "inputbuf.h"

using namespace std;

bool InputBuffer::EndOfInput()
{
    if (!input_buffer.empty())
        return false;
    else
        return cin.eof();
}

char InputBuffer::UngetChar(char c)
{
    if (c != EOF)
        input_buffer.push_back(c);;
    return c;
}

void InputBuffer::GetChar(char& c)
{
    if (!input_buffer.empty()) {
        c = input_buffer.back();
        input_buffer.pop_back();
    } else {
        cin.get(c);
    }
}

string InputBuffer::UngetString(string s)
{
    for (int i = 0; i < s.size(); i++)
        input_buffer.push_back(s[s.size()-i-1]);
    return s;
}
