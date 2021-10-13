#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct crmsfile
{
    uint8_t process_id; //proceso al que pertenece
    uint8_t validation_byte; // indica si entrada es valida
    char* name; // nombre
    uint32_t file_size; // tamaño del archivo
    uint32_t virtual_dir; // direccion virtual
    uint8_t VPN;  //VPN 3 bits no significativos y 5 bits VPN (0b***00000)
    uint32_t offset; // 9 bits no significativos y 23 bits de offset
    char mode; //r o w
    uint32_t index; // Posición dentro del file
    uint32_t  dir_TP; //en cuantas paginas se encuentra el archivo
};
typedef struct crmsfile CrmsFile; 

CrmsFile* crmsFile_init();
void crmsFile_destroy(CrmsFile* crmsfile);
