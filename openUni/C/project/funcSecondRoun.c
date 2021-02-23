#include "definitions.h"
int global_IC2=100;

int opcode(char *cmd_name){
    if(strcmp(cmd_name, "mov")  == 0) return 0;
    if(strcmp(cmd_name, "cmp")  == 0) return 1;
    if(strcmp(cmd_name, "add")  == 0) return 2;
    if(strcmp(cmd_name, "sub")  == 0) return 2;
    if(strcmp(cmd_name, "lea")  == 0) return 4;
    if(strcmp(cmd_name, "not")  == 0) return 5;
    if(strcmp(cmd_name, "clr")  == 0) return 5;
    if(strcmp(cmd_name, "inc")  == 0) return 5;
    if(strcmp(cmd_name, "dec")  == 0) return 5;
    if(strcmp(cmd_name, "jmp")  == 0) return 9;
    if(strcmp(cmd_name, "bne")  == 0) return 9;
    if(strcmp(cmd_name, "red")  == 0) return 12;
    if(strcmp(cmd_name, "prn")  == 0) return 13;
    if(strcmp(cmd_name, "jsr")  == 0) return 9;
    if(strcmp(cmd_name, "rts")  == 0) return 14;
    if(strcmp(cmd_name, "stop") == 0) return 15;
    return -1;
}

int function(char *cmd_name){
    if(strcmp(cmd_name, "mov")  == 0) return 0;
    if(strcmp(cmd_name, "cmp")  == 0) return 0;
    if(strcmp(cmd_name, "add")  == 0) return 1;
    if(strcmp(cmd_name, "sub")  == 0) return 2;
    if(strcmp(cmd_name, "lea")  == 0) return 0;
    if(strcmp(cmd_name, "not")  == 0) return 2;
    if(strcmp(cmd_name, "clr")  == 0) return 1;
    if(strcmp(cmd_name, "inc")  == 0) return 3;
    if(strcmp(cmd_name, "dec")  == 0) return 4;
    if(strcmp(cmd_name, "jmp")  == 0) return 1;
    if(strcmp(cmd_name, "bne")  == 0) return 2;
    if(strcmp(cmd_name, "red")  == 0) return 0;
    if(strcmp(cmd_name, "prn")  == 0) return 0;
    if(strcmp(cmd_name, "jsr")  == 0) return 3;
    if(strcmp(cmd_name, "rts")  == 0) return 0;
    if(strcmp(cmd_name, "stop") == 0) return 0;
    return -1;
}

int registerArg(char *arg){ /* validate the registers names are correct */
    if((strcmp(arg, "r0") == 0)||
       (strcmp(arg, "r1") == 0)||
       (strcmp(arg, "r2") == 0)||
       (strcmp(arg, "r3") == 0)||
       (strcmp(arg, "r4") == 0)||
       (strcmp(arg, "r5") == 0)||
       (strcmp(arg, "r6") == 0)||
       (strcmp(arg, "r7") == 0)) return 1;
    return 0;
}

int typeArg(char *arg, int t, symbol *symbol_head){ /* check if *arg is in symbol table from type t (macro, cmd, etc) */
    symbol *symbol_ptr;
    symbol_ptr = symbol_head;
    while(symbol_ptr){
        if(strcmp(arg,symbol_ptr->name) == 0){
            if(symbol_ptr->type == t) return 1;
            else return 0;
        }
        symbol_ptr = symbol_ptr->next;
    }
    return 0;
}

void binARGS(int *bits,int *ix, char *arg, int *trg, symbol *symbol_head, node *node_ptr){
    int are = 0;
    /* char *c = NULL; */
    int21 t21;

    if(*arg == '#'){ /* argument starts with # - are:100 */
        are = 4; 
        *trg = 0;
        arg++;
        if(*arg == '+' || *arg == '-' || isdigit(*arg))
            t21.val = atoi(arg); /* number arg */
        else if(typeArg(arg, 0, symbol_head))
            t21.val = getSymbolValue(symbol_head,arg); /* macro arg */
        bits[*ix] = t21.val*8 + are; /* bin word set to value + are(100) */
        (*ix)++;
    } else if(typeArg(arg,1,symbol_head) || typeArg(arg, 2,symbol_head)){
        *trg = 1;
        t21.val = getSymbolValue(symbol_head,arg); /* data/string arg */
        if(t21.val == 0) are = 1;
        else are = 2;
        bits[*ix] = t21.val*8 + are; /* bin word set to value + are(1/2) */
        (*ix)++;
    } else if (*arg == '&') {
        are = 4;
        arg++;
        arg=getStringTillChar(arg, '\0');
        t21.val = getSymbolValue(symbol_head,arg);
    } else { /* external label argument */
        *trg = 1;
        bits[*ix] = 1; /* external label has just are(1) */
        (*ix)++;
    }
}

