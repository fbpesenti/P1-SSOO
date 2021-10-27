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
    
        printf("\nFuncion cr_ls_processes\n");
        cr_ls_processes();


        int process_id_files = 400;
        printf("\nFuncion cr_ls_files invalid\n");
        cr_ls_files(process_id_files);

        process_id_files = 0;
        printf("\nFuncion cr_ls_files valid\n");
        cr_ls_files(process_id_files);

        printf("\nFuncion cr_exists\n");
        int process_id_exists = 0;
        char* file_name_exists = "secret.txt";
        char* file_name_exists_not = "no.mp4";
        int exists = cr_exists(process_id_exists, file_name_exists);
        int exists_not = cr_exists(process_id_exists, file_name_exists_not);
        printf("retorno: %i\n", exists);
        printf("retorno de inexitente: %i\n", exists_not);
        

        // printf("\nFuncion cr_start_process\n");
        // int process_id_start = 202;
        // char* process_name_start = "new_process";
        // cr_start_process(process_id_start, process_name_start);
        // // int process_id_start_invalid = 100;
        // // char* process_name_start_invalid = "new_process";
        // // cr_start_process(process_id_start_invalid, process_name_start_invalid);
        // for (size_t i = 0; i < 17; i++)
        // {
        //     /* code */
        // }
        

        // printf("\nFuncion cr_ls_processes\n");
        // cr_ls_processes();

        // printf("\nFuncion process_id_finish\n");
        // int process_id_finish = 202;
        // cr_finish_process(process_id_finish);

        // printf("\nFuncion cr_ls_processes\n");
        // cr_ls_processes();
    }
    return 0;
}