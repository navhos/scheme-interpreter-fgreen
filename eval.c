#include <stdlib.h>     
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parser2.h"
#include "eval.h"

Cons curr;
char token[20];

Cons eval(Cons list) {	
	if(list->symbol && strcmp(list->symbol, "(")) {
		return NULL;
	}
}