#include <stdio.h>
#include "../crms_API/crms_API.h"

int main(int argc, char const *argv[])
{
    if (argc >= 2)
    {
        const char* path = argv[1];
        cr_mount(path);
        printf("direccion es: %s \n", MEM_PATH);
        printf("argv: %s \n", argv[1]);

    }


    printf("direccion es: prueba\n");
    
    
    return 0;
}
