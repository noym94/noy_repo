#include "definitions.h"

int argCheck(node * node_ptr,char arg[]) { /* individual argument check */
    int i = 0;
    char c;
    c = arg[i];

    if(isalpha(c)){ /* argument starts alphabetic */
        c = arg[++i];
        while(isalnum(c)) c = arg[++i];
        if (c == '[')
        {
            c = arg[++i];
            while(isalnum(c)) c = arg[++i];
            if (c == ']') c = arg[++i];
            else {
                printf("line [%d]: missing ']' character\n", node_ptr->node_num);
                return 1;
            }
        }
    }
    else if(c == '#'){ /* argument starts with # */
        c = arg[++i];
        if(isalpha(c)){ /* # argument starts alphabetic */
            while(isalnum(c)) c = arg[++i];
        }
        else if(isdigit(c)){ /* argument is numeric */
            while(isdigit(c)) c = arg[++i];
        }
        else if((c == '+')||(c == '-')) { /* argument is signed number */
            c = arg[++i];
            while(isdigit(c)) c = arg[++i];
        }
        else { /* error - undefined argument */
            printf("line [%d]: illegal character in argument call\n", node_ptr->node_num);
            return 1;
        }

    }
    else if (c == '&'){
        c = arg[++i];
        if(isalpha(c)){ /* # argument starts alphabetic */
            while(isalnum(c)) c = arg[++i];
        }
        else if(isdigit(c)){ /* argument is numeric */
            while(isdigit(c)) c = arg[++i];
        }
        else if((c == '+')||(c == '-')) { /* argument is signed number */
            c = arg[++i];
            while(isdigit(c)) c = arg[++i];
        }
        else { /* error - undefined argument */
            printf("line [%d]: illegal character in argument call\n", node_ptr->node_num);
            return 1;
        }
    }

    if(c != '\0'){ /* error - end of string not reached */
        printf("line [%d]: wrong argument call\n", node_ptr->node_num);
        return 1;
    }
    else return 0;
}

int zeroArgCmd(node * node_ptr,int index) {
    char c;
    c = node_ptr->txt[index];

    while (c == ' ') c = node_ptr->txt[++index]; /* free to blank */
    if(c != '\0'){ /* if non-blank char appeared */
        printf("line [%d]: expected no arguments\n", node_ptr->node_num);
        return 1;
    }

    return 0;
}

int oneArgCmd(node * node_ptr,int index) {
    char arg1[100];
    char c;
    int start;
    start = index; /* saving the beginning of argument */
    c = node_ptr->txt[index];

    while ((c != ' ')&&(c != '\0')) c = node_ptr->txt[++index]; /* finding end */
    strncpy(arg1, node_ptr->txt+start, index - start); /* copying the argument's name */
    arg1[index-start] = '\0';
    while (c == ' ') c = node_ptr->txt[++index]; /* free to blank */
    if(c != '\0'){ /* if non-blank char appeared */
        printf("line [%d]: wrong argument call\n", node_ptr->node_num);
        return 1;
    }
    if(strlen(arg1) > 31) { /* length check */
        printf("line [%d]: argument name is too long\n", node_ptr->node_num);
        return 1;
    }
    return argCheck(node_ptr,arg1);
}

