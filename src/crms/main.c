#include <stdio.h>
#include "../crms_API/crms_API.h"

int main(int argc, char const *argv[])
{
    if (argc >= 2)
    {
        const char* path = argv[1];
        cr_mount(path);
        printf("argv: %s \n", argv[1]);
        CrmsFile* crms = cr_open(0, "secret.txt", 'r');
        

    }

    
    return 0;
}
