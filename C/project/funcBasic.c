#include "definitions.h"

node * input(char *filename){ /* writing code line text into the nodes */
    node * head;
    node * node_ptr;
    int node_num;
    FILE * fptr;
    
    head = NULL;
    node_ptr = NULL;
    node_num = 1; /* setting code line number */
    MALLOC(node, node_ptr, sizeof(node)) /* allocate memory for node_ptr (type=node, size=node size) */
    head = node_ptr; /* head = node_ptr */
    if (!(fptr = fopen(filename,"r"))){
        printf("Error opening file\n");
        return NULL;
    }
    while (!feof(fptr)) { /* creating node list for assembly code lines */
        fscanf(fptr, "%[^\n]%*c", node_ptr->txt);
        node_ptr->node_num = node_num++; /* numbering */
        node_ptr->next = NULL;
        node_ptr->bin = NULL;
        if(!(feof(fptr))) {
            MALLOC(node, node_ptr->next, sizeof(node))
        }
        node_ptr=node_ptr->next;
    }

    fclose(fptr);
    return head;
}

void print_node(node * node_head){ /* printing the node list */
    node * node_ptr;
    node_ptr = node_head;

    while (node_ptr){
        printf("[%d] %s\n",node_ptr->node_num, node_ptr->txt);
        fflush(stdout);
        node_ptr = node_ptr->next;
    }
}

void print_symbol(symbol *symbol_head){ /* printing the node list */
    symbol * symbol_ptr;
    symbol_ptr = symbol_head;
    puts("Symbol list:");
    while(symbol_ptr){
        printf("%d | %s | %d\n",symbol_ptr->value,symbol_ptr->name,symbol_ptr->type);
        fflush(stdout);
        symbol_ptr = symbol_ptr->next;
    }
}

char *getStringTillChar(char * string , char c){
    char *string_copy, ch;
    int i=0;
    MALLOC(char, string_copy, strlen(string)*sizeof(char))

    ch = *string;
    while(ch != c && ch != '\0'){
        *(string_copy + i) = ch;
        i++;
        ch = *(string + i);
    }
    string_copy[i] = '\0';
    return string_copy;
}

void delBlanks(char **str){
    char *ptr;
    ptr = *str;
    /* starts with blanks */
    while(**str == ' ') (*str)++;

    /* ends with blanks */
    ptr = *str;
    while(*ptr != ' ' && *ptr != '\0') ptr++;
    *ptr = '\0';

}

int isBlank(char c){
    if(c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\b') return 1;
    return 0;
}
