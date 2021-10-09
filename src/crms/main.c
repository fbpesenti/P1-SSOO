#include <stdio.h>
#include "../crms_API/crms_API.h"

int main(int argc, char const *argv[])
{
    if (argc >= 2)
    {
        char* path = argv[1];
        cr_mount(path);
        cr_ls_processes();
    }
    return 0;
}
