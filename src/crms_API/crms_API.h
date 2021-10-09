#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct crmsfile
{
    uint8_t validation_byte; // indica si 
    char* name; // nombre
    unsigned long int file_size; // tamaño del archivo
    unsigned int virtual_dir; // direccion virtual
    char mode; //r o w
    void* VPN;
    void* offset;
    unsigned int read_index; // Posición desde donde leer sus bytes
    unsigned int process_id; //proceso al que pertenece
    uint8_t  pag;
};

typedef struct crmsfile CrmsFile; 

// funciones generales
void cr_mount(char* memory_path);

void cr_ls_processes();

int cr_exists(int process_id, char* file_name);

// funciones procesos
void cr_ls_files(int process_id);

void cr_start_process(int process_id, char* process_name);

// funciones archivos
void cr_finish_process(int process_id);

CrmsFile* cr_open(int process_id, char* file_name, char mode);

int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes);

int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes);

void cr_delete_file(CrmsFile* file_desc);

void cr_close(CrmsFile* file_desc);