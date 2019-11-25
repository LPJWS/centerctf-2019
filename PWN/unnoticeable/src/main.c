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

// #define BITS 64 
#define BITS 64

#define MAX_NOTES 10
#define NAME_LEN 32
#define CONTENT_LEN 256

#if BITS == 32
    #define ALIGMENT 8 
#else
    #define ALIGMENT 16 
#endif

typedef unsigned int a16_ui32 __attribute__ ((aligned (ALIGMENT)));
typedef unsigned char BYTE;
typedef unsigned int  DWORD;

typedef struct __attribute__((__packed__)) Note
{
    int deleted;
    char *content;
    int content_size;
    char name[NAME_LEN];
} Note;

void read_flag()
{
    system("/bin/cat /service/flag.txt");
}

// return first empty note index, or -1 
int find_empty_note(Note *notes, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (notes[i].deleted == true)
            return i;
    }
    return -1;
}

void help()
{
    puts("[c]reate - create note");
    puts("[e]dit   - edit node");
    puts("[d]elete - delete note");
    puts("[v]iew   - view note");
    puts("[h]elp   - display this message");
    puts("[l]eave  - close program");
    fflush(NULL);
}

void banner()
{
    puts(R"(       .--.                   .---.
   .---|__|           .-.     |~~~|
.--|===|--|_          |_|     |~~~|--.
|  |===|  |'\     .---!~|  .--|   |--|
|%%|   |  |.'\    |===| |--|%%|   |  |
|%%|   |  |\.'\   |   | |__|  |   |  |
|  |   |  | \  \  |===| |==|  |   |  |
|  |   |__|  \.'\ |   |_|__|  |~~~|__|
|  |===|--|   \.'\|===|~|--|%%|~~~|--|
^--^---'--^    `-'`---^-^--^--^---'--')");
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

    Note notes[MAX_NOTES];
    for (size_t i = 0; i < MAX_NOTES; ++i)
    {
        notes[i].deleted = true;
        notes[i].content = NULL;
        notes[i].content_size = 0;
        memset(notes[i].name, '\0', NAME_LEN);
    }

    DWORD node_idx = -1;
    DWORD usr_idx  = -1;
    char choice[8];

    char *tmpbuf = (char *)malloc(CONTENT_LEN);

    while (1)
    {
        printf("> ");
        fflush(NULL);
        fgets(choice, sizeof(choice), stdin);

        if (strncmp(choice, "create", 6) == 0 || choice[0] == 'c')
        {
            node_idx = find_empty_note(notes, MAX_NOTES);

            if (node_idx != -1 && notes[node_idx].deleted == true)
            {
                // mark, that note in use now
                notes[node_idx].deleted = false;

                if (input("Enter note name: ", notes[node_idx].name, NAME_LEN) == -1)
                {
                    puts("Error, while performing read operation");
                    fflush(NULL);
                    exit(-1);
                }

                notes[node_idx].content_size = input("Enter note content: ", tmpbuf, CONTENT_LEN);
                if (notes[node_idx].content_size == -1)
                {
                    puts("Error, while performing read operation");
                    fflush(NULL);
                    exit(-1);
                }

                notes[node_idx].content = (char *)malloc(notes[node_idx].content_size + 1);
                notes[node_idx].content_size -= 1;
                memcpy(notes[node_idx].content, tmpbuf, notes[node_idx].content_size);

                printf("Note created successfully at index: %d!\n", node_idx);
                fflush(NULL);
            } else 
            {
                puts("All space is used. Delete something!");
                fflush(NULL);
            }
        } else if (strncmp(choice, "edit", 4) == 0 || choice[0] == 'e')
        {
            printf("Enter note index: ");
            fflush(NULL);
            scanf("%u", &usr_idx);
            getchar();

            if (usr_idx < MAX_NOTES) 
            {
                if (notes[usr_idx].deleted == false) 
                {
                    if (input("Enter new note name: ", notes[usr_idx].name, NAME_LEN + 1) == -1) // Oooooops, misstype...
                    {
                        puts("Error, while performing read operation");
                        fflush(NULL);
                        exit(-1);
                    }

                    notes[usr_idx].content_size = input("Enter new note content: ", tmpbuf, CONTENT_LEN);
                    if (notes[usr_idx].content_size == -1)
                    {
                        puts("Error, while performing read operation");
                        fflush(NULL);
                        exit(-1);
                    }

                    notes[usr_idx].content_size -= 1;
                    notes[usr_idx].content = (char *)malloc(notes[usr_idx].content_size);
                    memcpy(notes[usr_idx].content, tmpbuf, notes[usr_idx].content_size);

                    printf("Note edited successfully at index: %d!\n", usr_idx);
                    fflush(NULL);
                } else 
                {
                    puts("Note isn't in use!");
                    fflush(NULL);
                }
            } else 
            {
                puts("Impossible index!");
                fflush(NULL);
            }
        } else if (strncmp(choice, "delete", 6) == 0 || choice[0] == 'd')
        {
            printf("Enter note index: ");
            fflush(NULL);
            scanf("%u", &usr_idx);
            getchar();

            if (usr_idx < MAX_NOTES) 
            {
                if (notes[usr_idx].deleted == false) 
                {
                    free(notes[usr_idx].content);
                    notes[usr_idx].deleted = true;

                    memset(notes[usr_idx].name, '\0', NAME_LEN);

                    puts("Note deleted successfully");
                    fflush(NULL);
                } else 
                {
                    puts("Note isn't in use!");
                    fflush(NULL);
                }
            } else 
            {
                puts("Impossible index!");
                fflush(NULL);
            }
        } else if (strncmp(choice, "view", 4) == 0 || choice[0] == 'v')
        {
            printf("Enter note index: ");
            fflush(NULL);
            scanf("%u", &usr_idx);
            getchar();

            if (usr_idx < MAX_NOTES) 
            {
                if (notes[usr_idx].deleted == false) 
                {
                    printf("Note name: %s\n", notes[usr_idx].name);
                    printf("Note content: ");
                    fflush(NULL);
                    fwrite(notes[usr_idx].content, sizeof(char), notes[usr_idx].content_size, stdout);
                    putchar('\n');
                } else 
                {
                    puts("Note isn't in use!");
                    fflush(NULL);
                }
            } else 
            {
                puts("Impossible index!");
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
            // just to trigger stack-lifting, based on saved rbp rewrite 
            puts("Impossible option!");
            puts("Goodbye!");
            fflush(NULL);
            break;
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