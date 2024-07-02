# Part 3: More about grammars.

| NonTerm           | Rule                                                       |
| :---------------- | :----------------------------------------------------------|
| Teeny             | = 'program' ident ';' CmpndStmt '.' .                      |
| CmpndStmt         | = 'begin' [Stmt {';' Stmt }] 'end' .                       |
| Stmt              | = Writeln .                                                |
| Writeln           | = 'writeln' '(' [(string|Expr) {',' (string|Expr) }] ')' . |
| Expr              | = Term { ('+' &#124; '-') Term } .                              |
| Term              | = Factor { ('*' &#124; '/') Factor } .                          |
| Factor            | = number                                                   |
|                   |   &#124; ('+' &#124; '-') Factor                                     |
|                   |   &#124; '(' Expr ')'                                           |
|                   |   .                                                        |


[Tutorial Part 3: More about grammars.](https://borismix.github.io/SimpleCompiler/03-Grammar.html)
