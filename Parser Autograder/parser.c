#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below

struct SymbolTable{
    int staticCount;
    int fieldCount;
    struct{
        char kind[50];
        char type[50];
        char name[50];
        int index;
    } total_count;
};
struct SymbolTable table = {0, 0, {"", "", "", 0}};

ParserInfo operand();
ParserInfo factor();
ParserInfo expression();
ParserInfo relationalExpression();
ParserInfo ArthimeticExpression();
ParserInfo term();
ParserInfo expressionList();
ParserInfo statement();
ParserInfo varDeclarStatement();
ParserInfo letStatement();
ParserInfo ifStatement();
ParserInfo whileStatement();
ParserInfo doStatement();
ParserInfo returnStatement();
ParserInfo class_Declar();
ParserInfo memberDeclar();
ParserInfo classVarDeclar();
ParserInfo SubroutineDeclar();
ParserInfo paramList();
ParserInfo subroutineCall();
ParserInfo subroutineBody();
Token t;

const char* subroutine_declars[] = {"constructor", "method", "function"};
int subroutine_declars_num = 3;

const char* classVar_declars[] = {"static", "field"};
int classVar_declars_num = 2;

const char* types[] = {"boolean", "int", "char"};
int types_num = 3;

const char* statments_list[] = {"var", "let", "if", "while", "do", "return"};
int statements_num = 6;

const char* operand_list[] = {"null", "this", "true", "false","("};
int operand_num = 5;

int IsSubroutine(char* str)
{
	for (int i = 0; i < subroutine_declars_num; i++)
		if (!strcmp(subroutine_declars[i], str))
			return 1;
	return 0;
}
int IsClassVar(char* str)
{
	for (int i = 0; i < classVar_declars_num; i++)
		if (!strcmp(classVar_declars[i], str))
			return 1;
	return 0;
}
int Istype(Token t1)
{
	
	for (int i = 0; i < types_num; i++)
		if (!strcmp(types[i], t1.lx))
			return 1;
	if (t1.tp == ID)
		return 1;
			
	return 0;
}
int IsStatement(char* str)
{
	for (int i = 0; i < statements_num; i++)
		if (!strcmp(statments_list[i], str))
			return 1;
	return 0;
}
int IsFactor(Token t1)
{
	
	for (int i = 0; i < operand_num; i++)
		if (!strcmp(operand_list[i], t1.lx))
			return 1;
	if (!strcmp(t1.lx, "-"))
		return 1;
	else if (!strcmp(t1.lx, "~"))
		return 1;
	else if (t1.tp == ID)
		return 1;
	else if (t1.tp == STRING)
		return 1;
	else if (t1.tp == INT)
		return 1;
	else		
		return 0;
}
int IsOperand(Token t1)
{
	
	for (int i = 0; i < operand_num; i++)
		if (!strcmp(operand_list[i], t1.lx))
			return 1;
	if (t1.tp == ID)
		return 1;
	else if (t1.tp == STRING)
		return 1;
	else if (t1.tp == INT)
		return 1;
	else		
		return 0;
}


int InitParser (char* file_name)
{
	InitLexer(file_name);
	return 1;
}

ParserInfo Parse ()
{
	ParserInfo pi;

	// implement the function


	pi.er = none;
	pi.tk = t;
	pi = class_Declar();
	return pi;
}


int StopParser ()
{
	StopLexer();
	return 1;
}

