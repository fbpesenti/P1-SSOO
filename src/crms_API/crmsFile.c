#include "crmsFile.h"

void crmsFile_print(CrmsFile* crmsfile){
    printf("\nname: %s\n",crmsfile->name);
    printf("mode: %c\n",crmsfile->mode);
    printf("process_id: %u\n",crmsfile->process_id);
    printf("validez archivo: %02x\n",crmsfile->validation_byte);
    printf("Tamaño del archivo: %u\n",crmsfile->file_size);
    printf("Direccion virtual: %u\n",crmsfile->virtual_dir);
    printf("VPN: %u\n",crmsfile->VPN);
    printf("Offset: %u\n",crmsfile->offset);
    printf("Index: %u\n",crmsfile->index);
    printf("Direccion tabla de paginas: %u\n\n",crmsfile->dir_TP);
}
