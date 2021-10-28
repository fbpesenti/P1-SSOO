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
    
        printf("\nFuncion cr_open archivo existente modo lectura\n");
        CrmsFile* crms = cr_open(28, "facebook.png", 'r');
        if (crms){
            uint8_t* buffer = calloc(crms->file_size, 1);
            FILE* archivo = fopen("facebook.png", "w+b");
            int num = cr_read(crms, buffer, crms->file_size);
            // printf("error?\n");
            // printf("se leyeron %i bytes\n", num);
            fwrite(buffer, 1, crms->file_size, archivo);
            fclose(archivo);
            cr_close(crms);
            free(buffer);
        }

        printf("\nFuncion cr_open archivo en proceso inexistentes\n");
        CrmsFile* crms2 = cr_open(10, "facebook.png", 'r');
        if (crms2){
            cr_close(crms2);
        }
        
        printf("\nFuncion cr_open archivo inexistentes\n");
        CrmsFile* crms3 = cr_open(4, "secret.txt", 'r');
        if (crms3){
            cr_close(crms3);
        }

        printf("\nFuncion cr_delete_file\n");
        CrmsFile* crms4 = cr_open(28, "facebook.png", 'r');
        if (crms4){
            printf("Files antes de borrar facebook.png:\n");
            cr_ls_files(28);
            cr_delete_file(crms4);
            cr_close(crms4);
            printf("Files despues de borrar facebook.png\n");
            cr_ls_files(28);
        }

        // printf("\nFuncion cr_ls_processes\n");
        // cr_ls_processes();
        // printf("\nFiles pid: 4\n");
        // cr_ls_files(4);
        // printf("\nFiles pid: 200\n");
        // cr_ls_files(200);
        // printf("\nFiles pid: 27\n");
        // cr_ls_files(27);
        // printf("\nFiles pid: 32\n");
        // cr_ls_files(32);
        // printf("\nFiles pid: 9\n");
        // cr_ls_files(9);
        // printf("\nFiles pid: 28\n");
        // cr_ls_files(28);
        // printf("\nFiles pid: 0\n");
        // cr_ls_files(0);

    }
    return 0;
}