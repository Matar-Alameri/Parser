#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"


// you can declare prototypes of parser functions below
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
	t = GetNextToken();
	if (IsClassVar(t.lx))
	{
		;
		
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
	t = GetNextToken();
	if (Istype(t))
	{
		;
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
	t = GetNextToken();
	if (t.tp == ID)
	{
		;
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
		GetNextToken(); //consume ","
		t = GetNextToken();
		if (t.tp == ID)
		{
			;
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
	t = GetNextToken();
	if (!strcmp(t.lx, ";"))
	{
		;
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
	t = GetNextToken();
	if (IsSubroutine(t.lx))
	{
		;
		
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
	t = GetNextToken();
	if (Istype(t))
	{
		;
	}
	else if (!strcmp(t.lx,"void"))
	{
		;
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
	t = GetNextToken();
	if (t.tp == ID)
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"("))
	{
		;
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
	if (Istype(t))
	{
		pi = paramList();
		t = pi.tk;
	}
	t = GetNextToken();
	if (!strcmp(t.lx,")"))
	{
		;
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
	t = GetNextToken();
	if (Istype(t))
	{
		;
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
	t = GetNextToken();
	if (t.tp == ID)
	{
		;
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
		GetNextToken();
		t = GetNextToken();
		if (Istype(t))
		{
			;
		}
		else if (t.tp == ID)
		{
			;
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
		t = GetNextToken();
		if (t.tp == ID)
		{
			;
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
		t = PeekNextToken();
	}
	t = GetNextToken();
	if (!strcmp(t.lx,"}"))
	{
		;
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
	t = PeekNextToken();
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
	pi.tk = t;
	return pi;
}

ParserInfo varDeclarStatement()
{
	ParserInfo pi;
    pi.er = none;
    pi.tk = t;
	t = GetNextToken();
	if (!strcmp(t.lx,"var"))
	{
		;
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
	t = GetNextToken();
	if (Istype(t))
	{
		;
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
	t = GetNextToken();
	if (t.tp == ID)
	{
		;
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
		GetNextToken();
		t = GetNextToken();
		if (t.tp == ID)
		{
			;
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
	t = GetNextToken();
	if (!strcmp(t.lx,";"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"let"))
	{
		;
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
	t = GetNextToken();
	if (t.tp == ID)
	{
		;
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
		GetNextToken();
		t = PeekNextToken();
		if (IsFactor(t))
		{	
			pi = expression();
			if (pi.er != none) return pi;
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
	
		t = GetNextToken();
		if (!strcmp(t.lx,"]"))
		{
			;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"="))
	{
		;
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
	t = PeekNextToken();
	if (IsFactor(t))
		{	
			pi = expression();
			if (pi.er != none) return pi;
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
	t = GetNextToken();
	if (!strcmp(t.lx,";"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"if"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"("))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,")"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"{"))
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
	while (IsStatement(t.lx))
	{
		pi = statement();
		t = pi.tk;
		t = PeekNextToken();
	}
	t = GetNextToken();
	if (!strcmp(t.lx,"}"))
	{
		;
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
		GetNextToken();
		t = GetNextToken();
		if (!strcmp(t.lx,"{"))
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
		while (IsStatement(t.lx))
		{
			pi = statement();
			t = pi.tk;
			t = PeekNextToken();
		}
		t = GetNextToken();
		if (!strcmp(t.lx,"}"))
		{
			;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"while"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"("))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,")"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"{"))
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
	while (IsStatement(t.lx))
	{
		pi = statement();
		t = pi.tk;
		t = PeekNextToken();
	}
	t = GetNextToken();
	if (!strcmp(t.lx,"}"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"do"))
	{
		;
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
	
	t = GetNextToken();
	if (!strcmp(t.lx,";"))
	{
		;
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
	t = GetNextToken();
	if (t.tp == ID)
	{
		;
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
		GetNextToken();
		t = GetNextToken();
		if (t.tp == ID)
		{
			;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"("))
	{
		;
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
		pi = expressionList();
		t = pi.tk;
		
	}
	t = GetNextToken();
	if (!strcmp(t.lx,")"))
	{
		;
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
			GetNextToken();
			pi = expression();
			if (pi.er != none) return pi;
			t = pi.tk;
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
	t = GetNextToken();
	if (!strcmp(t.lx,"return"))
	{
		;
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
	t = GetNextToken();
	if (!strcmp(t.lx,";"))
	{
		;
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
	t = PeekNextToken();
	if (IsFactor(t))
	{
		pi = relationalExpression();
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
	while (!strcmp(t.lx,"&") || !strcmp(t.lx,"|"))
	{
		GetNextToken();
		t = PeekNextToken();
		if (IsFactor(t))
		{
			pi = relationalExpression();
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
	}
	pi.tk = t;
	return pi;
}
ParserInfo relationalExpression()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (IsFactor(t))
	{
		pi = ArthimeticExpression();
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
	while (!strcmp(t.lx,"=" ) || !strcmp(t.lx,">" ) || !strcmp(t.lx,"<" ))
	{
		GetNextToken();
		t = PeekNextToken();
		if (IsFactor(t))
		{
			pi = ArthimeticExpression();
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
	}
	pi.tk = t;
	return pi;
}
ParserInfo ArthimeticExpression()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (IsFactor(t))
	{
		pi = term();
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
	while (!strcmp(t.lx,"+" ) || !strcmp(t.lx,"-" ))
	{
		GetNextToken();
		t = PeekNextToken();
		if (IsFactor(t))
		{
			pi = term();
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
	}
	pi.tk = t;
	return pi;
}
ParserInfo term()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	t = PeekNextToken();
	if (IsFactor(t))
	{
		pi = factor();
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
	while (!strcmp(t.lx,"*" ) || !strcmp(t.lx,"/" ))
	{
		GetNextToken();
		t = PeekNextToken();
		if (IsFactor(t))
		{
			pi = factor();
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
		GetNextToken();
	}
	else if (!strcmp(t.lx,"~"))
	{
		GetNextToken();
	}
	t = PeekNextToken();
	if (IsOperand(t))
	{
		pi = operand();
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
	pi.tk = t;
	return pi;
}
ParserInfo operand()
{
	ParserInfo pi;
	pi.er = none;
	pi.tk = t;
	printf("you started operand\n");
	printf("Token details: type=%d, lexeme=%s, errorCode=%d, line=%d, file=%s\n",
		pi.tk.tp, pi.tk.lx, pi.tk.ec, pi.tk.ln, pi.tk.fl); 
	t = GetNextToken();
	if (!strcmp(t.lx,"true"))
	{
		;
	}
	else if (!strcmp(t.lx,"false"))
	{
		;
	}
	else if (!strcmp(t.lx,"null"))
	{
		;
	}
	else if (!strcmp(t.lx,"this"))
	{
		;
	}
	else if (t.tp == STRING)
	{
		;
	}
	else if (t.tp == INT)
	{
		;
	}
	else if (!strcmp(t.lx,"("))
	{
		t = PeekNextToken();
		if (IsFactor(t))
		{
			pi = expression();
			t = pi.tk;
			if (pi.er != none) return pi;
			
		}
		t = GetNextToken();
		if (!strcmp(t.lx,")"))
		{
			;
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
		t = PeekNextToken();
		if (!strcmp(t.lx,"."))
		{
			GetNextToken(); //consume the "."

			t = GetNextToken(); //consume the next token and assign it to t
			if (t.tp == ID)
			{
				;
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
		}
		if (!strcmp(t.lx,"["))
		{
			GetNextToken(); //consume "["
			t = PeekNextToken();
			if (IsFactor(t))
			{
				pi = expression();
				t = pi.tk;
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
			t = GetNextToken();
			if (!strcmp(t.lx,"]"))
			{
				;
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
			GetNextToken(); // consume "("
			t = PeekNextToken();
			if (IsFactor(t))
			{
				pi = expressionList();
				t = pi.tk;
				if (pi.er != none) return pi;

			}
			t = GetNextToken();
			if (!strcmp(t.lx, ")"))
			{
				;
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
	
	pi.tk = t;
	printf("ended operand\n");
	printf("Token details: type=%d, lexeme=%s, errorCode=%d, line=%d, file=%s\n",
		pi.tk.tp, pi.tk.lx, pi.tk.ec, pi.tk.ln, pi.tk.fl); 
	return pi;
}


