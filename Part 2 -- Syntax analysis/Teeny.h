/******************************************************************************/
/*                                                                            */
/*    Copyright © 2024 SemWare Corporation. All rights reserved worldwide.    */
/*       SemWare is a registered trademark of Sammy and Bobbi Mitchell.       */
/*                                                                            */
/******************************************************************************/

#ifndef TEENY_HEADER_H
#define TEENY_HEADER_H

enum { Ident_max = 16, Max_str = 200 };

typedef char Ident[ Ident_max + 1 ];

typedef enum { wrc = 1, wrl, hlt } Opcode;

typedef struct
{
  Opcode opcode;
  int operand;
} Instruction;

void interpret();

class Msg
{
public:
  Msg() { err_line = 0; err_col = 0; }
  void fatal( const char msg[] );
  void warn( const char msg[] );
  void set_line( int line ) { err_line = line; }
  void set_col( int col ) { err_col = col; }
private:
  int err_line, err_col;
};

class VirtualMachine
{
public:
  VirtualMachine() { code_index = 0; }
  void gen( Opcode opcode, int operand );
  const Instruction *getcode() { return code; }
private:
  enum { MAX_CODE = 500 };
  Instruction code[ MAX_CODE ];
  int code_index;
};

class Scanner
{
public:
  typedef enum
  {
    nul, lparen = '(', rparen = ')', times = '*', plus = '+', period = '.',
    semi = ';', numbersym, idsym, litstrsym, program, begin, end,
    writeln, eofsym
  } Symbol;
  bool init_scanner( const char fn[] );
  Symbol getsym();
  int getval() { return val; }
  const char *getid() { return id; }
  const char *getlitstr() { return lit_str; }
private:
  void read_ch();
  Symbol get_ident();
  Symbol get_number();
  Symbol get_string( int delimiter );
  int val;
  Ident id;
  char lit_str[ Max_str ];
  int ch;
  int cur_line, cur_col;
  FILE *f;
  static const char* key_words[];
  static Symbol key_syms[];
};

class Parser : Scanner
{
public:
  void parse( char fn[] );
private:
  Symbol sym;
  bool accept( Symbol s );
  void expect( Symbol s );
  bool stmt();
  void stmtseq();
};

#endif  TEENY_HEADER_H
