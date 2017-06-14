#include <stdio.h>
#include <stdlib.h>
#include "escapetechnion.h"
#include "mtm_ex3.h"
#include <string.h>
#include <unistd.h>

#define COMMENT_CHAR '#'
#define COMMANDS_NUM 4
#define COMMAND_MAX_LEN 8
#define DELIM " \t\n" //The chars we ignore when looking at the input.

/**
 * This function receives the argc and argv of the main function, and checks if
 * they're legal.
 * @param argc: The amount of arguements the program has gotten. Must be either
 * 1, 3, or 5(for 0, 2 and 4 parameters).
 * @param argv: The array which stores the arguments the program was given.
 * @param input_channel: A FILE**, which points to where the input channel
 * will be stored.
 * @param output_channel: A FILE**,  which points to where the output channel
 * will be stored.
 * @return
 * 0 Upon failure(invalid parameters, invalid filename).
 * 1 otherwise.
 */
int programArguments(int argc, char** argv, FILE** input_channel,
                      FILE** output_channel)
{
    if(argc != 1 && argc != 3 && argc != 5)
    {
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
        return 0;
    }
    if(argc == 5)
    {
        if(!strcmp(argv[1],"-i"))
        {
            *input_channel = fopen(argv[2], "r");
            if(strcmp(argv[3],"-o") != 0)
            {
                mtmPrintErrorMessage(stderr,
                                     MTM_INVALID_COMMAND_LINE_PARAMETERS);
                return 0;
            }
            *output_channel = fopen(argv[4], "w");
        }
        else if(!strcmp(argv[1],"-o")) {
            if (strcmp(argv[3], "-i") != 0) {
                mtmPrintErrorMessage(stderr,
                                     MTM_INVALID_COMMAND_LINE_PARAMETERS);
                return 0;
            }
            *input_channel = fopen(argv[4], "r");
            *output_channel = fopen(argv[2], "w");
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
        if(!strcmp(argv[1],"-i"))
        {
            *input_channel = fopen(argv[2], "r");
            *output_channel = stdout;
        }
        else if(!strcmp(argv[1],"-o"))
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
 * @param input: A char* containing the input. Its maximum size is 250.
 * @return 0 if the line is empty, -1 is the line is an annotation and 1 if the
 * line is a legal order.
 */
int sortInput(char* input)
{
    int counter = 0;
    while(*(input) == '\t' || *(input) == ' ' || *(input) == '\n')
    {
        if(*(input) == '\n')
        {
            return 0;
        }
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

/**
 * This function receives the relevant parameters for all of company's orders,
 * and uses them according to the value in subcommand.
 * @param escape: The EscapeTechnion we'll be working with.
 * @param email: The email address of the company we wish to create/destroy.
 * @param faculty: The faculty of the company we wish to create/destroy.
 * @param subcommand: Contains the rest of the order.
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if memory allocation problem occurred
 * MTM_INVALID_PARAMETER if the e-mail is invalid
 * MTM_EMAIL_ALREADY_EXISTS if a company with the same e-mail already exists
 * (when adding).
 * MTM_COMPANY_EMAIL_DOES_NOT_EXIST if no company with such e-mail exists(when
 * deleting).
 * MTM_RESERVATION_EXISTS if the company has reservations and cannot be removed
 * MTM_SUCCESS otherwise
 */
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

/**
 * This function receives the relevant parameters for all of room's orders,
 * and uses them according to the value in subcommand.
 * @param escape: EscapeTechnion who stores the orders.
 * @param email: The email address of the company which stores the  room.
 * @param id: The id of the order within the faculty.
 * @param price: The price of visitation per man.
 * @param working_hrs: The working hours of the room.
 * @param difficulty: The difficulty of the room.
 * @param faculty: The faculty of the room.
 * @param num_ppl: The amount of people it's optimized to host.
 * @param subcommand: Contains the rest of the order.
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if memory allocation problem occurred
 * MTM_COMPANY_EMAIL_DOES_NOT_EXIST if there's no company with such e-mail
 * MTM_ID_ALREADY_EXIST if a room with such ID already exists
 * MTM_RESERVATION_EXISTS if there is a reservation for this room, and therefore
 *                        it cannot be removed
 * MTM_ID_DOES_NOT_EXIST if a room with such ID does not exist
 * MTM_SUCCESS otherwise
 */
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

/**
 * This function receives the relevant parameters for all of escaper's orders,
 * and uses them according to the value in subcommand.
 * @param escape: Contains the actions we wish to make.
 * @param email: The email address of the customer.
 * @param id: The id of the room, if the action is to order a room.
 * @param num_ppl: The amount of people in the order, if the action is to order
 * a room.
 * @param time: The time until the order, if the action is to order a room.
 * @param skill: The skill level of the Customer. Only relevant if you wish
 * to create one.
 * @param faculty: The faculty of the customer. Needed to either get a
 * recommencation for a room or ordering one.
 * @param subcommand: Contains the rest of the order.
 * @return
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_ID_DOES_NOT_EXIST if no room with such ID exists
 * MTM_CLIENT_IN_ROOM if the client already has an order for this time.
 * MTM_ROOM_NOT_AVAILABLE if the room is not available at this time
 * MTM_INVALID_PARAMETER if the e-mail is invalid
 * MTM_CLIENT_EMAIL_DOES_NOT_EXIST if no client with such email exists
 * MTM_NO_ROOMS_AVAILABLE if no rooms available to book
 * MTM_CLIENT_EMAIL_DOES_NOT_EXIST if no client with such email exists
 * MTM_NO_ROOMS_AVAILABLE if no rooms available to book
 * MTM_NULL_PARAMETER if escape is NULL
 * MTM_OUT_OF_MEMORY if memory allocation problem occurred

 * MTM_SUCCESS otherwise


 */
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

/**
 * Receives a command and routes it to the relevant function according to the
 * object(customer_order, company_order, etc).
 * @param escape: Contains the actions and the info we need for those actions.
 * @param input: The input from the user. We know it's a legal line thanks
 * to earlier tests.
 * @return
 * The code of the relevant function(look above for more info).

 */
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
        return room_orders(escape, email, id, price, working_hrs, difficulty,
                           faculty, num_ppl, subcommand);
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

/**
 * Closes the input and output channels if they're files, and frees the memory
 * of the program.
 * @param input_channel: The input channel of the company.
 * @param output_channel: The output channel of the company.
 * @param escape: Contains the info + actions.
 * @param input : The input received from the program. Will be NULL if the
 * program is terminated before reading any input.
 */
void close_program(FILE* input_channel, FILE* output_channel,
                   EscapeTechnion escape, char* input)
{
    if(input_channel != stdin)
    {
        fclose(input_channel);
    }
    if(output_channel != stdout)
    {
        fclose(output_channel);
    }
    escapetechnion_destroy(escape);
    free(input);
}

int main(int argc, char** argv) {

    //If there aren't any parameters, use the default input and output channels.
    FILE *input_channel = stdin;
    FILE *output_channel = stdout;
    int worked = programArguments(argc, argv, &input_channel,
                                  &output_channel);
    if (!worked) {
        return 0;
    }
    EscapeTechnion escape = create_escapetechnion();
    if (escape == NULL) {
        close_program(input_channel, output_channel, escape, NULL); //Input
        // isn't defined, so we send NULL instead
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        return 0;
    }
    escapetechnion_set_output_channel(escape, output_channel);
    MtmErrorCode code;
    char *input = malloc(
            MAX_LEN);  //The maximum length of a line is MAX_LEN.
    if (input == NULL) {
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        close_program(input_channel, output_channel, escape, input);
        return 0;
    }
    while (fgets(input, MAX_LEN, input_channel) != NULL) {
        if (sortInput(input) == 1) {
            code = parseInput(escape, input);
            if (code != MTM_SUCCESS) {
                mtmPrintErrorMessage(stderr, code);
                if (code == MTM_OUT_OF_MEMORY) {
                    close_program(input_channel, output_channel, escape,
                                  input);
                    return 0;
                }
            }
        }
    }
    close_program(input_channel, output_channel, escape, input);
}