int twoArgCmd(node * node_ptr,int index) {
    char arg1[100];
    char arg2[100];
    char c;
    int start;
    start = index; /* saving the beginning of argument1 */
    c = node_ptr->txt[index];
    while ((c != ' ')&&(c != ',')&&(c != '\0')) c = node_ptr->txt[++index]; /* finding end */
    if(c == '\0'){ /* error - no blanks or , */
        printf("line [%d]: wrong argument call - missing comma\n", node_ptr->node_num);
        return 1;
    }
    strncpy(arg1, node_ptr->txt+start, index - start); /* copying the first argument name */
    arg1[index-start] = '\0';
    /* finding the beginning of argument 2 */
    while (c == ' ') c = node_ptr->txt[++index];
    if(c == ',') c = node_ptr->txt[++index];
    else { /* if comma wasn't found */
        printf("line [%d]: wrong argument call - missing comma\n", node_ptr->node_num);
        return 1;
    }

    while (c == ' ') c = node_ptr->txt[++index];
    start = index; /* saving the beginning of argument2 */
    while ((c != ' ')&&(c != '\0')) c = node_ptr->txt[++index]; /* finding end */
    strncpy(arg2, node_ptr->txt+start, index - start); /* copying the second argument name */
    arg2[index-start] = '\0';
    while (c == ' ') c = node_ptr->txt[++index]; /* free to blank */
    if(c != '\0'){ /* if non-blank char appeared */
        printf("line [%d]: wrong argument call\n", node_ptr->node_num);
        return 1;
    }
    if((strlen(arg1) > 31)||(strlen(arg2) > 31)) { /* length check */
        printf("line [%d]: at least one argument name is too long\n", node_ptr->node_num);
        return 1;
    }
    return (argCheck(node_ptr,arg1)+argCheck(node_ptr,arg2));

}

