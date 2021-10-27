#include <stdio.h>
#include "../crms_API/crms_API.h"

extern enum cr_error;

int main(int argc, char const *argv[])
{
    if (argc >= 2)
    {
        char* path = argv[1];
        printf("Funcion cr_mount\n");
        cr_mount(path);
        printf("Path cr_mount: %s\n", path);
    
        printf("\nFuncion cr_start_process\n");
        for (size_t i = 0; i < 16; i++)
        {
            char* process_name_start = "new_process";
            cr_start_process(i, process_name_start);
        }
        
        printf("\nFuncion cr_ls_processes\n");
        cr_ls_processes();
        
        printf("\nCreando processo con memoria llena\n");
        cr_start_process(16, "nuevo_fake");

        printf("\nFuncion process_id_finish\n");
        for (size_t i = 0; i < 16; i++)
        {
            cr_finish_process(i);
        }
        printf("\nerror porque ya se elimino processo id2\n");
        cr_finish_process(2);

        printf("\nFuncion cr_ls_processes con error\n");
        cr_ls_processes();
    }
    return 0;
}