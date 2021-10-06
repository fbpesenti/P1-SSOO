#include "crms_API.h"

// funciones generales
void cr_mount(char* memory_path) {
    printf("memory_path: %s \n", memory_path);
    
    MEM_PATH = memory_path;
    
    printf("MEM_PATH: %s \n", MEM_PATH);
}

void cr_ls_processes() {

}

int cr_exists(int process_id, char* file_name){

}

// funciones procesos
void cr_ls_files(int process_id){

}

void cr_start_process(int process_id, char* process_name){

}

// funciones archivos
void cr_finish_process(int process_id){
    
}

CrmsFile* cr_open(int process_id, char* file_name, char mode){
    
}

int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes){
    char* bytes;
    for (int i = 0; i < n_bytes; i++)
    {
        // bytes[i] = buffer[i];
    }
    
}

int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes){
    
}

void cr_delete_file(CrmsFile* file_desc){
    
}

void cr_close(CrmsFile* file_desc){
    
}