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

#define MAX_SIZE  20
#define DESC_SIZE 64
#define MAX_CONTENT_SIZE 64

typedef unsigned char BYTE;
typedef unsigned int  DWORD;
typedef size_t QWORD;

typedef struct __attribute__((__packed__)) ArrayEntry
{
    char  desc[DESC_SIZE];
    bool  in_use;
    char  *content;
    QWORD content_size;
} ArrayEntry;

void help()
{
    puts("[e]dit   - edit array entry");
    puts("[c]lean  - clear array entry content");
    puts("[v]iew   - view array entry");
    puts("[h]elp   - display this message");
    puts("[l]eave  - close program");
    fflush(NULL);
}

void banner()
{
    puts(R"(+------------------+   +------------------+   +----------+
| 1.               | + | 2.               | + | -1.     ++
| BBBBBBBBBBBBBBBB | | | AAAAAAAAAAAAAAAAAAAAAA         |
| BBBBBBBBBBBBBBBB | | |  AAAAAAAAAAAAAAAAAAAAAA    +---+
| BBBBBBBBBBBBBBBB | | |   AAAAAAAAAAAAAAAAAAAAAA +-+
| BBBBBBBBBBBBBBBB | | |    AAAAAAAAAAAAAAAAAAAAAA+
| BBBBBBBBBBBBBBBB | | |     AAAAAAAAAAAAAAAAAAAAAA---+
|                  | + |                  | + |       +--+
+------------------+   +------------------+   +----------+
)");
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

int view_entry(ArrayEntry array[], int idx) 
{
    // signed comparison!
    if (idx > MAX_SIZE)
        return -1;
    
    if (array[idx].in_use != true)
        return -2;

    printf("Description: %s\n",   array[idx].desc);
    printf("Content size: %zu\n", array[idx].content_size);
    if (array[idx].content != NULL)
        printf("Content: %s\n",       array[idx].content);
    fflush(NULL);
}

int clean_entry(ArrayEntry array[], int idx) 
{
    // signed comparison!
    if (idx > MAX_SIZE)
        return -1;
    
    if (array[idx].in_use != true)
        return -2;

    free(array[idx].content);

    memset(&array[idx], '\x00', sizeof(ArrayEntry));

    puts("Note deleted successfully!");
    fflush(NULL);
}

int edit_entry(ArrayEntry array[], int idx) 
{
    QWORD content_size = 0;
    // signed comparison!
    if (idx > MAX_SIZE)
        return -1;

    array[idx].in_use = true;

    input("Input entry description: ", array[idx].desc, DESC_SIZE);

    printf("Enter content size: ");
    fflush(NULL);

    int scanf_res = scanf("%zu", &content_size);
    getchar();

    if (content_size > MAX_CONTENT_SIZE || scanf_res == 0)
        return -2;

    array[idx].content_size = content_size;

    input("Input entry content: ", (array[idx].content = (char *)calloc(array[idx].content_size, sizeof(char))), array[idx].content_size);

    return 0;
}

int main(int argc, const char *argv[])
{
    setup();
    banner();
    help();

    ArrayEntry array[MAX_SIZE];
    for (size_t i = 0; i < MAX_SIZE; ++i)
    {
        memset(array[i].desc, '\0', DESC_SIZE);
        array[i].content = NULL;
        array[i].content_size = 0;
        array[i].in_use = false;
    }

    char choice[16];
    DWORD idx = 0;

    while (1)
    {
        printf("> ");
        fflush(NULL);
        fgets(choice, sizeof(choice), stdin);

        if (strncmp(choice, "edit", 4) == 0 || choice[0] == 'e')
        {
            printf("Enter index: ");
            fflush(NULL);
            scanf("%u", &idx);
            getchar();

            int res = edit_entry(array, idx);
            if(res == 0) // translate DWORD to int -> type-confusion
            {
                puts("Array entry edited successfully!");
                fflush(NULL);
            } else if (res == -1)
            {
                puts("Impossible index specified...");
                fflush(NULL);
            } else if (res == -2)
            {
                puts("Impossible content size specified...");
                fflush(NULL);
            }
        } else if (strncmp(choice, "clean", 5) == 0 || choice[0] == 'c')
        {
            printf("Enter index: ");
            fflush(NULL);
            scanf("%u", &idx);
            getchar();
            
            int res = clean_entry(array, idx);
            if(res == 0) // translate DWORD to int -> type-confusion
            {
                puts("Array entry edited successfully!");
                fflush(NULL);
            } else if (res == -1)
            {
                puts("Impossible index specified...");
                fflush(NULL);
            } else if (res == -2)
            {
                puts("Array entry isn't is use!");
                fflush(NULL);
            }
        } else if (strncmp(choice, "view", 4) == 0 || choice[0] == 'v')
        {
            printf("Enter index: ");
            fflush(NULL);
            scanf("%u", &idx);
            getchar();

            int res = view_entry(array, idx);
            if (res == -1)
            {
                puts("Impossible index specified...");
                fflush(NULL);
            } else if (res == -2)
            {
                puts("Array entry isn't is use!");
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

    return 0;
}