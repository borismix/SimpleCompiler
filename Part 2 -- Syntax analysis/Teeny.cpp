/******************************************************************************/
/*                                                                            */
/*    Copyright © 2024 SemWare Corporation. All rights reserved worldwide.    */
/*       SemWare is a registered trademark of Sammy and Bobbi Mitchell.       */
/*                                                                            */
/******************************************************************************/

// A simple parser for a language with identifiers and statements.

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "teeny.h"

VirtualMachine vm;
Msg error;

void Msg::fatal( const char msg[] )
{
  if( err_line == 0 )
    printf( msg );
  else
    printf( "line %d col %d %s\n", err_line, err_col, msg );
  exit( 2 );
}

void Msg::warn( const char msg[] )
{
  printf( "line %d col %d %s\n", err_line, err_col, msg );
  exit( 2 );
}

/* ---------------- Virtual Machine interpreter ---------------- */
void interpret()
{
  const Instruction *pc = vm.getcode();
  for( ;;)
  {
    switch( pc->opcode )
    {
      case wrc: printf( "%c", pc->operand ); break;
      case wrl: printf( "\n" ); break;
      case hlt: return;
      default: error.fatal( "whoops! unknown opcode" );
    }
    pc++;
  }
}

/* ---------------- Code generator ----------------------------*/
void VirtualMachine::gen( Opcode opcode, int operand = 0 )
{
  if( code_index < MAX_CODE )
  {
    code[ code_index ].opcode = opcode;
    code[ code_index ].operand = operand;
    code_index++;
  }
  else error.fatal( "program too long" );
}

/*---------------- Lexical analyzer --------------------------*/
const char* Scanner::key_words[] = { "begin", "end", "program", "writeln", "" };
Scanner::Symbol Scanner::key_syms[] = { begin, end, program, writeln, idsym };

void Scanner::read_ch()
{
  if( ch == '\n' )
  {
    cur_line++;
    cur_col = 0;
  }
  ch = fgetc( f );
  if( ch != EOF )
    cur_col++;
}

Scanner::Symbol Scanner::get_string( int delimiter )
{
  int i = 0;
  for( ;;)
  {
    read_ch();
    if( ch == delimiter )
    {
      read_ch();
      break;
    }
    if( ch == EOF )
    {
      error.warn( "eof in string" );
      break;
    }
    if( ch < ' ' ) error.warn( "illegal character in string" );
    if( i < Max_str )
    {
      lit_str[ i ] = ( char )ch;
      i++;
    }
  }
  lit_str[ i ] = '\0';
  return litstrsym;
}

Scanner::Symbol Scanner::get_ident()
{
  int i = 0;
  do
  {
    if( i < Ident_max )
    {
      id[ i ] = ( char )ch;
      i++;
    }
    read_ch();
  }
  while( ch != EOF && isalnum( ch ) );
  id[ i ] = '\0';
  for( i = 0;
    key_words[ i ][ 0 ] != '\0' && strcmp( id, key_words[ i ] ) != 0;
    i++ )
    ;
  return key_syms[ i ];
}

Scanner::Symbol Scanner::get_number()
{
  val = 0;
  do
  {
    val = 10 * val + ( ch - '0' );
    read_ch();
  }
  while( ch != EOF && isdigit( ch ) );
  return numbersym;
}

Scanner::Symbol Scanner::getsym()
{
  int save_ch;
  while( ch != EOF && ch <= ' ' )
    read_ch();
  error.set_line( cur_line ); error.set_col( cur_col );
  save_ch = ch;
  switch( ch )
  {
    case EOF: return eofsym;
    case '+':case '*':case '(':case ')':case '.':case ';':
      read_ch(); return ( Symbol )save_ch;
    case '\'': return get_string( ch );
    default:
      if( isalpha( ch ) ) return get_ident();
      if( isdigit( ch ) ) return get_number();
      error.warn( "illegal character" );
      read_ch();
      return getsym();
  }
}

bool Scanner::init_scanner( const char fn[] )
{
  if( ( f = fopen( fn, "r" ) ) == NULL ) return false;
  error.set_line( 1 );
  error.set_col( 0 );
  cur_line = 1;
  cur_col = 0;
  ch = ' ';
  read_ch();
  return true;
}

/*---------------- Parser ------------------------------------*/

bool Parser::accept( Symbol s )
{
  if( s == sym )
  {
    sym = getsym();
    return true;
  }
  return false;
}

void Parser::expect( Symbol s )
{
  if( !accept( s ) ) error.warn( "syntax error" );
}

// Stmt = ['writeln' '(' String ')'] .
bool Parser::stmt()
{
  if( accept( writeln ) )
  {
    expect( lparen );
    if( sym != litstrsym ) error.warn( "string expected" );
    const char *lit_str = getlitstr();
    for( int i = 0; lit_str[ i ] != '\0'; i++ )
      vm.gen( wrc, lit_str[ i ] );
    vm.gen( wrl );
    sym = getsym();
    expect( rparen );
    return true;
  }
  return false;
}

// Stmtseq = Stmt {';' Stmt } .
void Parser::stmtseq()
{
  do
  {
    stmt();
  }
  while( accept( semi ) );
}

// Pascal = 'program' id ';' 'begin' Stmtseq 'end' '.' .
void Parser::parse( char fn[] )
{
  if( !init_scanner( fn ) ) error.fatal( "no source" );
  sym = getsym();
  expect( program );
  expect( idsym );
  expect( semi );
  expect( begin );
  stmtseq();
  expect( end );
  vm.gen( hlt );
  expect( period );
}

/*---------------- Main driver --------------------------------*/

int main( int argc, char *argv[] )
{
  Parser parser;
  parser.parse( argv[ 1 ] );
  interpret();
  return 0;
}
