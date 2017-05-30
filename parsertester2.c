#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parser2.h"

int main(){
	char token[20];
	startTokens(20);
	Cons list, list2;
	while(1) 
	{
		printf("scheme> ");
		list = S_expr();
		list2 = eval(list);
		printList(list2);
		printf("\n\n");
	}
	return 0;
}