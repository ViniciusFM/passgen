#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#include <stdint.h>
#include <inttypes.h>

#define BILLION  1000000000L

#define LETTERS "abcdefghijklmnopqrstuvwxyz\0"
#define NUMBERS "0123456789\0"
#define SYMBOLS "!@#$*()_,.;:?+-=\0"
#define DEFAULT_PASS_SIZE 16
//flags
#define LOWER_LETTERS 2
#define UPPER_LETTERS 4
#define P_NUMBERS 8
#define P_SYMBOLS 16

typedef struct {
    unsigned int flags;
    unsigned int pswd_length;
} program_params;

void print_help(void);
program_params handle_program_input(int argc, char* argv[]);
size_t get_base_size(unsigned int flag);
void to_upper_case(char* str);
char* new_charset_base(unsigned int flag);
char* produce_password(char* character_base, unsigned int password_length);

int main(int argc, char* argv[])
{
    program_params pparams;
    char* charset_base;
    char* password;
    
    pparams = handle_program_input(argc, argv);
    charset_base = new_charset_base(pparams.flags);
    password = produce_password(charset_base, pparams.pswd_length);
    free(charset_base);
    printf("%s\n", password);
    free(password);

    return 0;
}

void print_help(void)
{
    printf("\nUsage:\n\n");
    printf("\t> passgen [flags] [password_length]\n\n");
    printf("Flags are: \n\n");
    printf("\tl -> lowercase [a-z]\n");
    printf("\tL -> uppercase [A-Z]\n");
    printf("\tn -> number [0-9]\n");
    printf("\ts -> symbol characters [%s]\n\n", SYMBOLS);
    printf("Password length restriction [default 16]: \n\n");
    printf("\tpassword_length must be greater than 0 and lesser than %u.\n\n", UINT_MAX);
    printf("Example of use:\n\n");
    printf("* All flags activated and default 16 characters length:\n\n");
    printf("\t> passgen\n");
    printf("\t9LwVZ06JF/)G-8H*\n\n");
    printf("* Letters and numbers flags activated and 8 character length:\n\n");
    printf("\t> passgen lLn 8\n");
    printf("\ta18bR69x\n\n");
}

program_params handle_program_input(int argc, char* argv[]) 
{
    program_params pparams;
    int i;
    pparams.flags = LOWER_LETTERS|UPPER_LETTERS|P_NUMBERS|P_SYMBOLS;
    pparams.pswd_length = 16;
    for(i = 1; i < argc; i++) 
    {
        char* arg = argv[i];
        int j;
        size_t arglen = strlen(arg);
        if(arg[0] > 47 && arg[0] < 58) 
        {
            unsigned long int converted_number = strtol(arg, NULL, 10);
            if(converted_number > 0 && converted_number < UINT_MAX) 
            {
                pparams.pswd_length = converted_number;
                continue;
            }
            else
            {
                print_help();
                exit(EXIT_FAILURE);
            }
        }
        pparams.flags = 0;
        for(j = 0; j < arglen; j++)
        {
            switch (arg[j])
            {
                case 'l': pparams.flags |= LOWER_LETTERS; break;
                case 'L': pparams.flags |= UPPER_LETTERS; break;
                case 'n': pparams.flags |= P_NUMBERS; break;
                case 's': pparams.flags |= P_SYMBOLS; break;
                default:
                    print_help();
                    exit(EXIT_FAILURE);
                    break;
            }
        }
    }
    return pparams;
}

size_t get_base_size(unsigned int flag)
{
    size_t base_size = 0;
    if(flag & LOWER_LETTERS)
        base_size += strlen(LETTERS);
    if(flag & UPPER_LETTERS)
        base_size += strlen(LETTERS);
    if(flag & P_NUMBERS)
        base_size += strlen(NUMBERS);
    if(flag & P_SYMBOLS)
        base_size += strlen(SYMBOLS);
    return base_size;
}

void to_upper_case(char* str)
{
    int i;
    size_t strsize = strlen(str);
    for(i = 0; i < strsize; i++)
    {
        if(str[i] > 96 && str[i] < 123)
            str[i] = str[i] - 32;
    }
}

char* new_charset_base(unsigned int flag) 
{
    size_t base_size = get_base_size(flag);
    char* charset = (char *) malloc((base_size * sizeof(char))+1);
    if(charset == NULL)
    {
        fprintf(stderr, "Err! An error has occurred during charset allocation.\n");
        exit(EXIT_FAILURE);
    }
    charset[0] = '\0';
    if(flag & UPPER_LETTERS)
    {
        strcat(charset, LETTERS);
        to_upper_case(charset);
    }
    if(flag & LOWER_LETTERS)
        strcat(charset, LETTERS);
    if(flag & P_NUMBERS)
        strcat(charset, NUMBERS);
    if(flag & P_SYMBOLS)
        strcat(charset, SYMBOLS);
    return charset;
}

uint64_t nano_since_epoch(void)
{
    long int ns;
    time_t sec;
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    sec = spec.tv_sec;
    ns = spec.tv_nsec;
    return (uint64_t) sec * BILLION + (uint64_t) ns;
}

char* produce_password(char* character_base, unsigned int password_length)
{
    int i;
    int base_len = strlen(character_base);
    char* password = (char*) malloc((password_length * sizeof(char))+1);
    srand(nano_since_epoch());
    if(password == NULL)
    {
        fprintf(stderr, "Err! An error has occurred during password allocation.\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < password_length; i++)
    {
        int base_index = rand() % base_len;
        password[i] = character_base[base_index];
    }
    password[password_length] = '\0';
    return password;
}