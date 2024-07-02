# Part 3: More about grammars.

Teeny     = 'program' ident ';' CmpndStmt '.' .
CmpndStmt = 'begin' [Stmt {';' Stmt }] 'end' .
Stmt      = Writeln .
Writeln   = 'writeln' '(' [(string|Expr) {',' (string|Expr) }] ')' .
Expr      = Term { ('+' | '-') Term } .
Term      = Factor { ('*' | '/') Factor } .
Factor    = number
           | ('+' | '-') Factor
           | '(' Expr ')'
           .
