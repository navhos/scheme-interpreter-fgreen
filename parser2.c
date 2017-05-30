#include <stdlib.h>     
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "parser2.h"

char token[20];
static Cons globalEnv;

struct con {
    char *symbol;    //symbol is the string within each node
    struct con *first;     //car
    struct con *rest;      //cad
};

//this creates a cons cell where con->symbol=token
Cons createCell(char *token) {
    Cons con = malloc(sizeof(Cons));
    con->symbol = malloc(20);
    strcpy(con->symbol,token);
    return con;
}

//createCell which creates an empty cons cell
Cons createemptyCell() {
    Cons con = malloc(sizeof(Cons));
    con->symbol = NULL;
    con->first = NULL;
    con->rest = NULL;

    return con;
}

//the true cons cell
Cons t() {
    Cons tr = createCell("#t");
    return tr;
}
int checkt(Cons list) {
    return (list->symbol && strcmp(list->symbol, "#t") == 0);
}

//false/empty/() cons cell
Cons f() {
    Cons f = createCell("(");
    f->rest = createemptyCell();
    return f;
}

//check if a list is f()
//Unique property of f():
//f->symbol=="(" && f->first==null
int checkf(Cons list) {
    return(list->symbol && strcmp(list->symbol, "(") == 0 && (!(list->first)));
}

//if list is a terminating cell || ")"
//a terminating cons cell is such that:
//list->symbol == list->first->symbol == null
int checkTerminate(Cons list) {
     return ((!(list->symbol)) && (list->first) && (!(list->first->symbol)));
}

Cons S_expr()
{
    Cons local, temp;

    strcpy(token, getToken());

    if (strcmp(token, "(") == 0)
    {
        local = createCell("(");
        local->first = S_expr();
        temp = local;
        while (strcmp(token,")") != 0)
        {
            temp->rest = createemptyCell();
            temp = temp->rest;
            temp->first = S_expr();
        }
        temp -> rest = NULL;
        strcpy(token, "placeholder");
        return local;
    }
    else if(strcmp(token, ")") == 0)
    {
        return createemptyCell();
    }
    else if(strcmp(token, "#f") == 0 || strcmp(token, "()") == 0) {
        return f();
    }
    else
    {
        return createCell(token);
    }
    
}

void printList(Cons list) {
    if(list == 0) return;

    if(list->symbol)printf("%s ", (list->symbol));
    //if cons cell does not have a rest,first or symbol, it is a ")"
    else if((!(list->first))&&(!(list->rest))&&(!(list->symbol))) printf("%s ", ")");
    
    printList(list->first);
    printList(list->rest); 
}

Cons symbol(Cons list) {
    //if it is the empty token return false/()
    if(strcmp("()", list->symbol) == 0) return f();
    //if it does not start with a ( return t
    else if(list->symbol && (strcmp("(", list->symbol) != 0)) return t();
    //else return t
    else return f();
}

Cons car(Cons list) {
    //if the list is a cons cell with a symbol != '(', it must be just a symbol or,
    //symbol(list) == #t => so just return this cons cell with the symbol
    if(list->symbol && strcmp(list->symbol, "(") != 0) return list;
    //or else just return list->first
    else return list->first;
}

Cons cdr(Cons list) {
    //These are all corner-case | More efficient implementation of S_expr() may have helped avoid these if statements

    //if cdr=null:
    if(list->symbol && strcmp(list->symbol, "(") != 0) return f();
    if(list->rest && checkTerminate(list->rest)) return f();
    
    //if there is only one cons cell(symbol) in cdr we do not want to embed that cons cell in an additional list
    if(list->rest && list->rest->rest && checkTerminate(list->rest->rest)) return list->rest->first;

    //if sizeof(cdr)>1 embed list->rest in a list
    Cons start = createCell("(");
    start->first = list->rest->first;
    start->rest = list->rest->rest;
    return start;

}

Cons quote(Cons list) {
    //just return the list itself
    //Functionality:Stops recursion in eval()
    return list;
}

Cons cons(Cons l1, Cons l2) {
    //if l2 is another list the first element would be "("
    //it must be made to immitate the S_expr() lists
    //so a head is made to replace "(" and the original "head" is set to NULL
    //typical cons operations take place after: new head = "(" | head->first=l1 | head->rest=l2
    if(l2 && l2->symbol && strcmp(l2->symbol, "(") == 0) {
        l2->symbol = NULL;
        Cons head = createCell("(");
        head->rest = l2;
        head->first = l1;
        return head;
    }

    //if l2 is a symbol we will need create a NULL cell since there is no head(with "(") to replace
    else {
        Cons head = createCell("(");
        Cons tail = createemptyCell();
        head->first = l1;
        head->rest = tail;
        tail->first = l2;
    }
}

Cons append(Cons l1, Cons l2) {
    //if l1 is the empty list return l2
    if((l1->symbol && strcmp(l1->symbol, "(") == 0 && !(l1->first))) {
        return l2;
    } 
    //else do the following recursion:identical to the append algorithm as implemented in scheme
    else {
        return cons(car(l1),append(cdr(l1), l2));
    }
}

