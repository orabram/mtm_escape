#include <stdio.h>
#include <stdlib.h>
#include "escapetechnion.h"
#include "mtm_ex3.h"
#include <string.h>
#include <unistd.h>

#define COMMENT_CHAR '#'
#define COMMANDS_NUM 4
#define COMMAND_MAX_LEN 8
#define DELIM " \t"

int programArguments(int argc, char** argv, FILE** input_channel,
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
                return 0;
            }
            *output_channel = fopen(argv[5], "w");
        }
        else if(argv[1] == "o") {
            if (argv[3] != "i") {
                mtmPrintErrorMessage(stderr,
                                     MTM_INVALID_COMMAND_LINE_PARAMETERS);
                return 0;
            }
            *input_channel = fopen(argv[5], "r");
            *output_channel = fopen(argv[3], "w");
        }
        else
        {
            mtmPrintErrorMessage(stderr,
                                 MTM_INVALID_COMMAND_LINE_PARAMETERS);
            return 0;
        }
        if(*input_channel == NULL || *output_channel == NULL)
        {
            mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
            return 0;
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
            return 0;
        }
    }
    return 1;
}
/** This function deals with the entered input, and returns a value according
 * to which type of line we got.
 *
 * @param output_channel:
 * @param input: A char* containing the input. Its maximus size is 250;
 * @return 0 if the line is empty, -1 is the line is an annotation and 1 if the
 * line is a legal order.
 */
int sortInput(char* input)
{
    int counter = 0;
    while(*(input) == '\t' || *(input) == ' ')
    {
        input++;
        counter++;
    }
    if(counter == MAX_LEN)
    {
        return 0;
    }
    if(*input == COMMENT_CHAR)
    {
        return -1;
    }
    return 1;

}

char* get_parameter(char* input)
{
    char* tok = input;
    tok = strtok(NULL, DELIM);
    return tok;
}
MtmErrorCode company_orders(EscapeTechnion escape, char* email,
                            int faculty, char* subcommand)
{
    email = strtok(NULL, DELIM);
    if(!strcmp(subcommand, "add"))
    {
        faculty = atoi(strtok(NULL, DELIM));
        return escapetechnion_add_company(escape, email,
                                          faculty);
    }
    return escapetechnion_remove_company(escape, email);
}

MtmErrorCode room_orders(EscapeTechnion escape, char* email,
                            int id, int price, char* working_hrs,
                         int difficulty, int faculty, int num_ppl,
                         char* subcommand)
{
    if(!strcmp(subcommand, "add"))
    {
        email = strtok(NULL, DELIM);
        id = atoi(strtok(NULL, DELIM));
        price = atoi(strtok(NULL, DELIM));
        num_ppl = atoi(strtok(NULL, DELIM));
        working_hrs = strtok(NULL, DELIM);
        difficulty = atoi(strtok(NULL, DELIM));
        return escapetechnion_add_room(escape, email, id, price, num_ppl,
                                       working_hrs, difficulty);
    }
    faculty = atoi(strtok(NULL, DELIM));
    id = atoi(strtok(NULL, DELIM));
    return escapetechnion_remove_room(escape, faculty, id);
}

MtmErrorCode customer_orders(EscapeTechnion escape, char* email, int id,
                             int num_ppl, char* time, int skill, int faculty,
                             char* subcommand)
{
    email = strtok(NULL, DELIM);
    if(!strcmp(subcommand, "remove"))
    {
        return escapetechnion_remove_customer(escape, email);
    }
    if(!strcmp(subcommand, "recommend"))
    {
        num_ppl = atoi(strtok(NULL, DELIM));
        return escapetechnion_recommended_room(escape, email, num_ppl);
    }
    faculty = atoi(strtok(NULL, DELIM));
    if(!strcmp(subcommand, "add"))
    {
        skill = atoi(strtok(NULL, DELIM));
        return escapetechnion_add_customer(escape, email, faculty, skill);
    }
    id = atoi(strtok(NULL, DELIM));
    time = strtok(NULL, DELIM);
    num_ppl = atoi(strtok(NULL, DELIM));
    return escapetechnion_create_order(escape, email, faculty, id, time,
                                       num_ppl);
}

MtmErrorCode parseInput(EscapeTechnion escape, char* input)
{
    char* command = strtok(input, DELIM);
    char* subcommand = strtok(NULL, DELIM);
    char *email = NULL, *working_hrs = NULL, *time = NULL;
    int faculty=0, id=0, num_ppl=0, price=0, difficulty=0, skill=0;
    if(command == NULL || subcommand == NULL)
    {
        return MTM_OUT_OF_MEMORY;
    }
    if(!strcmp(command, "company"))
    {
        return company_orders(escape, email, faculty, subcommand);
    }

    if(!strcmp(command, "room"))
    {
        return room_orders(escape, email, id, price, working_hrs, difficulty, faculty,
                    num_ppl, subcommand);
    }
    if(!strcmp(command, "escaper"))
    {
        return customer_orders(escape, email, id, num_ppl, time, skill,
                               faculty, subcommand);
    }
    if(!(strcmp(subcommand, "day")))
    {
        return escapetechnion_reportday(escape);
    }
    escapetechnion_reportbest(escape);
    return MTM_SUCCESS;
}

int main(int argc, char** argv){

    FILE* input_channel;
    FILE* output_channel;
    int worked = programArguments(argc, argv, &input_channel, &output_channel);
    if(!worked)
    {
        return 0;
    }
    EscapeTechnion escape = create_escapetechnion();
    if(escape == NULL)
    {
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        return 0;
    }
    escapetechnion_set_output_channel(output_channel);
    MtmErrorCode code;
    char* input = malloc(MAX_LEN);  //The maximum length of a line is MAX_LEN.
    if(input == NULL)
    {
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        return 0;
    }
    while(fgets(input, MAX_LEN, input_channel) != NULL)
    {
        if(sortInput(input) == 1)
        {
            code = parseInput(escape, input);
            if(code != MTM_SUCCESS)
            {
                mtmPrintErrorMessage(stderr, code);
                if(code == MTM_OUT_OF_MEMORY)
                {
                    return 0;
                }
            }
        }
    }

}