void bitCMD(int *bits, char *txt, symbol *symbol_head,node *node_head) {
    int opc=0, dst=0, src=0, ix=0, funct=0, are=4;
    node *node_ptr;
    char *c, *cmd_name, *arg1 = NULL, *arg2 = NULL;
    int21 t21_a,t21_b;
    node_ptr = node_head;
    c = txt;
    cmd_name = getStringTillChar(c, ' ');
    opc = opcode(cmd_name);
    funct = function(cmd_name);
    if (opc < 14) { /* at least one arg */
        /* reaching first arg location */
        c += strlen(cmd_name);
        while (*c == ' ') c++;
        /* capturing args */
        if (opc < 5) { /* two args */
            arg1 = getStringTillChar(c, ',');
            c += strlen(arg1);
            while (*c == ' ' || *c == ',') c++;
            arg2 = getStringTillChar(c, '\0');
        } else /* one arg */
            arg1 = getStringTillChar(c, '\0');

        ix = 1;
        if(arg1 && arg2){ /* case we have 2 arguments */
            delBlanks(&arg1);
            delBlanks(&arg2);
            if(registerArg(arg1) && registerArg(arg2)){ /* check if the arguments are registers (are=0) */
                src = 3;
                dst = 3;
                t21_a.val = atoi(arg1+1);
                t21_b.val = atoi(arg2+1);
                /* registers bin word set to regs_num + are(0) */
                bits[ix++] = t21_a.val*((int)pow(2,13)) + t21_b.val*((int)pow(2,8));
            } else if(registerArg(arg1)){ /* only first operand is a register, then check second operand if it's 0,1,2 */
                src = 3;
                t21_a.val = atoi(arg1+1);
                bits[ix++] = t21_a.val*((int)pow(2,13)); /* register bin word set to reg_num + are(0) */
                binARGS(bits,&ix,arg2,&dst,symbol_head,node_ptr);
            } else if(registerArg(arg2)){ /* only second operand is a register, then check first operand if it's 0,1,2 */
                dst = 3;
                binARGS(bits,&ix,arg1,&src,symbol_head,node_head);
                t21_b.val = atoi(arg2+1);
                bits[ix++] = t21_b.val*((int)pow(2,8)); /* register bin word set to reg_num + are(0) */
            } else{ /* two operands are not registers, then check both of them (2,1,0) */
                binARGS(bits,&ix,arg1,&src,symbol_head,node_head);
                binARGS(bits,&ix,arg2,&dst,symbol_head,node_head);
            }

        } else if(arg1) {
            delBlanks(&arg1);
            if(registerArg(arg1)){
                src = 3;
                t21_a.val = atoi(arg1+1);
                bits[ix++] = t21_a.val*((int)pow(2,13)); /* register bin word set to reg_num + are(0) */
            }
            else binARGS(bits,&ix,arg1,&dst,symbol_head,node_head);
        }

    }
    /* set first bin word by fields */
    bits[0] = opc*((int)pow(2,18)) + src*((int)pow(2,16)) + t21_a.val*((int)pow(2,13)) + dst*((int)pow(2,11)) + t21_b.val*((int)pow(2,8)) + funct*((int)pow(2,3)) + are;
}

