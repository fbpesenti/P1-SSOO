#include <stdio.h>
#include "../crms_API/crms_API.h"

int main(int argc, char const *argv[])
{
    if (argc >= 2)
    {
        char* path = argv[1];
        printf("Funcion cr_mount\n");
        cr_mount(path);
        printf("argv: %s \n", argv[1]);
        CrmsFile* crms = cr_open(0, "secret2.txt", 'r');
        if (crms)
        {
                    printf("\ncrms virdir: %u", crms->virtual_dir);
        }
    
        printf("\nFuncion cr_ls_processes\n");
        cr_ls_processes();
        int process_id_files = 0;

        printf("\nFuncion cr_ls_files\n");
        cr_ls_files(process_id_files);

        printf("\nFuncion cr_exists\n");
        int process_id_exists = 0;
        char* file_name_exists = "secret.txt";
        cr_exists(process_id_exists, file_name_exists);

        printf("\nFuncion cr_start_process\n");
        int process_id_start = 202;
        char* process_name_start = "new_process";
        cr_start_process(process_id_start, process_name_start);

        printf("\nFuncion cr_ls_processes\n");
        cr_ls_processes();

        printf("\nFuncion process_id_finish\n");
        int process_id_finish = 202;
        cr_finish_process(process_id_finish);

        printf("\nFuncion cr_ls_processes\n");
        cr_ls_processes();
        printf("\nFuncion crs open\n");
        //char* process_name = "secret.txt";
        //cr_open(0, process_name,'r');
        printf("\nFuncion crs open\n");
        uint8_t my_buffer2[1000];
        cr_read(crms, my_buffer2, 5);
       printf("\npriuena\n");

    }
    return 0;
}
