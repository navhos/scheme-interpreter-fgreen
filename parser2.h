#ifndef PARSER2
#define PARSER2
#include <stdlib.h>

//defines pointer to struct cons as Cons
 struct con;
 typedef struct con *Cons;

//has a local variable which holds the initial token read in the current recursion level
//has a temp variable which recurses through the current list by progressing int the order:
//list -> list.rest/list->rest
//Algorithm:
//1.make cons cell with ( as the symbol | Initialization step
//2.make temp=temp.rest | make (new)temp->first point to another call to S_expr() | Progressing step
//3. Three possibilities here: Recursive step
//if ( is the next token, another recursion loop of S_expr begins at the end of which this list = temp.first
//else if this is a symbol the cons cell with this symbol=temp.first
//else (')') the current recursion ends | look into body of code for more notes
Cons S_expr();

static Cons globalEnv;

/*Takes a cons pointer as an argument
Implementation:
return if list==NULL
print the symbol of the list
recurse on the list->first cons pointer first
then on the list->rest pointer*/
void printList(Cons list);

int main();

/*Cons symbol(Cons list);

Cons car(Cons list);

Cons cdr(Cons list);

Cons cons(Cons l1, Cons l2);

Cons quote(Cons list);

Cons t();*/

Cons f();

/*Cons createCell(char *token);

Cons createemptyCell();*/

Cons eval(Cons list);

#endif