int labelCheck(node * node_ptr, int index){
    int label_index = 0;
    char * label; /* label var definition */
    char c;
    c = node_ptr->txt[index];
    MALLOC(char,label,100*sizeof(char))

    if (!isalpha(c)){
        printf("line [%d]: label name must start with a letter\n",node_ptr->node_num);
        fflush(stdout);
        return 1;
    }

    if (index == 0) { /* In case checking label definition */
        while ((c != ':') && (c != ' ') && (c != '\0')){
            label[label_index++] = c;
            c = node_ptr->txt[++index];
        }
        label[label_index] = '\0'; /* ending label name as string */

        if (c != ':') { /*Label ended without ':'*/
            printf("line [%d]: wrong label definition\n", node_ptr->node_num);
            fflush(stdout);
            return 1;
        }
        c = node_ptr->txt[++index];
        while (c == ' ') c = node_ptr->txt[++index];
        nonZeroIndex(node_ptr, index); /*checks what is the next argument after label*/
    }
    else { /* checking label as argument */
        while (c == ' ') c = node_ptr->txt[++index];
        while ((c != ' ') && (c != '\0')) {
            label[label_index++] = c;
            c = node_ptr->txt[++index];
        }
        label[label_index] = '\0'; /* ending label name as string */

        while (c == ' ') c = node_ptr->txt[++index];
        if (c != '\0') { /*Label argument ended wrong */
            printf("line [%d]: wrong label argument\n", node_ptr->node_num);
            fflush(stdout);
            return 1;
        }
    }


    /*General label syntax check from char number 2*/
    if (label_index > 31){
        printf("line [%d]: label is too long\n",node_ptr->node_num);
        fflush(stdout);
        return 1;
    }

    index = 1;
    while ((index < label_index) && isalnum(label[index])) index++;

    if (label[index] != '\0'){
        printf("line [%d]: label syntax error\n",node_ptr->node_num);
        fflush(stdout);
        return 1;
    }

    return 0;
}
int isDot(node * node_ptr, int index) { /* syntax check for lines begin with a dot */
    int i, start;
    char str_copy[100];
    char c;
    /* NOTE - copying the command line from index to end*/
    strncpy(str_copy, node_ptr->txt+index, strlen(node_ptr->txt) - index);
    str_copy[strlen(node_ptr->txt) - index] = '\0';
    /* define */
    if(strcmp(strtok(str_copy," "),".define") == 0){
        if (index != 0)
        {
            printf("line [%d]: \'.define\' macro misplaced\n",node_ptr->node_num);
            return 1;
        }
        i = index + 8;
        c = node_ptr->txt[i];
        if (isalpha(c)) { /* case macro use */
            start = i;
            c = node_ptr->txt[++i];
            while((c != ' ')&&(c != '\0')) {
                if (isalnum(c)) c = node_ptr->txt[++i];
                else {
                    printf("line [%d]: wrong syntax in \'.define\' macro\n", node_ptr->node_num);
                    return 1;
                }
            }
            if ((i - start) > 31) { /* if macro too long */
                printf("line [%d]: macro is too long \n", node_ptr->node_num);
                return 1;
            }
        }
        while(c == ' ') c = node_ptr->txt[++i];
        if(c != '=') {
            printf("line [%d]: wrong syntax on \'.define\' macro\n",node_ptr->node_num);
            return 1;
        }
        c = node_ptr->txt[++i]; /* after = sign*/
        while(c == ' ') c = node_ptr->txt[++i];
        if ((c == '+') || (c == '-')) c = node_ptr->txt[++i];
        if(isdigit(c)) { /* value check */
            while (isdigit(c) == 0) c = node_ptr->txt[++i];
        }
        else{
            printf("line [%d]: \'.define\' macro value must be number\n",node_ptr->node_num);
            return 1;
        }

        c = node_ptr->txt[++i];
        while(c == ' ') c = node_ptr->txt[++i];
        if(c!='\0'){
            printf("line [%d]: wrong syntax on \'.define\' macro\n",node_ptr->node_num);
            return 1;
        }
    }
        /* data */
    else if(strcmp(strtok(str_copy," "),".data")  == 0) {
        if(index != 0){
            i = index + 5;
            c = node_ptr->txt[i];
            while(c == ' ') c = node_ptr->txt[++i];
            if(c == ',') { /* checking if begins with ',' */
                printf("line [%d]: \'.data\' instruction can't begin with ','\n",node_ptr->node_num);
                return 1;
            }
            while(c != '\0') { /* running on the data */
                while ((c != ',') && (c != '\0')) {/* ensuring one ',' is dividing each argument */
                    while(c == ' ') c = node_ptr->txt[++i]; /* free to blank */
                    if((c == '+') || (c == '-')) { /* case - signed number */
                        c = node_ptr->txt[++i];
                        while(isdigit(c)) c = node_ptr->txt[++i];
                    }
                    else if(isdigit(c)) /* case unsigned number */
                        while (isdigit(c)) c = node_ptr->txt[++i];
                    else if(isalpha(c)) { /* case macro use */
                        start = i;
                        c = node_ptr->txt[++i];
                        while(isalnum(c)) c = node_ptr->txt[++i];
                        if((i-start) > 31){ /* if macro too long */
                            printf("line [%d]: wrong syntax in \'.data\' instruction \n",node_ptr->node_num);
                            return 1;
                        }
                    }
                    else{ /* error if non of the above is issued */
                        printf("line [%d]: wrong syntax in \'.data\' instruction \n",node_ptr->node_num);
                        return 1;
                    }
                    if((c != ',') && (c != '\0') && (c != ' ')){ /* error if data arg is not valid */
                        printf("line [%d]: passed illegal \'.data\' argument/s \n",node_ptr->node_num);
                        return 1;
                    }
                }
                if(c == ','){
                    c = node_ptr->txt[++i]; /* next arg */
                    while (c == ' ') c = node_ptr->txt[++i]; /* free to blank */
                    if (c == ',') { /* double ',' check, error if needed */
                        printf("line [%d]: \'.data\' instruction can't contain two ',' in a row\n", node_ptr->node_num);
                        return 1;
                    }
                }
            }
        }
        else {
            printf("line [%d]: \'.data\' instruction must be followed by label\n", node_ptr->node_num);
            return 1;
        }
    }
        /* string */
    else if(strcmp(strtok(str_copy," "),".string") == 0) {
        if (index != 0)
        {
            i = index + 7;
            c = node_ptr->txt[i];
            while(c == ' ') c = node_ptr->txt[++i];
            if (c != '\"') /* check value is wrapped */
            {
                printf("line [%d]: \'.string\' values must be wrapped by \'\"\' \n", node_ptr->node_num);
                return 1;
            }
            c = node_ptr->txt[++i];
            while ((c != '\"') && (c != '\0')) c = node_ptr->txt[++i];
            if (c != '\"'){
                printf("line [%d]: \'.string\' values must be wrapped by \'\"\' \n", node_ptr->node_num);
                return 1;
            }
            c = node_ptr->txt[++i];
            while(c == ' ') c = node_ptr->txt[++i];
            if (c != '\0'){
                printf("line [%d]: \'.string\' values must be wrapped by \'\"\' \n", node_ptr->node_num);
                return 1;
            }
        }
        else {
            printf("line [%d]: \'.string\' instruction must be followed by label\n",node_ptr->node_num);
            return 1;
        }
    }
        /* entry */
    else if(strcmp(strtok(str_copy," "),".entry")  == 0) {
        if (index != 0)
        {
            printf("line [%d]: \'.entry\' instruction misplaced\n",node_ptr->node_num);
            return 1;
        }
        i = 6;
        c = node_ptr->txt[i];
        while(c == ' ') {
            i++;
            c = node_ptr->txt[i];
        }
        return labelCheck(node_ptr, i);
    }
        /* extern */
    else if(strcmp(strtok(str_copy," "),".extern") == 0) {
        if (index != 0)
        {
            printf("line [%d]: \'.extern\' instruction misplaced\n",node_ptr->node_num);
            return 1;
        }
        i = 7;
        c = node_ptr->txt[i];
        while (c == ' ') c = node_ptr->txt[++i];
        return labelCheck(node_ptr, i);
    }
    else{
        printf("line [%d]: undefined instruction followed by a dot\n",node_ptr->node_num);
        return 1;
    }
    return 0;
}

