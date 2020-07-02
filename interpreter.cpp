# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <iostream>
# include <cctype>
# include <exception>
# include <string>

// Standard Template Library
# include <list>
# include <map>

using namespace std;

// Global variables (Remember to put the prefix 'g')


/**
Error message
// Lexical error
Line X: unrecognized token with first char: '$'
// Syntactical error (token recognized)
Line X: unexpected token: '*'
// Semantic error (grammar ok)
Line X: undefined identifier: 'bcd'
*/
class LexicalErr {
public:
  LexicalErr( char ch ) {
    mC = ch;
    char *cstr = new char[100];
    sprintf( cstr, "Line %d: unrecognized token with first char: '%c'", mLine, mC );
    mErrMsg = cstr;
  } // end LexicalErr()

  string What() {
    return mErrMsg;
  } // end What()

private:
  char mC;
  int mLine;
  string mErrMsg;
}; // end class LexicalErr

class SyntacticalErr : public exception {
public:
  SyntacticalErr( char ch ) {
    int line = 0;
    printf( "Line %d: unexpected token: '%c'", line, ch );
  } // end SyntacticalErr()
}; // end class SyntacticalErr

class SemanticErr : public exception {
public:
  SemanticErr( string id ) {
    int line = 0;
    printf( "Line %d: undefined identifier: '%s'", line, id.c_str() );
  } // SemanticErr()
}; // end class SematicErr

// Declarations
union Num {
  int i;
  float f;
};

class Token {
public:
  Token( string name, int tkt ) {
    mName = name;
    mTokenType = tkt;
  } // end Token()

  Token( string name, int tkt, int val ) {
    mName = name;
    mTokenType = tkt;   
    mVal.i = val;
  } // end Token()

  Token( string name, int tkt, double val ) {
    mName = name;
    mTokenType = tkt;   
    mVal.f = ( float ) val;
  } // end Token()

  string mName;
  int mTokenType;
  Num mVal;
}; // class Token



class Context {
// public:
//   long getValue(string var);
//   long SetValue(string var);
// private:
  // map<string, Variable> mSymbolTables;
}; // class Context
// Scanner
//   |
// Parser
//   |

enum TokenType {
  // Operators
  // Arithmetic Operators: +, -, *, /, %, ++, --, +=, -=, *=, /=, %=
  ADD, SUB, MUL, DIV, MOD, INC, DEC, ADD_ASSIGN, SUB_ASSIGN,
  MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
  // Bitwise Operators: &, |, ^, ~, <<, >>, <<=, >>=
  AND, OR, XOR, COMP, SHL, SHR, SHL_ASSIGN, SHR_ASSIGN, 
  // Relational Operators: ==, !=, >, <, >=, <=
  EQ, NEQ, GT, LT, GE, LE,
  // Logical Operators: &&, ||, !
  LAND, LOR, NOT,
  // =
  ASSIGN,
  // Literals
  INT, FLOAT, BOOL, STRING, IDENTIFIER,
  // Keywords: if, else if, else, for, while, switch, break, do_while, goto
  IF, ELSE_IF, ELSE, FOR, WHILE, SWITCH, BREAK, DO_WHILE, GOTO,
  // Keywords: (, ), {, }, ;, ., *
  L_BRACE, R_BRACE, L_PAREN, R_PAREN, COMMA, DOT, STAR, RETURN,
  // System calls
  COUT, CIN
};

class Scanner {
public:
  Scanner( string src ) {
    mSource = src;
    mPos = 0;
    // System Call
    mKeywords["cout"] = COUT;

    // Control flow
    mKeywords["if"] = IF;
    mKeywords["else if"] = ELSE_IF;
    mKeywords["else"] = ELSE;
    mKeywords["while"] = WHILE;
    mKeywords["return"] = RETURN;
  } // end Scanner()

  void ScanTokens() {
    while ( mSource.length() > (unsigned int) mPos )
      ScanToken();
  } // end ScanTokens()

