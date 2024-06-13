# Grammar for teeny Pascal

| NonTerm           | Rule                                           |
| :---------------- | :--------------------------------------------: |
| Pascal            | = 'program' id ';' 'begin' Stmtseq 'end' '.' . |
| Stmtseq           | = Stmt {';' Stmt } .                           |
| Stmt              | = ['writeln' '(' String ')'] .                 |

[Tutorial Part 1: Lexical analysis.](https://borismix.github.io/SimpleCompiler/02-Parser.html)