Cons null(Cons list) {
    //according to this interpreter an empty list has the structure:
    //null->symbol = "(" && null->first=NULL && null->rest=emptycell
    //However, no other cons cell has the following property:
    //null->symbol = "(" && null->first=NULL
    //so this is enough to check for null cons cell
    if(list->symbol && strcmp(list->symbol, "(") == 0 && !(list->first)) return t();
    else return f();
}

Cons equal (Cons l1, Cons l2) {
    Cons localf, localr;

    //if l1 and l2 are both symbols, check if they have the same symbols
    if(l1->symbol && l2-> symbol && strcmp((symbol(l1))->symbol, "#t") == 0 && strcmp((symbol(l2))->symbol, "#t") == 0) {
        if(strcmp(l1->symbol, l2->symbol) == 0) return t();
        else return f();
    }
    //if l1 and l2 are both lists check that eq(car(l1), car(l2)) && eq(cdr(l1), cdr(l2)) = true
    //According to how f() works: if (->null for both the cons cell is a #f cons cell 
    else if(strcmp((symbol(l1))->symbol, "(") == 0 && strcmp((symbol(l2))->symbol, "(") == 0) {
        localf = equal(car(l1), car(l2));
        localr = equal(cdr(l1), cdr(l2));
        if(localf->symbol && localr->symbol && strcmp(localf->symbol, localr->symbol) == 0) return t();
        else return f();
    }
    else return f();
}

//takes a symbol and a list of pairs(lists of 2)
Cons assoc(char * symb, Cons list) {
    //check if list is a symbol or #f
    //return false if so | this means recursion should terminate
    if(checkf(list)) return f();
    Cons local;
    //this conditional statement is for the last element of the list of pairs
    //because of how cdr works this will not be in an embedded list
    if(list-> first && checkt(symbol(list->first))) {
        if((list->first && strcmp(list->first->symbol, symb) == 0) || (list->rest->first && strcmp(list->rest->first->symbol, symb) == 0)) return list;
        else return f();
    }
    //pair = car(list)
    //check if any of the two elements of the pair: car(pair) && cdr(pair) has symb
    //if not check in the rest of the list = cdr(local)
    else {
        local = car(list);
        if(local && car(local) && strcmp((car(local))->symbol, symb) == 0) return local;
        else return assoc(symb, cdr(list));
    }
}

Cons cond(Cons list) {
    //evaluates the condition(car(car(list)))
    //if this evaluates as not false, returns the second element or cdr of the pair | cdr(car(list))
    //if it evaluates as false, recurses by calling cond on the cdr of the list
    //recursion stops when the empty list is called or, car(car(list)) does not exist.
    if(car(list) && car(car(list))) {
        Cons conditional = car(car(list));
        Cons evaluated = eval(conditional);
        if(!(checkf(evaluated))) {
            if(evaluated->rest && !checkf(evaluated->rest->rest)) return cdr(car(list));
            else return cdr(list);
            
        }
        else return cond(cdr(list));
        
    } 
    else return f();
}

Cons define(Cons list) {

    Cons symb = car(list);
    //Cons temp = f();
    Cons temp;
    temp = cons(list, globalEnv);
    globalEnv = temp;
    return car(symb);
   
}

Cons eval(Cons list) {
    if(!globalEnv) globalEnv = f();
    if(list->symbol && strcmp(list->symbol, "(") == 0) {
        if(!(list->first)) return list;
        else if(strcmp(list->first->symbol, "car") == 0) return car(eval(list->rest));
        else if(strcmp(list->first->symbol, "cdr") == 0) return cdr(eval(list->rest));
        else if(strcmp(list->first->symbol, "quote") == 0) return quote(cdr(list));
        else if(strcmp(list->first->symbol, "symbol") == 0) return symbol(eval(list->rest));
        else if(strcmp(list->first->symbol, "cons") == 0) return cons(eval(list->rest), eval(list->rest->rest));
        else if(strcmp(list->first->symbol, "append") == 0) return append(eval(list->rest), eval(list->rest->rest));
        else if(strcmp(list->first->symbol, "null") == 0) return null(eval(list->rest));
        else if(strcmp(list->first->symbol, "equal") == 0) return equal(eval(list->rest), eval(list->rest->rest));
        else if(strcmp(list->first->symbol, "assoc") == 0) return assoc((eval(list->rest))->symbol, eval(list->rest->rest));
        else if(strcmp(list->first->symbol, "cond") == 0) return cond(eval(list->rest));
        else if(strcmp(list->first->symbol, "define") == 0) return define(eval(list->rest)); //define(pair)
        else if(strcmp(list->first->symbol, "exit") == 0) exit(0);
        else if(!(checkf(assoc(list->first->symbol, globalEnv)))) {
            return eval(cdr(assoc(list->first->symbol, globalEnv)));
        }
        //else if(list->first->symbol && strcmp(list->first->symbol, "(")==0) eval(list->first);
        else return list;
    }
    else if(!(list->symbol)) return eval(list->first);
    else return list;
}