  void ScanToken() {
    int count = 1;
    char c = mSource[mPos];
    char nc = mSource[mPos + count];
    if ( c == '+' ) {
      if ( nc == '+' ) AddToken( "++", INC );
      else if ( nc == '=' ) AddToken( "+=", ADD_ASSIGN );
      else {
        AddToken( "+", ADD );
        count--; // return the overcharge
      } // end else

      count++;
    } // end if()
    else if ( c == '-' ) {
      if ( nc == '-' ) AddToken( "--", DEC );
      else if ( nc == '=' ) AddToken( "-=", SUB_ASSIGN );
      else if ( isdigit( nc ) ) AddNumToken( count ); // Negative
      else {
        AddToken( "-", SUB );
        count--; // return the overcharge
      } // end else

      count++;
    } // end else if()
    else if ( c == '*' ) AddToken( "*", MUL );
    else if ( c == '/' ) {
      if ( nc == '/' ) {
        while ( mSource[mPos + count] != '\n' && mSource[mPos + count] != '\0' )
          count++;
      } // end if()
      else AddToken( "/", DIV );
    } // end else if()
    else if ( c == '%' ) AddToken( "%", MOD );
    // Num
    else if (  isdigit( c ) )  AddNumToken( count );
    //
    else if ( c == '=' ) {
      if ( nc == '=' ) AddToken( "==", EQ );
      else AddToken( "=", ASSIGN);
    } // end else if()
    else if ( c == '!' ) {
      if ( '=' == c ) {
        AddToken( "!=", NEQ );
        count++;
      } // end if()
      else AddToken( "!", NOT );
    } // end if()
    else if ( '<' == c ) {
      if ( '=' == c ) {
        AddToken( "<=", LE );
        count++;
      } // end if()
      else AddToken( "<", LT );
    } // end if()
    else if ( '>' == c ) {
      if ( '=' == c ) {
        AddToken( ">=", GE );
        count++;
      } // end if()
      else AddToken( ">", GT );
    } // end if()
    

    // Key Symbols
    else if ( c == '(' ) AddToken( "(", L_BRACE );
    else if ( c == ')' ) AddToken( ")", R_BRACE );
    else if ( c == '{' ) AddToken( "{", L_BRACE );
    else if ( c == '}' ) AddToken( "}", L_BRACE );
    else if ( c == ';' ) AddToken( ";", COMMA );
    // Ignore
    else if ( c == ' ' || c == '\t' || c == '\r' ) ;
    else if ( c == '\n' ) ; //mLine++;
    else
      throw LexicalErr( c );
  
    mPos += count;
  } // end ScanToken() 

  void AddToken( string name, int tkt ) {
    Token tk( name, tkt );
    mTokens.push_back( tk );
  } // end AddToken();

  void AddToken( string name, int tkt, int val ) {
    Token tk( name, tkt, val );
    mTokens.push_back( tk );
  } // end AddToken();

  void AddToken( string name, int tkt, double val ) {
    Token tk( name, tkt, val );
    mTokens.push_back( tk );
  } // end AddToken();

  void AddNumToken( int &count )
  {
    bool isFloat = false;
    // Scan the series of number
    while ( isdigit( mSource[mPos + count] ) )
      count++;
    // determine int or float
    if ( mSource[mPos + count] == '.' )
    {
      count++; // include the dot
      while ( isdigit( mSource[mPos + count] ) )
        count++;
      isFloat = true;
    } // end if()

    // https://stackoverflow.com/questions/13294067/how-to-convert-string-to-char-array-in-c
    // https://stackoverflow.com/questions/7951019/how-to-convert-string-to-float
    string tmps = mSource.substr( mPos, count );
    char *cha = new char[tmps.length() + 1];
    strcpy( cha, tmps.c_str() );

    if ( isFloat )
      AddToken( tmps, FLOAT, atof( cha ) );
    else
      AddToken( tmps, INT, atoi( cha ) );
  } // end AddNumToken()

  void Print() {
    cout << "Scanner Source:" << endl;
    cout << mSource << endl;
    cout << "Scanner Token:" << endl;
    for ( list<Token>::iterator it = mTokens.begin() ; it != mTokens.end() ; it++ )
      cout << "name: " << it->mName << ", type: " << it->mTokenType << endl;
  } // end Print()

private:
  string mSource;
  int mPos;
  list<Token> mTokens;
  map<string, int> mKeywords;
}; // class Scanner



int main() {
  cout << "Hello World!\n";

  // init()

  while ( 1 ) {
    string src;
    cin >> src;
    try {
      Scanner scnr( src );
      scnr.ScanTokens();
      scnr.Print();
    }
    catch ( LexicalErr &e ) {
      cout << e.What() << endl;
    } // end catch ( LexicalErr &e )
  } // end while(1)

  // Context ctx();

  return 0;
} // end main()