int nonZeroIndex(node * node_ptr, int index){
    char cmd_name[100];
    char c;
    if (node_ptr->txt[index] == '.') /* for arguments begins with '.'*/
        return isDot(node_ptr, index);
    strcpy(cmd_name, node_ptr->txt+index); /* getting the command name */
    strtok(cmd_name, " ");
    index += strlen(cmd_name);
    c = node_ptr->txt[index];
    while (c == ' ') c = node_ptr->txt[++index];

    /* commands with two arguments */
    if ((strcmp(cmd_name, "mov") == 0) ||
        (strcmp(cmd_name, "cmp") == 0) ||
        (strcmp(cmd_name, "add") == 0) ||
        (strcmp(cmd_name, "sub") == 0) ||
        (strcmp(cmd_name, "lea") == 0)) return twoArgCmd(node_ptr, index);
    /* commands with one argument */
    if ((strcmp(cmd_name, "not") == 0) ||
        (strcmp(cmd_name, "clr") == 0) ||
        (strcmp(cmd_name, "inc") == 0) ||
        (strcmp(cmd_name, "dec") == 0) ||
        (strcmp(cmd_name, "jmp") == 0) ||
        (strcmp(cmd_name, "bne") == 0) ||
        (strcmp(cmd_name, "red") == 0) ||
        (strcmp(cmd_name, "prn") == 0) ||
        (strcmp(cmd_name, "jsr") == 0)) return oneArgCmd(node_ptr, index);
    /* commands with no argument */
    if ((strcmp(cmd_name, "rts") == 0) ||
        (strcmp(cmd_name, "stop") == 0)) return zeroArgCmd(node_ptr, index);
    /* undefined cmd */
    printf("line [%d]: command not found \n", node_ptr->node_num);
    return 1;
}

int syntaxValidator(node * head){ /* syntax validation function*/
    node * node_ptr;
    int error_count=0, i=0;
    char c;
    node_ptr = head; /* copying first node */
    while (node_ptr != NULL)
    {
        if (node_ptr->txt[0] == '.') /* if command starts with dot */
            error_count += isDot(node_ptr, 0);
        else if (isalpha(node_ptr->txt[0])) /* if command starts with label */
            error_count += labelCheck(node_ptr, 0);
        else if  (node_ptr->txt[0] == ' '){
            c = node_ptr->txt[i];
            while (c == ' ') c = node_ptr->txt[++i];
            error_count += nonZeroIndex(node_ptr, i);
        }
        else{
            if (node_ptr->txt[0] != ';'){
                printf("line [%d]: illegal beginning of line \n",node_ptr->node_num);
                return 1;
            }
        }
        node_ptr=node_ptr->next;
    }
    return error_count;
}
