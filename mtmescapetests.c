#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define COMMANDS_NUM 4
#define COMMAND_MAX_LEN 8
#define DELIM " \t"

int parseInput(FILE* output_channel, char* input)
{
    int counter = 0;
    while(*(input))
    {
        if(*(input) == '\t' || *(input) == ' ')
        {
            input++;
        }
        else
        {
            counter++;
            input++;
        }
    }
    return counter;

}
/*
char* get_command(char* input)
{
    //char* command = malloc(COMMAND_MAX_LEN);
    int comsize = 0;
    while(*(input + comsize) != ' ' && *(input  + comsize) != '\t')
    {
        comsize++;
    }
    char* command = malloc(comsize + 1);
    if(command == NULL)
    {
        return NULL;
    }
    memcpy(command, input, comsize);
    command[comsize] = '\0';
    input += comsize;
    while(*input == ' ' || *input == '\t' || *input == '\0')
    {
        input++;
    }
    return command;

}
 */


char* get_parameter(char* input)
{
    char* tok = input;
    tok = strtok(NULL, DELIM);
    return tok;
}

int main(){

    /*
    char* input = "company  \t  simon@gmail.com";
    char *tok;
    tok = strtok(input, " ");
    while(!strcmp(tok,"\t"))
    {
        tok = strtok(NULL, " ");
    }
    char* comp = tok;
    printf("%s\n", comp);
    tok = strtok(NULL, " ");
    while(!strcmp(tok,"\t"))
    {
        tok = strtok(NULL, " ");
    }
    char* email = tok;
    printf("%s\n", email);


    char str[80] = "company  \t  simon@gmail.com";
    char delim[] = " \t";
    char *token;

     get the first token
    token = strtok(str, delim);

     walk through other tokens
    while( token != NULL )
    {
        printf("%s\n", token);
        token = strtok(NULL, delim);
    }

    return(0);
     */
    char str[80] = "company  \t  simon@gmail.com  33";
    char* param = strtok(str, DELIM);
    printf("%s\n", param);/*
    char* tok = get_parameter(param);
    printf("%s\n", tok);
    tok = get_parameter(param);
    printf("%s\n", tok);
    */
    char* email = strtok(NULL, DELIM), *id = strtok(NULL, DELIM);
    printf("%s\n", email);
    printf("%s\n", id);







}