grammar RPG ;

// ALPHABET AND NUMBERS
digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
lowercase_letter = "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | 
					"l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" ;
uppercase_letter = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | 
					"L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" ;
german_letter = "ä" | "ö" | "ü" | "Ä" | "Ö" | "Ü" | "ß" ;
letter = lowercase_letter | uppercase_letter | german_letter ;

symbol = "+" | "#" | "-" | "!" | "\"" | "§" | "$" | "%" | "&" | "/" | "(" | ")" | "=" | "?" | "*" | "'" | "_" | "{" | "[" | "]" | "}" | "\\" | "~" | "<" | ">" | ";" | ":" | "|" ;
number = digit, { digit } ;


// CODE
int = number ;
int64 = number ;
uint = number ;
uint64 = number ;
bool = 'false' | 'true' ;
char = lowercase_letter | uppercase_letter | symbol ;
wchar = letter | symbol ;
string = letter | symbol, { letter | symbol } ;
type = int | int64 | uint | uint64 | bool | char | wchar | string ;

kw_types = 'int' | 'int64' | 'uint' | 'uint64' | 'bool' | 'char' | 'wchar' | 'string' ;
kw_if = 'if' ;
kw_if_else = 'if else' ;
kw_else = 'else' ;
kw_while = 'while' ;
kw_for = 'for' ;
kw_return = 'return' ;
kw_const = 'const' ;
kw_ptr = '*' ;
kw_ref = '&' ;

instruction = kw_if | kw_if_else | kw_else | kw_while | kw_for | kw_return ;

code_letter = lowercase_letter | uppercase_letter ;
identifier = code_letter, { code_letter | digit } ;
assignment = identifier, '=', identifier | number | bool | char | wchar | string, ';' ;

body = { assignment | identifier | instruction } ;
function = type, identifier, '(', { identifier }, ')', '{', body, '}' ;


// CONTROL FLOW
condition_op = '==' | '<' | '>' | '<=' | '>=' ;
condition = identifier | number, condition_op, identifier | number ;
if_condition = 'if', '(', condition, ')', '{', body, '}', { 'else if', '(', condition, ')', '{', body, '}' }, { 'else', '{', body, '}' } ;
for_loop = 'for', '(', [assignment], ";", condition, ";", assignment, ')', '{', body, '}' ;
while_loop = 'while', '(', condition, ')', '{', body, '}' ;
return = 'return', [ identifier | type ], ';'


// ARITHMETIC
addition = identifier | number, "+", identifier | number ;
subtraction = identifier | number, "-", identifier | number ;
multiplication = identifier | number, "*", identifier | number ;
division = identifier | number, "/", identifier | number ;

bool_flip = '!', identifier ;

// GAME
decl_npc = 'NPC', '{', body, '}' ;
decl_item = 'ITEM', '{', body, '}' ;