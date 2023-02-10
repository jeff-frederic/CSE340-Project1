/*
    Name: Jeffery Frederic
    ASU ID: 1218834552
    Course: CSE340 - Principles of Programming Languages
    Professor: James Gordon
    Semester: Spring 2023
    Due: Feb 12th, 2023
*/

#ifndef __INPUT_BUFFER__H__
#define __INPUT_BUFFER__H__

#include <string>

class InputBuffer {
  public:
    void GetChar(char&);
    char UngetChar(char);
    std::string UngetString(std::string);
    bool EndOfInput();

  private:
    std::vector<char> input_buffer;
};

#endif  //__INPUT_BUFFER__H__
