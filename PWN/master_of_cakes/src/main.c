/*
* @Author: m4drat
* @Date:   2019-10-21 21:31:53
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#define MAX_WEIGHT 128
#define MAX_CAKES 10
#define NAME_LEN_MAX 256

typedef unsigned char BYTE;
typedef unsigned int  DWORD;

typedef struct Cake
{
    int   weight;
    char* name;
    bool  is_magic_cake;
} Cake;

void flag()
{
    system("/bin/cat /service/flag.txt");
}

int find_empty_cake(Cake* cakes[], size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (cakes[i] == NULL)
            return i;
    }
    return -1;
}

void help()
{
    puts("[b]ake   - bake a cake");
    puts("[s]ell   - sell a cake");
    puts("[e]dit   - edit cake");
    puts("[v]iew   - view all cakes");
    puts("[h]elp   - display this message");
    puts("[l]eave  - close program");
    fflush(NULL);
}

void banner()
{
    puts(R"(              (        (
             ( )      ( )          (
      (       Y        Y          ( )
     ( )     |\"|      |\"|          Y
      Y      | |      | |         |\"|
     |\"|     | |.-----| |---.___  | |
     | |  .--| |,~~~~~| |~~~,,,,'-| |
     | |-,,~~'-'___   '-'       ~~| |._
    .| |~       // ___            '-',,'.
   /,'-'     <_// // _  __             ~,\
  / ;     ,-,     \\_> <<_______________;_)
  | ;    {(_)} _,      . |================|
  | '-._ ~~,,,           | ,,             |
  |     '-.__ ~~~~~~~~~~~|________________|   
  |\         `'----------|
  | '=._                 |     Cake
  :     '=.__            |      Shop
   \         `'==========|
    '-._                 |
        '-.__            |
             `'----------|)");
    fflush(NULL);
}

void handler()
{
    puts("Time is out!");
    fflush(NULL);
    exit(0);
}

void setup()
{
    setbuf(stdin, 0LL);
	setbuf(stdout, 0LL);
	setbuf(stderr, 0LL);
	alarm(40);
	signal(SIGALRM, (__sighandler_t)handler);
}

int input(char *msg, char *buf, size_t size)
{
    printf("%s", msg);
    fflush(stdout);

    int cnt = read(0, buf, size);

    if (cnt == -1)
    {
        return -1;
    } else 
    {
        buf[cnt - 1] = 0;
    }
    return cnt;
}

void main_loop()
{
    help();

    Cake* cakes[MAX_CAKES];
    for (int i = 0; i < MAX_CAKES; ++i)
        cakes[i] = NULL;

    char choice[8];
    char *tmpbuf = (char *)malloc(NAME_LEN_MAX);

    DWORD cake_idx_usr = -1;

    while (1)
    {
        printf("> ");
        fflush(NULL);
        fgets(choice, sizeof(choice), stdin);

        if (strncmp(choice, "bake", 4) == 0 || choice[0] == 'b')
        {
            int cake_idx = find_empty_cake(cakes, MAX_CAKES);
            cakes[cake_idx] = (Cake*)calloc(sizeof(Cake), sizeof(char));
            #if DEBUG == 1
                printf("Cake at: %p\n", cakes[cake_idx]);
            #endif
            cakes[cake_idx]->is_magic_cake = false;

            int name_len = input("Enter cake name: ", tmpbuf, NAME_LEN_MAX);
            if (name_len == -1)
            {
                puts("Error, while performing read operation");
                fflush(NULL);
                exit(-1);
            }
            srand(time(NULL));
            cakes[cake_idx]->weight = rand() % MAX_WEIGHT;

            cakes[cake_idx]->name = (char*)calloc(name_len, sizeof(char));
            #if DEBUG == 1
                printf("Cake name at: %p\n", cakes[cake_idx]->name);
            #endif
            memcpy(cakes[cake_idx]->name, tmpbuf, name_len);

            printf("The cake is baked! It's placed on %d shelf\n", cake_idx);
            fflush(NULL);
        } else if (strncmp(choice, "sell", 4) == 0 || choice[0] == 's')
        {
            printf("Enter the number of shelf, where cake is placed: ");
            fflush(NULL);
            scanf("%u", &cake_idx_usr);
            getchar();

            if (cake_idx_usr < MAX_CAKES)
            {
                if (cakes[cake_idx_usr] != NULL)
                {
                    if (cakes[cake_idx_usr]->is_magic_cake)
                    {
                        puts("Wow, this cake is so well baked, so buyer want to reward you with the flag!");
                        fflush(NULL);
                        flag();
                        puts("Now you can start baking cakes in a huge house, at the ocean coast");
                        fflush(NULL);
                    }
                    free(cakes[cake_idx_usr]->name);
                    free(cakes[cake_idx_usr]);
                    puts("Cake selled successfully");
                    fflush(NULL);
                } else {
                    puts("The cake isn't baked yet.");
                    fflush(NULL);
                }
            } else 
            {
                puts("You don't have that many shelves!");
                fflush(NULL);
            }
        } else if (strncmp(choice, "edit", 4) == 0 || choice[0] == 'e')
        {
            printf("Enter the number of shelf, where cake is placed: ");
            fflush(NULL);
            scanf("%u", &cake_idx_usr);
            getchar();

            if (cake_idx_usr < MAX_CAKES)
            {
                if (cakes[cake_idx_usr] != NULL)
                {
                    int name_len = input("Enter new cake name: ", tmpbuf, NAME_LEN_MAX);
                    if (name_len == -1)
                    {
                        puts("Error, while performing read operation");
                        fflush(NULL);
                        exit(-1);
                    }
                    srand(time(NULL));
                    cakes[cake_idx_usr]->weight = rand() % MAX_WEIGHT;

                    cakes[cake_idx_usr]->name = (char*)calloc(name_len, sizeof(char));
                    #if DEBUG == 1
                        printf("Cake name at: %p\n", cakes[cake_idx_usr]->name);
                    #endif
                    memcpy(cakes[cake_idx_usr]->name, tmpbuf, name_len);

                    printf("The cake is re-baked! It's placed on %d shelf\n", cake_idx_usr);
                    fflush(NULL);
                } else {
                    puts("The cake isn't baked yet.");
                    fflush(NULL);
                }
            } else 
            {
                puts("You don't have that many shelves!");
                fflush(NULL);
            }
        } else if (strncmp(choice, "view", 4) == 0 || choice[0] == 'v')
        {
            printf("Enter the number of shelf, where cake is placed: ");
            fflush(NULL);
            scanf("%u", &cake_idx_usr);
            getchar();

            if (cake_idx_usr < MAX_CAKES)
            {
                if (cakes[cake_idx_usr] != NULL)
                {
                    printf("Cake name:   %s\n", cakes[cake_idx_usr]->name);
                    printf("Cake weight: %d\n", cakes[cake_idx_usr]->weight);
                } else {
                    printf("Cake isn't baked yet...\n");
                    fflush(NULL);
                }
            } else {
                puts("The cake isn't baked yet.");
                fflush(NULL);
            }
        } else if (strncmp(choice, "help", 4) == 0 || choice[0] == 'h')
        {
            help();
        } else if (strncmp(choice, "leave", 5) == 0 || choice[0] == 'l')
        {
            puts("Goodbye!");
            fflush(NULL);
            exit(0);
        } else
        {
            puts("Impossible option!");
            fflush(NULL);
        }
    }
}

int main(int argc, const char *argv[])
{
    setup();
    banner();

    main_loop();

    return 0;
}