ParserInfo class_Declar()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	char class_id[256];
	t = GetNextToken();
    if(t.tp == ERR)
	{
        if(pi.er == none){
            pi.er = lexerErr;
            pi.tk = t;
            return pi;
        }
    }
    if(!strcmp(t.lx, "class"))
	{
        ;
    }
    else
	{
        if(pi.er == none)
		{
            pi.er = classExpected;
            pi.tk = t;
            return pi;
        }
	}
	t = GetNextToken();
	if (t.tp == ID)
	{
		strcpy(class_id, t.lx);
	}
	else
	{
		if(pi.er == none)
		{
			pi.er = idExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = GetNextToken();
	if (!strcmp(t.lx, "{"))
	{
		;
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	while (IsClassVar(t.lx) || IsSubroutine(t.lx))
	{
		pi = memberDeclar();
		t = pi.tk;
		t = PeekNextToken();
	}
	
	t = GetNextToken();
	if (!strcmp(t.lx, "}"))
	{
		;
	}
	else 
	{
		if(pi.er == none)
		{
			pi.er = closeBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}

ParserInfo memberDeclar()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (IsClassVar(t.lx))
	{
		pi = classVarDeclar();
		t = pi.tk;
		
	}
	else if (IsSubroutine(t.lx))
	{
		pi = SubroutineDeclar();
		t = pi.tk;
		
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = memberDeclarErr;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}

ParserInfo classVarDeclar()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (IsClassVar(t.lx))
	{
		t = GetNextToken();
		
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = classVarErr;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (Istype(t))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = illegalType;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (t.tp == ID)
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = idExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	while (!strcmp(t.lx, ",")) // if the next token is "," enter in a loop
	{
		t = GetNextToken(); //consume ","
		t = PeekNextToken();
		if (t.tp == ID)
		{
			t = GetNextToken();
		}
		else 
		{
			if (pi.er == none)
			{
				pi.er = idExpected;
				pi.tk = t;
				return pi;
			}
		}
		t = PeekNextToken(); //check the next token without consuming if it was a "," then the loop continues
	}
	
	if (!strcmp(t.lx, ";"))
	{
		t = GetNextToken();
	}
	else 
	{
		if (pi.er == none)
		{
			pi.er = semicolonExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}

ParserInfo SubroutineDeclar()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;
	t = PeekNextToken();
	if (IsSubroutine(t.lx))
	{
		t = GetNextToken();
		
	}
	else
	{
		if (pi.er == none)
			{
				pi.er = subroutineDeclarErr;
				pi.tk = t;
				return pi;
			}
	}
	t = PeekNextToken();
	if (Istype(t))
	{
		t = GetNextToken();
	}
	else if (!strcmp(t.lx,"void"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = illegalType;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (t.tp == ID)
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = idExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"("))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openParenExpected;
			pi.tk = t;
			return pi;
		}
	}
	
	
	pi = paramList();
	t = pi.tk;
	
	t = PeekNextToken();
	if (!strcmp(t.lx,")"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = closeParenExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"{"))
	{
		pi = subroutineBody();
		t = pi.tk;
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	
	
	pi.tk = t;
	return pi;
}

ParserInfo paramList()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;
	t = PeekNextToken();
	if (Istype(t))
	{
		t = GetNextToken();
		t = PeekNextToken();
		if (t.tp == ID)
		{
			t = GetNextToken();
		}
		else
			{
				if (pi.er == none)
				{
					pi.er = idExpected;
					pi.tk = t;
					return pi;
				}
			}
		t = PeekNextToken();
		while (!strcmp(t.lx,","))
		{
			t = GetNextToken();
			t = PeekNextToken();
			if (Istype(t))
			{
				t = GetNextToken();
			}
			else
			{
				if (pi.er == none)
				{
					pi.er = illegalType;
					pi.tk = t;
					return pi;
				}
			}
			t = PeekNextToken();
			if (t.tp == ID)
			{
				t = GetNextToken();
			}
			else
			{
				if (pi.er == none)
				{
					pi.er = idExpected;
					pi.tk = t;
					return pi;
				}
			}
			t = PeekNextToken();
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo subroutineBody()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;
	t = GetNextToken();

	t = PeekNextToken();
	while (IsStatement(t.lx))
	{
		pi = statement();
		t = pi.tk;
		if (pi.er != none) return pi;
		t = PeekNextToken();
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"}"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = closeBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;	
	return pi;
}

ParserInfo statement()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;

	if (!strcmp(t.lx,"var"))
	{
		pi = varDeclarStatement();
		if (pi.er != none) return pi;
	}
	else if (!strcmp(t.lx,"let"))
	{
		pi = letStatement();
		if (pi.er != none) return pi;
	}
	else if (!strcmp(t.lx,"if"))
	{
		pi = ifStatement();
		if (pi.er != none) return pi;
	}
	else if (!strcmp(t.lx,"while"))
	{
		pi = whileStatement();
		if (pi.er != none) return pi;
	}
	else if (!strcmp(t.lx,"do"))
	{
		pi = doStatement();
		if (pi.er != none) return pi;
	}
	else if (!strcmp(t.lx,"return"))
	{
		pi = returnStatement();
		if (pi.er != none) return pi;
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = syntaxError;
			pi.tk = t;
			return pi;
		}
	}

	return pi;
}

ParserInfo varDeclarStatement()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;
	t = PeekNextToken();
	if (!strcmp(t.lx,"var"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = syntaxError;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (Istype(t))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = illegalType;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (t.tp == ID)
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = idExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	while (!strcmp(t.lx,","))
	{
		t = GetNextToken();
		t = PeekNextToken();
		if (t.tp == ID)
		{
			t = GetNextToken();
		}
		else
		{
			if (pi.er == none)
			{
				pi.er = idExpected;
				pi.tk = t;
				return pi;
			}
		}
		t = PeekNextToken();
	}
	
	if (!strcmp(t.lx,";"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
			{
				pi.er = semicolonExpected;
				pi.tk = t;
				return pi;
			}
	}
	pi.tk = t;
	return pi;
}
ParserInfo letStatement()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;
	t = PeekNextToken();
	if (!strcmp(t.lx,"let"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = syntaxError;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (t.tp == ID)
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = idExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"["))
	{
		t = GetNextToken();
		
		pi = expression();
		if (pi.er != none) return pi;
		t = pi.tk;
		
		t = PeekNextToken();
		if (!strcmp(t.lx,"]"))
		{
			t = GetNextToken();
		}
		else
		{
			if (pi.er == none)
			{
				pi.er = closeBracketExpected;
				pi.tk = t;
				return pi;
			}
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"="))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = equalExpected;
			pi.tk = t;
			return pi;
		}
	}
		
	pi = expression();
	if (pi.er != none) return pi;
	t = pi.tk;
		
	t = PeekNextToken();
	if (!strcmp(t.lx,";"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = semicolonExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo ifStatement()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;
	t = PeekNextToken();
	if (!strcmp(t.lx,"if"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = syntaxError;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"("))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openParenExpected;
			pi.tk = t;
			return pi;
		}
	}
	
	pi = expression();
	t = pi.tk;
	
	t = PeekNextToken();
	if (!strcmp(t.lx,")"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = closeParenExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"{"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	while (IsStatement(t.lx))
	{
		pi = statement();
		t = pi.tk;
		t = PeekNextToken();
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"}"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = closeBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"else"))
	{
		t = GetNextToken();
		t = PeekNextToken();
		if (!strcmp(t.lx,"{"))
		{
			t = GetNextToken();
		}
		else
		{
			if (pi.er == none)
			{
				pi.er = openBraceExpected;
				pi.tk = t;
				return pi;
			}
		}
		t = PeekNextToken();
		while (IsStatement(t.lx))
		{
			pi = statement();
			t = pi.tk;
			t = PeekNextToken();
		}
		t = PeekNextToken();
		if (!strcmp(t.lx,"}"))
		{
			t = GetNextToken();
		}
		else
		{
			if (pi.er == none)
			{
				pi.er = closeBraceExpected;
				pi.tk = t;
				return pi;
			}
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo whileStatement()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (!strcmp(t.lx,"while"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = syntaxError;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"("))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openParenExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (IsFactor(t))
	{
		pi = expression();
		t = pi.tk;
		
	}
	else
	{
		if (pi.er == none)
			{
				pi.er = syntaxError;
				pi.tk = t;
				return pi;
			}	
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,")"))
	{
		t = GetNextToken();
	}
	else
	{
		
			if (pi.er == none)
			{
				pi.er = closeParenExpected;
				pi.tk = t;
				return pi;
			}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"{"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	while (IsStatement(t.lx))
	{
		pi = statement();
		t = pi.tk;
		t = PeekNextToken();
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"}"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = closeBraceExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo doStatement()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (!strcmp(t.lx,"do"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = syntaxError;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (t.tp == ID)
	{
		pi = subroutineCall();
		t = pi.tk;
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = idExpected;
			pi.tk = t;
			return pi;
		}
	}
	
	t = PeekNextToken();
	if (!strcmp(t.lx,";"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = semicolonExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo subroutineCall()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (t.tp == ID)
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = idExpected;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"."))
	{
		t = GetNextToken();
		t = PeekNextToken();
		if (t.tp == ID)
		{
			t = GetNextToken();
		}
		else
		{
			if (pi.er == none)
			{
				pi.er = idExpected;
				pi.tk = t;
				return pi;
			}
		}

	}
	t = PeekNextToken();
	if (!strcmp(t.lx,"("))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = openParenExpected;
			pi.tk = t;
			return pi;
		}
	}
	
	pi = expressionList();
	if (pi.er != none) return pi;
	t = pi.tk;
		
	t = PeekNextToken();
	if (!strcmp(t.lx,")"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = closeParenExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo expressionList()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (IsFactor(t))
	{
		pi = expression();
		if (pi.er != none) return pi;
		t = pi.tk;
		t = PeekNextToken();
		while (!strcmp(t.lx,","))
		{
			t = GetNextToken();
			pi = expression();
			t = pi.tk;
			if (pi.er != none) return pi;
			t = PeekNextToken();
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo returnStatement()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;	
	t = PeekNextToken();
	if (!strcmp(t.lx,"return"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = syntaxError;
			pi.tk = t;
			return pi;
		}
	}
	t = PeekNextToken();
	if (IsFactor(t))
	{
		pi = expression();
		t = pi.tk;
	}
	t = PeekNextToken();
	if (!strcmp(t.lx,";"))
	{
		t = GetNextToken();
	}
	else
	{
		if (pi.er == none)
		{
			pi.er = semicolonExpected;
			pi.tk = t;
			return pi;
		}
	}
	pi.tk = t;
	return pi;
}
ParserInfo expression()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	
	pi = relationalExpression();
	t = pi.tk;
	
	t = PeekNextToken();
	while (!strcmp(t.lx,"&") || !strcmp(t.lx,"|"))
	{
		t = GetNextToken();
		
		pi = relationalExpression();
		t = pi.tk;
			
		t = PeekNextToken();
	}
	pi.tk = t;
	return pi;
}
ParserInfo relationalExpression()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	
	pi = ArthimeticExpression();
	t = pi.tk;
	
	t = PeekNextToken();
	while (!strcmp(t.lx,"=" ) || !strcmp(t.lx,">" ) || !strcmp(t.lx,"<" ))
	{
		t = GetNextToken();

		pi = ArthimeticExpression();
		t = pi.tk;
		
		t = PeekNextToken();
	}
	pi.tk = t;
	return pi;
}
ParserInfo ArthimeticExpression()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;

	pi = term();
	t = pi.tk;
	
	t = PeekNextToken();
	while (!strcmp(t.lx,"+" ) || !strcmp(t.lx,"-" ))
	{
		t = GetNextToken();
		
		pi = term();
		t = pi.tk;
			
		t = PeekNextToken();
	}
	pi.tk = t;
	return pi;
}
ParserInfo term()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	
	pi = factor();
	t = pi.tk;
	
	t = PeekNextToken();
	while (!strcmp(t.lx,"*" ) || !strcmp(t.lx,"/" ))
	{
		t = GetNextToken();
		
		pi = factor();
		t = pi.tk;
			
		t = PeekNextToken();
	}
	pi.tk = t;
	return pi;
}
ParserInfo factor()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (!strcmp(t.lx,"-"))
	{
		t = GetNextToken();
	}
	else if (!strcmp(t.lx,"~"))
	{
		t = GetNextToken();
	}
	
	pi = operand();
	t = pi.tk;
		
	pi.tk = t;
	return pi;
}
ParserInfo operand()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;

	t = PeekNextToken();
	if (!strcmp(t.lx,"true"))
	{
		t = GetNextToken();
	}
	else if (!strcmp(t.lx,"false"))
	{
		t = GetNextToken();
	}
	else if (!strcmp(t.lx,"null"))
	{
		t = GetNextToken();
	}
	else if (!strcmp(t.lx,"this"))
	{
		t = GetNextToken();
	}
	else if (t.tp == STRING)
	{
		t = GetNextToken();
	}
	else if (t.tp == INT)
	{
		t = GetNextToken();
	}
	else if (!strcmp(t.lx,"("))
	{
		t = GetNextToken();
		pi = expression();
		t = pi.tk;
		if (pi.er != none) return pi;
			
		t = PeekNextToken();
		if (!strcmp(t.lx,")"))
		{
			t = GetNextToken();
		}
		else
		{
			if (pi.er == none)
			{
			pi.er = closeParenExpected;
			pi.tk = t;
			return pi;
			}
		}
	}
	else if (t.tp == ID)
	{
		t = GetNextToken();
		t = PeekNextToken();
		if (!strcmp(t.lx,"."))
		{
			t = GetNextToken(); //consume the "."

			t = PeekNextToken(); //consume the next token and assign it to t
			if (t.tp == ID)
			{
				t = GetNextToken();
			}
			else
			{
				if (pi.er == none)
				{
					pi.er = idExpected;
					pi.tk = t;
					return pi;
				}
			}
			t = PeekNextToken();
		}
		if (!strcmp(t.lx,"["))
		{
			t = GetNextToken(); //consume "["
			pi = expression();
			t = pi.tk;
			if (pi.er != none) return pi;

			t = PeekNextToken();
			if (!strcmp(t.lx,"]"))
			{
				t = GetNextToken();
			}
			else
			{
				if (pi.er == none)
				{
					pi.er = closeBracketExpected;
					pi.tk = t;
					return pi;
				}
			}
			
		}
		else if (!strcmp(t.lx,"("))
		{
			t = GetNextToken(); // consume "("
			
			pi = expressionList();
			t = pi.tk;
			if (pi.er != none) return pi;

			t = PeekNextToken();
			if (!strcmp(t.lx, ")"))
			{
				t = GetNextToken();
			}
			else
			{
				if (pi.er == none)
				{
					pi.er = closeParenExpected;
					pi.tk = t;
					return pi;
				}
			}
		}

	}
	else
	{
        if(pi.er == none)
		{
            pi.er = syntaxError;
            pi.tk = t;
            return pi;
        }
    }
	
	pi.tk = t;
	return pi;
}
#ifndef TEST_PARSER
int main ()
{   
    InitParser("List.jack");
    Parse();
    printf("%s\n", class_name);
    printf("Static: %i\n", table.staticCount);
    printf("Field: %i\n", table.fieldCount);
    printf("Kind: %s\n", table.total_count.kind);
    printf("Type: %s\n", table.total_count.type);
    printf("Name: %s\n", table.total_count.name);
    printf("Index: %i\n", table.total_count.index);
    return 1;
}
#endif


