#include <cstdio>
#include <cstdlib>
#include <cstring>


int main(int argc, char *argv[])
{
        void *p = malloc(343);
        if (p)
        {
                free(p);
                p = NULL;
        }
        const char * str = "llllllllllll";
        void * ptr = memcpy(p, str, strlen(str));
        if (!ptr)
        {
                printf("memcpy error\n");
        }

        return 0;
}