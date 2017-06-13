#include <stdio.h>
#include <stdlib.h>
#include "escapetechnion.h"
#include "mtm_ex3.h"
#include <string.h>


#define COMMENT_CHAR "#"

void programArguments(int argc, char** argv, FILE** input_channel,
                      FILE** output_channel)
{
    if(argc != 1 && argc != 3 && argc != 5)
    {
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
        exit(0);
    }
    //If there aren't any parameters, use the default input and output channels.
    if(argc == 1)
    {
        *input_channel = stdin;
        *output_channel = stdout;
    }
    if(argc == 5)
    {
        if(argv[1] == "i")
        {
            *input_channel = fopen(argv[3], "r");
            if(argv[3] != "o")
            {
                mtmPrintErrorMessage(stderr,
                                     MTM_INVALID_COMMAND_LINE_PARAMETERS);
                exit(0);
            }
            *output_channel = fopen(argv[5], "w");
        }
        else if(argv[1] == "o") {
            if (argv[3] != "i") {
                mtmPrintErrorMessage(stderr,
                                     MTM_INVALID_COMMAND_LINE_PARAMETERS);
                exit(0);
            }
            *input_channel = fopen(argv[5], "r");
            *output_channel = fopen(argv[3], "w");
        }
        else
        {
            mtmPrintErrorMessage(stderr,
                                 MTM_INVALID_COMMAND_LINE_PARAMETERS);
            exit(0);
        }
        if(*input_channel == NULL || *output_channel == NULL)
        {
            mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
            exit(0);
        }

    }

    if(argc == 3)
    {
        if(argv[1] == "i")
        {
            *input_channel = fopen(argv[2], "r");
            *output_channel = stdout;
        }
        else if(argv[1] == "o")
        {
            *output_channel = fopen(argv[2], "w");
            *input_channel = stdin;
        }
        else
        {
            mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
            exit(0);
        }
    }
}
/**
 *
 * @param output_channel
 * @param input
 * @return
 */
int parseInput(FILE* output_channel, char* input)
{
    while(*(input) != NULL)
    {
        if(*(input) == "\t" || *(input) == " ")
        {
            input++;
        }
    }

}

int main(int argc, char** argv){

    FILE* input_channel;
    FILE* output_channel;
    programArguments(argc, argv, &input_channel, &output_channel);
    char* input = malloc(MAX_LEN);  //The maximum length of a line is MAX_LEN.
    if(input == NULL)
    {
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        return 0;
    }
    while(fgets(input, MAX_LEN, input_channel) != NULL)
    {
        //Lines that start with # are comment lines.
        if(!strcmp(input[0], COMMENT_CHAR))
        {
            continue;
        }
    }

}