void DataString(int *bits, char *txt,symbol *symbol_head){ /* puts in "bits" array the ascii value of "strings" and "data" values */
    int ix=0, num;
    char *ptr, *c = txt, *val=NULL;

    while (*c == ' ') c++; /* reach .data or .string beginning */
    while (*c != ' ') c++; /* reach blank after .data or .string */
    while (*c == ' ') c++; /* reach .data or .string content */

    if(*c == '"'){ /* case string */
        c++; /* after " */
        while(*c != '"'){
            bits[ix++] = *c;
            c++;
        }
    }
    else{ /* case data */
        while (*c != '\0'){
            if((ptr = strchr(c,','))) {
                val = getStringTillChar(c, *ptr); /* more than one value left */
                c = ptr+1;

            }
            else {
                val = getStringTillChar(c, '\0'); /* one value left */
                c = (c + strlen(val));
            }
            delBlanks(&val);

            if(typeArg(val,0,symbol_head))
                num = getSymbolValue(symbol_head,val); /* macro value */
            else
                num = atoi(val); /* number value */


            bits[ix++] = num;
        }
    }
}

void printBin(node *node_head){
    node *node_ptr;
    int i, mask, stop;
    stop = (int)pow(2,24);
    node_ptr = node_head;

    while(node_ptr){
        if(*(node_ptr->txt) == ' ' || isalpha(*(node_ptr->txt))) {
            puts(node_ptr->txt);
            for (i = 0; i < node_ptr->DC; i++) {
                mask = 1;
                while (mask < stop) {
                    if (mask & node_ptr->bin[i]) putchar('1');
                    else putchar('0');

                    mask <<= 1;
                }
                putchar('\n');
            }
            puts("-----------------------");
        }
        node_ptr = node_ptr->next;
    }
}

void bitMap(node *node_head, symbol *symbol_head){
    node *node_ptr;
    char *c;
    int *bits;

    node_ptr = node_head;
    if(!node_ptr) return;
    while(node_ptr){
        /* bin array allocation */
        MALLOC(int, node_ptr->bin, node_ptr->DC*sizeof(int))
        bits = node_ptr->bin;
        c = node_ptr->txt;

        if(isalpha(*c)){ /* if line starts with label */
            while(*c != ' ') c++; /* reach first blank after label name: */
            while(*c == ' ') c++; /* reach first statement */

            if(*c == '.') DataString(bits,c,symbol_head); /* case data/string */
            else if(isalpha(*c)) bitCMD(bits,c,symbol_head,node_head); /* case command */
        }
        else if(*c == ' '){ /* the line is a part of a label */
            while (*c == ' ') c++; /* reach to the statement */
            bitCMD(bits,c,symbol_head,node_head); /* just command is an option */
        }
        node_ptr = node_ptr->next;
    }
}



/* Appends char to a string */
void append(char* dst, char c)
{
    int len ,i;
    char ch1, ch2;
    ch1 = *dst;
    *dst = c;
    len = strlen(dst);
    for(i=1; i<=len; i++){
        ch2 = *(dst + i);
        *(dst + i) = ch1;
        ch1 = ch2;
    }
    *(dst + i) = '\0';
}

/*Maps bit values to a hexa-decimal character */
char binToHex(int *num){
    char c;
    unsigned result = 0, mask1 = 0x1, mask2 = 0x2, mask4 = 0x4, mask8 = 0x8; /* Result will hold the number for each 2 bits in original number */
    if (mask1 & *(num)) result++;
    if (mask2 & *(num)) result += 2;
    if (mask4 & *(num)) result += 4;
    if (mask8 & *(num)) result += 8;

    switch (result){
        case 0: c = '0'; break;
        case 1: c =  '1'; break;
        case 2: c =  '2'; break;
        case 3: c = '3'; break;
        case 4: c = '4'; break;
        case 5: c =  '5'; break;
        case 6: c =  '6'; break;
        case 7: c = '7'; break;
        case 8: c = '8'; break;
        case 9: c =  '9'; break;
        case 10: c =  'A'; break;
        case 11: c = 'B'; break;
        case 12: c = 'C'; break;
        case 13: c = 'D'; break;
        case 14: c = 'E'; break;
        case 15: c = 'F'; break;
    }
    *num >>= 4;
    return c;
}

int getBinaryRows(node *head){
    node *ptr;
    int count = 0;

    ptr = head;
    while(ptr){
        count += ptr->DC;
        ptr= ptr->next;
    }
    return count;
}

