#include "definitions.h"

int global_IC = 100; /* current decimal line address */

symbol *decimalMap(node * node_head){
    node * node_ptr;
    symbol **symbol_head;
    char *start;
    node_ptr = node_head;

    MALLOC(symbol *, symbol_head, sizeof(symbol *))
    (*symbol_head) = NULL;

    while(node_ptr){
        start = getStringTillChar(node_ptr->txt, ' ');
        delBlanks(&start);
        if (strcmp(start, ".define") == 0) /* if command starts with dot */
            addMacroSymbol(node_ptr,symbol_head);
        else if(isalpha(node_ptr->txt[0])) /* if command starts with label */
            addLabelSymbol(node_ptr,symbol_head);
        else if  (node_ptr->txt[0] == ' ')
            setNodeAddress(node_ptr,0);

        node_ptr=node_ptr->next; /* including comments */
    }
    return *symbol_head;
}

int isSymbolExist(char *symbol_name, symbol *symbol_head){
    symbol *symbol_ptr;
    symbol_ptr = symbol_head;

    while(symbol_ptr)
    {
        if (strcmp(symbol_name ,symbol_ptr->name) == 0)
            return 1;
        symbol_ptr=symbol_ptr->next;
    }

    return 0;
}

int addSymbol(char *name, int decimal, int typ, symbol **symbol_head, node * node_ptr) {
    symbol * symbol_ptr; /* copying symbol */
    symbol_ptr = *symbol_head;

    if(isSymbolExist(name,symbol_ptr)) { /* check if existed */
        printf("line [%d]: symbol %s already exists\n", node_ptr->node_num, name);
        return 0;
    }
    if(!symbol_ptr) { /* in case head is empty */
        MALLOC(symbol, symbol_ptr, sizeof(symbol))
        symbol_ptr->value = decimal;
        symbol_ptr->name = name;
        symbol_ptr->type = typ;
        symbol_ptr->next = NULL;
        *symbol_head = symbol_ptr;
    }
    else { /* any other case */
        while (symbol_ptr->next != NULL) symbol_ptr = symbol_ptr->next; /*reaching null next */
        MALLOC(symbol, symbol_ptr->next, sizeof(symbol))
        symbol_ptr->next->value = decimal;
        symbol_ptr->next->name = name;
        symbol_ptr->next->type = typ;
        symbol_ptr->next->next = NULL;
    }
    return 1;
}

void setNodeAddress(node *node_ptr,int index) {
    char *args, *dot;

    args = getStringTillChar(node_ptr->txt + index,'\0');
    while(args[0] == ' ') args += 1; /* avoiding blanks */
    node_ptr->DC = 1;
    if(args[0] == '.') { /* dot cases */
        dot = getStringTillChar(args,' ');
        if(strcmp(dot,".data") == 0) node_ptr->DC = getDataDC(args);
        else if(strcmp(dot,".string") == 0) node_ptr->DC = getStringDC(args);
        else if(strcmp(dot,".entry") == 0) return;
        else if(strcmp(dot,".extern") == 0) return;
    }
    else node_ptr->DC += getCmdDC(args); /* cmd cases */
    node_ptr->IC = global_IC;
    if(node_ptr->DC) global_IC += node_ptr->DC;
}

int addLabelSymbol(node *node_ptr, symbol **symbol_head){
    int typ;
    char *label, *cmd_name;

    label = getStringTillChar(node_ptr->txt,':');
    cmd_name = node_ptr->txt + strlen(label) + 1;
    while(*cmd_name == ' ') cmd_name++;
    cmd_name = getStringTillChar(cmd_name, ' ');
    if((strcmp(cmd_name,".data") == 0) || (strcmp(cmd_name,".string") == 0))
        typ = _datstr;
    else typ = _cmd;
    setNodeAddress(node_ptr,strlen(label)+2);
    return addSymbol(label,node_ptr->IC,typ,symbol_head,node_ptr); /* adding symbol */

}
/* the only legal dot prefix is macro - assuming syntax check is already done */
int addMacroSymbol(node * node_ptr, symbol **symbol_head) { /* add macros (.define) to symbol table + value */
    char *macro_name, *val;
    int macro_val;

    macro_name = getStringTillChar(node_ptr->txt + 8, ' '); /* getting macro var name */
    val = node_ptr->txt + (8 + strlen(macro_name));
    while (val[0] == ' ' || val[0] == '=') val++; /* syntax was checked at syntax phase */
    macro_val = atoi(val);
    return addSymbol(macro_name,macro_val,_macro,symbol_head,node_ptr);
}

int getDataDC (char *args){
    char * data_args, *c;
    int DC = 1;
    data_args = getStringTillChar(args+5,'\0'); /* get arguments only */
    while(data_args[0] == ' ') data_args++;
    c = data_args;
    while(*c != '\0'){ /* counting */
        if(*c == ',') DC++;
        c++;
    }
    return DC;
}

int getStringDC(char * args){
    char * string_value;
    while (args[0] == ' ') args++; /* ignoring blanks */
    while (args[0] != '\"') args++; /* ignoring first " */
    string_value = getStringTillChar(args+1, '\"'); /* getting the string beginning without the " */
    return strlen(string_value)+1; /* getting the string character length, includes '\0' */
}

int getCmdDC (char * args){
    char * cmd_name, * rest;
    cmd_name = getStringTillChar(args,' ');
    rest = (getStringTillChar(args + strlen(cmd_name),'\0'));

    while(rest[0] == ' ') rest += 1;
    /* commands with two arguments */
    if ((strcmp(cmd_name, "mov") == 0) ||
        (strcmp(cmd_name, "cmp") == 0) ||
        (strcmp(cmd_name, "add") == 0) ||
        (strcmp(cmd_name, "sub") == 0) ||
        (strcmp(cmd_name, "lea") == 0)) return twoArgDC(rest);
    /* commands with one argument */
    else if ((strcmp(cmd_name, "not") == 0) ||
        (strcmp(cmd_name, "clr") == 0) ||
        (strcmp(cmd_name, "inc") == 0) ||
        (strcmp(cmd_name, "dec") == 0) ||
        (strcmp(cmd_name, "jmp") == 0) ||
        (strcmp(cmd_name, "bne") == 0) ||
        (strcmp(cmd_name, "red") == 0) ||
        (strcmp(cmd_name, "prn") == 0) ||
        (strcmp(cmd_name, "jsr") == 0)) return argDC(rest);
    /* commands with no argument */
    else return 0;
}

int argDC (char * c){
    if(*c == 'r') return 0;
    else if (isalpha(*c)) return 1;
    else if (strrchr(c, '#'))
        return 1;
    else if (strrchr(c, '&'))
        return 1;
    return 0;
}

int twoArgDC(char * args){
    char * arg1, * arg2;
    arg1 = getStringTillChar(args,',');
    arg2 = getStringTillChar(args + strlen(arg1) + 1,'\0');
    while(arg2[0] == ' ') arg2 += 1;

    if(*arg1 == 'r' && *arg2 == 'r') return 0;
    else return (argDC(arg1) + argDC(arg2));
}

int getSymbolValue(symbol *symbol_head, char *name){
    symbol * symbol_ptr;
    symbol_ptr = symbol_head;

    while(symbol_ptr){
        if(strcmp(symbol_ptr->name,name) == 0)
            return symbol_ptr->value;
        symbol_ptr = symbol_ptr->next;
    }
    return 0;
}
