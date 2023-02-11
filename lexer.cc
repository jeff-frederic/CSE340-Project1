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
#include <cctype>
#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", 
    
    "REALNUM", "BASE08NUM", "BASE16NUM"     // New Tokens
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

// HELPER FUNCTIONS
bool checkBase16_alphabet(char c){return c == 'F' || c == 'C' || c == 'B' || c == 'E' || c == 'D' || c == 'A';}
bool checkBase16(char c){return isdigit(c) || checkBase16_alphabet(c);}

/**
 * Displaying TOKEN found as 
 * {token_lexeme, token_name, line_no}
*/
void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

/**
 * LexicalAnalyzer class constructor. 
 * Initializes the class variables accordingly.
*/
LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

/**
 * Iterate through a string till it 
 * arrives to the first character. It will
 * ignore all spaces found inside of the string.
*/
bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

/**
 * Checks if a string is a reserved keyword
 * in our system. If so, true, else, false.
*/
bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

/**
 * Returns TokenType of keyword if found
 * in the string. 
*/
TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

/**
 * Go through the input and begin scanning for numbers, 
 * trying to build REALNUM, BASE08NUM or BASE16NUM tokens.
*/
Token LexicalAnalyzer::ScanNumber()
{
    char c;
    
    // temporary characters that we will
    // use to check if its a BASE08NUM or
    // BASE16NUM and add to the lexeme after 
    // checking
	char ct;
    char ct2; 
    char ct3;

	char arr[100];
    
    input.GetChar(c);
    
    if (isdigit(c)) {
        
        if (c != '0') {
            tmp.lexeme = "";
            
            // Iterating through rest of input
            // checking if there is an 8, if so
            // then it's NOT a BASE08NUM (making b08 false)
            while (isdigit(c) && !input.EndOfInput()) {
                tmp.lexeme += c;
                input.GetChar(c);
		    }
            
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }

        } else {
            tmp.lexeme = "0";
        }

		input.GetChar(ct);
		
        if (ct == 'x'){
		    input.GetChar(ct2);
		    
            if(ct2 == '0'){
		        input.GetChar(ct3);
		        
                if (ct3 == '8'){                            // BASE08NUM Identified
                    tmp.lexeme = tmp.lexeme+ct+ct2+ct3;     // Updating lexeme
                    tmp.token_type = BASE08NUM;             // Updating token type
                    tmp.line_no = line_no;                  // Updating line number
                    return tmp;                             // Returning token

		        } else {input.UngetChar(ct3);}
		
                input.UngetChar(ct2);

		    } else if(ct2 == '1') {
		        input.GetChar(ct3);

		        if (ct3 == '6'){                            // BASE16NUM Identified
		            tmp.lexeme = tmp.lexeme+ct+ct2+ct3;     // Updating lexeme
			        tmp.token_type = BASE16NUM;             // Updating token type
                    tmp.line_no = line_no;                  // Updating line number
                    return tmp;                             // Returning token 

                } else { input.UngetChar(ct3); }

		        input.UngetChar(ct2);

		    } else {
		        input.UngetChar(ct2);
		    }
		
		    input.UngetChar(ct);


		} else if(checkBase16(ct)){
            int count = 0;
		    char arr[100];
			arr[count] = ct;
			
            // Iterates through 
            while(checkBase16(arr[count])){
               count++;
               input.GetChar(arr[count]);
            }
            
            ct = arr[count];
		    
            if(ct == 'x'){ 
		        
                input.GetChar(ct2);
			    
                if(ct2 == '1'){   
                    input.GetChar(ct3);
				
                    if (ct3 == '6'){                                    // BASE16NUM IDENTIFIED
                        
                        for(int i=0; i<count; i++){                     // Updating lexeme
                            tmp.lexeme = tmp.lexeme + arr[i];
                        }
                        
                        tmp.lexeme = tmp.lexeme + ct + ct2 + ct3;       // Updating lexeme
                        tmp.token_type = BASE16NUM;                     // Updating token type
                        tmp.line_no = line_no;                          // Updating line number
                        return tmp;                                     // return full token
                        
                    } else {
                        input.UngetChar(ct3);
                        input.UngetChar(ct2);	
                    }					
			    
                } else { input.UngetChar(ct2);}
			
			    input.UngetChar(ct);
		
            } else { input.UngetChar(ct);}
		
            count--;
            while(count>-1){
                input.UngetChar(arr[count]);
                count--;
            }

        } else if(ct == '.') {
            string lt = "";                 // temporary lexeme
            input.GetChar(ct2);
            
            // Check if digit after . (DOT)
            if(isdigit(ct2)){

                // Iterate through every char in the input
                // check to find if ending in 0, which is
                // NOT valid after . (DOT)
                while (isdigit(ct2) && !input.EndOfInput()){
                    lt += ct2;              // add to temp lexeme
                    input.GetChar(ct2); 
                }
                
                // Unget everything consumed
                if (!input.EndOfInput()){
                    input.UngetChar(ct2);
                }

            } else{ input.UngetChar(ct2); }

            input.UngetString(lt); 
            input.GetChar(ct2);
                
            if(isdigit(ct2)){                                       // REALNUM Identified
                tmp.lexeme += '.';
                
                // Iterating through the rest of the token. 
                // REALNUM already identified to be the token, need to add
                // the rest to lexeme.
                while (!input.EndOfInput() && (isdigit(ct2))){
                    tmp.lexeme += ct2;                                // Updating token lexeme
                    input.GetChar(ct2);
                }

                if (!input.EndOfInput()) {
                    input.UngetChar(ct2);
                }

                tmp.token_type = REALNUM;                           // Updating token type to REALNUM          
                tmp.line_no = line_no;                              // Updating line number
                return tmp;                                         // returning full token
            
            } else {
                input.UngetChar(ct2);
            }
        	   
            input.UngetChar(ct);

		} else{ input.UngetChar(ct); }
	
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
    
    } else {
        // Case where digit was found but lexer
        // was unable to scan for the type
        // of token that the number needs to be 
        // identified as. Therefore, token not found,
        // no lexeme returned.

        if (!input.EndOfInput()) { input.UngetChar(c);}
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
       
        return tmp;
    }
}

/**
 * Start checking input based on the 
 * saved keywords in our system. 
*/
Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}


/**
 * Return the previously retreived token. Used
 * to build a greater (bigger) token.
*/
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

/**
 * Retrieving single tokens from from every character
 * in the input stream. If none found immediately, it will
 * attempt to build a bigger token depending on the
 * rules set up. 
*/
Token LexicalAnalyzer::GetToken()
{
    char c;
    
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    
    while (token.token_type != END_OF_FILE){
        token = lexer.GetToken();
        token.Print();
    }
}
