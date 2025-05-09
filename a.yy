%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace{a_lang}
%define api.parser.class {Parser}
%define api.value.type variant
%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
	#include "ast.hpp"
	namespace a_lang {
		class Scanner;
	}

//The following definition is required when 
// we don't use the %locations directive (which we won't)
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

//End "requires" code
}

%parse-param { a_lang::Scanner &scanner }
%parse-param { a_lang::ProgramNode** root }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"
   #include "ast.hpp"
   #include "tokens.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

//%define parse.assert

/* Terminals 
 *  No need to touch these, but do note the translation type
 *  of each node. Most are just Token pointers, 
 *  and thus have no fields (other than line and column).
 *  Some terminals, like ID, have the translation type IDToken *,
 *  meaning the translation also has a name field. 
*/
%token                     END   0 "end file"
%token	<a_lang::Token *>       AND
%token	<a_lang::Token *>       ASSIGN
%token	<a_lang::Token *>       ARROW
%token	<a_lang::Token *>       BOOL
%token	<a_lang::Token *>       COLON
%token	<a_lang::Token *>       COMMA
%token	<a_lang::Token *>       CUSTOM
%token	<a_lang::Token *>       DASH
%token	<a_lang::Token *>       ELSE
%token	<a_lang::Token *>       EH
%token	<a_lang::Token *>       EQUALS
%token	<a_lang::Token *>       FALSE
%token	<a_lang::Token *>       FROMCONSOLE
%token	<a_lang::Token *>       GREATER
%token	<a_lang::Token *>       GREATEREQ
%token	<a_lang::IDToken *>     ID
%token	<a_lang::Token *>       IF
%token	<a_lang::Token *>       INT
%token	<a_lang::IntLitToken *> INTLITERAL
%token	<a_lang::Token *>       IMMUTABLE
%token	<a_lang::Token *>       LCURLY
%token	<a_lang::Token *>       LESS
%token	<a_lang::Token *>       LESSEQ
%token	<a_lang::Token *>       LPAREN
%token	<a_lang::Token *>       MAYBE
%token	<a_lang::Token *>       MEANS
%token	<a_lang::Token *>       NOT
%token	<a_lang::Token *>       NOTEQUALS
%token	<a_lang::Token *>       OR
%token	<a_lang::Token *>       OTHERWISE
%token	<a_lang::Token *>       CROSS
%token	<a_lang::Token *>       POSTDEC
%token	<a_lang::Token *>       POSTINC
%token	<a_lang::Token *>       RETURN
%token	<a_lang::Token *>       RCURLY
%token	<a_lang::Token *>       REF
%token	<a_lang::Token *>       RPAREN
%token	<a_lang::Token *>       SEMICOL
%token	<a_lang::Token *>       SLASH
%token	<a_lang::Token *>       STAR
%token	<a_lang::StrToken *>    STRINGLITERAL
%token	<a_lang::Token *>       TOCONSOLE
%token	<a_lang::Token *>       TRUE
%token	<a_lang::Token *>       VOID
%token	<a_lang::Token *>       WHILE

/* Nonterminals
*  The specifier in angle brackets
*  indicates the type of the translation attribute.
*  TODO: You will need to add more attributes for other nonterminals
*  to this list as you add productions to the grammar
*  below (along with indicating the appropriate translation
*  attribute type).
*/
/*       (attribute type)    (nonterminal)    */
%type <a_lang::ProgramNode *> program
%type <std::list<a_lang::DeclNode *> *> globals
%type <a_lang::DeclNode *> decl
%type <a_lang::VarDeclNode *> varDecl
%type <a_lang::TypeNode *> type
%type <a_lang::TypeNode *> datatype
%type <a_lang::TypeNode *> primType
%type <a_lang::LocNode *> loc
%type <a_lang::IDNode *> name

%right ASSIGN
%left OR
%left AND
%nonassoc LESS GREATER LESSEQ GREATEREQ EQUALS NOTEQUALS
%left DASH CROSS
%left STAR SLASH
%left NOT 

%%


program		: globals
		  {
		  $$ = new ProgramNode($1);
		  *root = $$;
		  }

