/******************************************************************************/
/*                                                                            */
/*    Copyright © 2024 SemWare Corporation. All rights reserved worldwide.    */
/*       SemWare is a registered trademark of Sammy and Bobbi Mitchell.       */
/*                                                                            */
/******************************************************************************/

#ifndef TEENY_HEADER_H
#define TEENY_HEADER_H

enum { Ident_max = 16 };

typedef char Ident[ Ident_max + 1 ];

typedef enum
{
  null, times, plus, rparen, lparen, number, writesym, eofsym
} Symbol;

class Scanner
{
public:
  bool init( const char fn[] );
  Symbol getsym( void );
  int getval( void ) { return val; }
  const char *getid( void ) { return id; }

private:
  void read_ch( void );
  Symbol get_ident( void );
  Symbol get_number( void );
  void enter_kw( Symbol sym, const char* name );
  enum { KW = 25 };
  int val;
  Ident id;
  int ch;
  int nkw;
  const char*  keyTab[ KW + 1 ];
  Symbol keySym[ KW + 1 ];
  FILE* file;
};

#endif  TEENY_HEADER_H