int getDataRows(node *head){
    node *ptr;
    int count = 0;
    ptr = head;

    while(ptr){
        if (strstr(ptr->txt, ".data") || strstr(ptr->txt, ".string")) count+= ptr->DC;
        ptr = ptr -> next;

    }
    return count;
}

void baseHexaFile(node *head, symbol *symbolHead, char *filename) {
    node *ptr;
    int i, j, binum;
    int numBinRows, numDataRows;
    char *file, *base4, currentSign;
    FILE *fptr;
    MALLOC(char, base4, 8* sizeof(char))
    ptr = head;
    file = strcat(getStringTillChar(filename,'\0'), ".ob");
    fptr = fopen(file, "w+");


    if (!fptr) {
        printf("File allocation error\n");
        return;
    }

    numBinRows = getBinaryRows(head); /*Total number of binary rows */
    numDataRows = getDataRows(head); /*Total number of data/string binary rows */

    fprintf(fptr, "\t%d %d\n", (numBinRows - numDataRows), numDataRows); /*Printing first line */

    while (ptr) {
        for (i=0; i<ptr->DC; i++) {
            binum = ptr->bin[i]; /* Get each binary number */
            for (j=0; j<6; j++){
                currentSign = binToHex(&binum); /*Gets 2 bits of the binary line */
                append(base4, currentSign); /* Appends the base 4 character to the string */
            }
            fprintf(fptr, "0000%d %s\n",(ptr->IC + i), base4); /* Prints the 4 base line */
            *base4 = '\0';
        }
        ptr = ptr->next;
    }
    if(fptr) fclose(fptr);
}



/* Creates external file */
void createExternFile(node *head, char *filename){
    node *node_ptr, *ptr;
    char *start, *val, *afterExt, *beforeExt;
    char *file;
    int extIC;
    FILE *exptr = NULL;

    file = strcat(getStringTillChar(filename,'\0'), ".ext");
    node_ptr = head;

    while(node_ptr){ /* Looping all cmd lines for getting .extern and .entry records */
        start = getStringTillChar(node_ptr->txt, ' '); /* getting first word in command */
        if (strcmp(start, ".extern") == 0){ /* If it's .extern */
            val = node_ptr->txt + strlen(start); /*Getting the label value */
            delBlanks(&val);
            ptr = head; /*Looping all command lines and getting the address of the lines with the extern records*/
            while(ptr){
                start = getStringTillChar(ptr->txt, ' '); /* getting first word in command */
                afterExt = strstr(ptr->txt, val);
                if (afterExt){
                    beforeExt = afterExt -1;
                    afterExt+=strlen(val) + 1;

                    if (*start != '.') {
                        if ((isBlank(*afterExt) || *afterExt == '\0') && (*beforeExt == ' ' || *beforeExt == ','))
                            extIC = ptr->IC + 1;
                        /*If the file doesn't exist */
                        if (!exptr){
                            exptr = fopen(file, "w+");
                            if  (!exptr) {
                                puts("Error opening .ext file");
                                return;
                            }
                        }

                        fprintf(exptr, "%s 0000%d\n", val, extIC);
                    }
                }

                ptr = ptr->next;
            }
        }
        node_ptr = node_ptr->next;
    }
    if(exptr) fclose(exptr);
}

/* Creates entry file */
void createEntryFile(node *head, symbol *symhead, char *filename) {
    node *node_ptr;
    char *start, *label;
    char *file;
    int entIC;
    FILE *enptr = NULL;

    node_ptr = head;
    file = strcat(getStringTillChar(filename,'\0'), ".ent");

    while (node_ptr) {
        start = getStringTillChar(node_ptr->txt, ' '); /* getting first word in command */
        if (strcmp(start, ".entry") == 0) { /* If it's .entry */
            label = getStringTillChar(node_ptr->txt + strlen(start)+1, ' ');
            delBlanks(&label);
            entIC = getSymbolValue(symhead, label);
            /*If the file doesn't exist */
            if (!enptr){
                enptr = fopen(file, "w+");
                if  (!enptr) {
                    puts("Error opening .ent file");
                    return;
                }
            }
            fprintf(enptr, "%s 0000%d\n", label, entIC);
        }
        node_ptr = node_ptr->next;
    }
    if (enptr) fclose(enptr);
}
