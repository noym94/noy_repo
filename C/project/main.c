#include "definitions.h"



int main(int argc, char *argv[]) { 

    char *filename; /* initialize a pointer named filename */
    char *file2execute; /* initialize a pointer for actual filename */
    node * node_head, *ntmp; /* initialize a node on a pointer named node_head */
    symbol * symbol_head, *stmp; /* initialize a symbol named symbol_head */
    int i;

    for (i=1; i<argc; i++){
        file2execute = strcat(argv[i], ".as");
        filename = getStringTillChar(file2execute, '.');
        node_head = input(argv[i]); /* insert to node_head pointer the value of the start of the assembly file in memory and create a linked list of nodes with assembly lines */
        symbol_head = NULL;  /* initialize symbol_head with null */

        if(syntaxValidator(node_head) == 0) { /* run syntax validator on all file */
            symbol_head = decimalMap(node_head); /* if syntax validator is OK, put inside symbol_head the value of the first symbol in the symbol table*/
        }
        bitMap(node_head,symbol_head);
        baseHexaFile(node_head, symbol_head, filename);
        createExternFile(node_head, filename);
        createEntryFile(node_head, symbol_head, filename);
        
        ntmp = node_head;
        while(node_head){
            free(ntmp);
            node_head = node_head->next;
            ntmp = node_head;
        }
        stmp = symbol_head;
        while(symbol_head){
            free(stmp);
            symbol_head = symbol_head->next;
            stmp = symbol_head;
        }
    }
    return 0;
}