globals		: globals decl
		  {
		  $$ = $1;
		  DeclNode * declNode = $2;
		  $$->push_back(declNode);
		  }
		| /* epsilon */
		  {
		  $$ = new std::list<DeclNode *>();
		  }

decl		: varDecl SEMICOL
		  {
		  $$ = $1;
		  }
		| classTypeDecl
		  {
		  }
		| fnDecl
		  {
		  }

varDecl		: name COLON type
		  {
		  const Position * p;
		  p = new Position($1->pos(), $2->pos());
		  $$ = new VarDeclNode(p, $1, $3);
		  }
		| name COLON type ASSIGN exp
		  {
		  }

type		: IMMUTABLE datatype
		  {
		  }
		| datatype
		  {
		  $$ = $1;
		  }

datatype	: REF primType
		  {
		  }
		| primType
		  {
		  $$ = $1;
		  }
		| REF name
		  {
		  }
		| name
		  {
		  }

primType	: INT
		  {
		  $$ = new IntTypeNode($1->pos());
		  }
		| BOOL
		  {
      $$ = new BoolTypeNode($1->pos());
		  }
		| VOID
		  {
		  }

classTypeDecl 	: name COLON CUSTOM LCURLY classBody RCURLY SEMICOL
		  {
		  }

classBody 	: classBody varDecl SEMICOL
		  {
		  //TODO
		  }
		| classBody fnDecl
		  {
		  //TODO
		  }
		| /* epsilon */
		  {
		  }

fnDecl 		: name COLON LPAREN maybeFormals RPAREN ARROW type LCURLY stmtList RCURLY
		  {
		  }

maybeFormals	: /* epsilon */
		  {
		  }
		| formalList
		  {
		  }

formalList	: formalDecl
		  {
		  }
		| formalList COMMA formalDecl
		  {
		  }

formalDecl	: name COLON type
		  {
		  }

stmtList	: /* epsilon */
		  {
		  }
		| stmtList stmt SEMICOL
		  {
		  }
		| stmtList blockStmt
		  {
		  }

blockStmt	: WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY
		  {
		  }
		| IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY
		  {
		  }

stmt		: varDecl
		  {
		  }
		| loc ASSIGN exp
		  {
		  }
		| callExp
		  {
		  }
		| loc POSTDEC
		  {
		  }
		| loc POSTINC
		  {
		  }
		| TOCONSOLE exp
		  {
		  }
		| FROMCONSOLE loc
		  {
		  }
		| MAYBE exp MEANS exp OTHERWISE exp
		  {
		  }
		| RETURN exp
		  {
		  }
		| RETURN
		  {
		  }


exp		: exp DASH exp
		  {
		  }
		| exp CROSS exp
		  {
		  }
		| exp STAR exp
		  {
		  }
		| exp SLASH exp
		  {
		  }
		| exp AND exp
		  {
		  }
		| exp OR exp
		  {
		  }
		| exp EQUALS exp
		  {
		  }
		| exp NOTEQUALS exp
		  {
		  }
		| exp GREATER exp
		  {
		  }
		| exp GREATEREQ exp
		  {
		  }
		| exp LESS exp
		  {
		  }
		| exp LESSEQ exp
		  {
		  }
		| NOT exp
		  {
		  }
		| DASH term
		  {
		  }
		| term
		  {
		  }

callExp		: loc LPAREN RPAREN
		  {
		  }
		| loc LPAREN actualList RPAREN
		  {
		  }

actualList	: exp
		  {
		  }
		| actualList COMMA exp
		  {
		  }

term 		: loc
		  {
		  }
		| INTLITERAL
		  {
		  }
		| STRINGLITERAL
		  {
		  }
		| TRUE
		  {
		  }
		| FALSE
		  {
		  }
		| EH
		  {
		  }
		| LPAREN exp RPAREN
		  {
		  }
		| callExp 
		  {
		  }

loc		: name
		  {
		  $$ = $1;
		  }
		| loc ARROW name
		  {
		  }

name		: ID
		  {
                  const Position * pos = $1->pos();
	 	  $$ = new IDNode(pos, $1->value());
		  }
	
%%

void a_lang::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
