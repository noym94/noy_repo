#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


/* MACROS */
#define EXIT1(term,msg) if(term){ puts(msg); exit(1);}
#define MALLOC(type,ptr,size) ptr = (type *)malloc(size); EXIT1(!ptr,"memory allocation error!")
#define REALLOC(type,ptr,size) ptr = (type *)realloc(ptr,size); EXIT1(!ptr,"memory allocation error!")
#define START_IC 100
typedef struct int21{
    int val:21;
}int21;

typedef struct node { /* structure for assembly line. */
    char txt[80]; /* code line text */
    int *bin; /* binary words */
    int IC; /* first decimal address (instructions counter) */
    int DC; /* count decimal address (data counter) */
    int node_num; /* the number of the code line */
    struct node * next; /* the next code line */
} node;

typedef struct symbol {
    char * name;
    int value;
    enum {_macro,_cmd,_datstr} type;
    struct symbol * next;
} symbol;


/* basic function declarations*/
void print_node(node *);
void print_symbol(symbol *);
node * input(char *);
char * getStringTillChar (char *, char);
void delBlanks(char **);
int isBlank(char);


/* syntax functions declarations*/
int nonZeroIndex(node *, int);
int argCheck(node *, char[]);
int zeroArgCmd(node *, int);
int oneArgCmd(node *, int);
int twoArgCmd(node *, int);
int labelCheck(node *, int);
int isDot(node *, int);
int syntaxValidator(node * );

/* first round declarations */
symbol *decimalMap(node *);
int addSymbol (char *,int, int, symbol **, node *);
int isSymbolExist (char *, symbol *);
int addMacroSymbol(node *, symbol **);
int getStringDC (char *);
int getDataDC (char *);
int argDC(char *);
int twoArgDC(char *);
int getCmdDC (char *);
int addLabelSymbol (node *, symbol **);
void setNodeAddress(node *, int);
int getSymbolValue(symbol *, char *);

/* second round declarations */
void bitMap(node *, symbol *);
void DataString(int *, char *,symbol *);
void bitCMD(int *, char *, symbol *,node *);
void binARGS(int *,int *, char *, int *, symbol *, node *);
int typeArg(char *, int, symbol *);
int registerArg(char *);
int opcode(char *);
void printBin(node *);
int getDataRows(node *);
int getBinaryRows(node *);
void baseHexaFile(node *, symbol *, char *);
void createExternFile(node *,  char *);
void createEntryFile(node *, symbol *, char *);
char binToHex(int *num);
