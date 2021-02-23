#include <stdio.h>

#define MAX_SIZE 101 /* define the string's max character size */

void sequence_f(char arr[])
{
    int up=0; 
    int down=0;
    int really_up=0;
    int really_down=0;
    int no_order=0; /* declare the variables that will keep count of the way the sequence is arranged */
    int i=0;
    int j=1;
    while (arr[j] != '\0')
    {
        printf("The Character at %d Index Position = %c \n", i, arr[i]);
        i++;
        printf("The Character at %d Index Position = %c \n", j, arr[j]);
        j++;
        if (arr[i] < arr[j])
            up++;
        if (arr[i] > arr[j])
            down++;
        else
            no_order++;
    }
    printf("The string is %s\n", arr);
    printf("up %d\n", up);
    printf("really up %d\n", really_up);
    printf("down %d\n", down);
    printf("really down %d\n", really_down);
    printf("no order %d\n", no_order);
}

int main ()
{
    char string[MAX_SIZE]; /* initialize the array that will keep the sting the user will input */
    printf("Enter a string and this program will return you if the string's characters are arranged a certain way or not:\n"); /* print output to the user */
    scanf("%s", string);  /* save the user's input in the string array */
    sequence_f(string); /* call the sequence_f function with the string the user entered */
}
