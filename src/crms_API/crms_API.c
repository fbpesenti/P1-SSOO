#include "crms_API.h"
#include <string.h>

char* MEM_PATH;

// funciones generales
char* MEM_PATH;
void cr_mount(char* memory_path) {
    MEM_PATH = memory_path;
    printf("MEM_PATH: %s \n", MEM_PATH);
}

void cr_ls_processes() {
    FILE* MEM = fopen(MEM_PATH, "r+b");
    fseek(MEM, 0, SEEK_SET);
    //uint8_t entry_pcb;
    for (uint8_t i=0; i < 16; i+=1){
        //entry_pcb=i*256;
        uint8_t estado[1];
        uint8_t id_proceso[1];
        char nombre[12];
        //printf("Puntero file: %ld\n",ftell(MEM));
        fread(estado,1,1,MEM);
        fread(id_proceso,1,1,MEM);
        fread(nombre,1,12,MEM);
        //printf("Estado: %02x\n", 12);
        //printf("Estado: %hhx\n", estado[0]);
        //printf("id_proceso: %d\n",id_proceso[0]);
        //printf("Nombre proceso: %s\n",nombre);
        if (estado[0]==1){
            //if (strcmp(nombre, "")){
            //    printf("Proceso con id %d y nombre es vacio\n", id_proceso[0]);
            //}
            //else {
            //    printf("Proceso con id %d y nombre %s esta en ejecución\n", id_proceso[0], nombre);
            //}
            printf("Proceso con id %d y nombre %s esta en ejecución\n", id_proceso[0], nombre);
        }
        fseek(MEM, 242, SEEK_CUR);
    }

    fclose(MEM);
}
int cr_exists(int process_id, char* file_name){
    FILE* MEM = fopen(MEM_PATH, "r+b");
    fseek(MEM, 0, SEEK_SET);
    int retornar = 0;
    for (uint8_t i=0; i < 16; i+=1){
        uint8_t estado[1];
        uint8_t id_proceso[1];
        char nombre[12];
        fread(estado,1,1,MEM);
        fread(id_proceso,1,1,MEM);
        fread(nombre,1,12,MEM);
        for (uint8_t i=0; i<10; i+=1){
            uint8_t validez[1];
            char nombre_archivo[12];
            char tamano_archivo[4];
            char direccion_virtual[4];
            fread(validez,1,1,MEM);
            fread(nombre_archivo,1,12,MEM);
            fread(tamano_archivo,1,4,MEM);
            fread(direccion_virtual,1,4,MEM);
            if (strcmp(nombre_archivo,file_name)==0){
                printf("El archivo %s existe\n", nombre_archivo);
                retornar+=1;
            }
        }
        fseek(MEM, 32, SEEK_CUR);
    }
    if (retornar==0){
        printf("Función cr_exists retorno 0\n");
        return 0;
    }
    else{
        printf("Función cr_exists retorno 1\n");
        return 1;
    }

    fclose(MEM);
}

// funciones procesos
void cr_ls_files(int process_id){
    FILE* MEM = fopen(MEM_PATH, "r+b");
    fseek(MEM, 0, SEEK_SET);
    //uint8_t entry_pcb;
    for (uint8_t i=0; i < 16; i+=1){
        //entry_pcb=i*256;
        uint8_t estado[1];
        uint8_t id_proceso[1];
        char nombre[12];
        //printf("Puntero file: %ld\n",ftell(MEM));
        fread(estado,1,1,MEM);
        fread(id_proceso,1,1,MEM);
        fread(nombre,1,12,MEM);
        //printf("nombre proceso %s\n", nombre);
        //printf("Puntero file: %ld\n",ftell(MEM));
        if (id_proceso[0] == process_id && estado[0]==1){
            printf("Mostrando los archivos del proceso con id %d y nombre %s\n", id_proceso[0], nombre);
        }
        for (uint8_t i=0; i<10; i+=1){
            uint8_t validez[1];
            //printf("validez: %hhx\n", validez[0]);
            char nombre_archivo[12];
            uint8_t tamano_archivo[4];
            //char tamano_archivo[4];
            //char direccion_virtual[4];
            uint8_t direccion_virtual[4];
            fread(validez,1,1,MEM);
            fread(nombre_archivo,1,12,MEM);
            fread(tamano_archivo,1,4,MEM);
            fread(direccion_virtual,1,4,MEM);
            if (nombre_archivo!=NULL && id_proceso[0] == process_id && validez[0]==1){
                printf("Archivo: %s con tamaño %d y direccion virtual %d\n", nombre_archivo, tamano_archivo[0], direccion_virtual[0]);
            }

        }
        //256 - 14 - 210 = 32 para moverse la cantidad de bytes para estar en el siguiente proceso
        fseek(MEM, 32, SEEK_CUR);
    }

    fclose(MEM);
}

void cr_start_process(int process_id, char* process_name){
    FILE* MEM = fopen(MEM_PATH, "r+b");
    fseek(MEM, 0, SEEK_SET);
    //uint8_t entry_pcb;
    int entro = 0;
    for (uint8_t i=0; i < 16; i+=1){
        //entry_pcb=i*256;
        uint8_t estado[1];
        uint8_t id_proceso[1];
        char nombre[12];
        //printf("Puntero file antes de leer: %ld\n",ftell(MEM));
        fread(estado,1,1,MEM);
        fread(id_proceso,1,1,MEM);
        fread(nombre,1,12,MEM);
        if (estado[0]==0 && id_proceso[0]==0 && entro==0 && strcmp(nombre,"")==0){
            fseek(MEM, -14, SEEK_CUR);
            estado[0]=1;
            id_proceso[0]=process_id;
            //nombre=process_name;
            //printf("hay un espacio disponible\n");
            //printf("Puntero file antes de escribir: %ld\n",ftell(MEM));
            fwrite(estado,1,1,MEM);
            fwrite(id_proceso,1,1,MEM);
            fwrite(process_name,1,12,MEM);
            entro+=1;
            //printf("Puntero file despues de escribir: %ld\n",ftell(MEM));
        }
        fseek(MEM, 242, SEEK_CUR);
    }

    fclose(MEM);
}

void cr_finish_process(int process_id){
    FILE* MEM = fopen(MEM_PATH, "r+b");
    fseek(MEM, 0, SEEK_SET);
    for (uint8_t i=0; i < 16; i+=1){
        //printf("Puntero file: %ld\n",ftell(MEM));
        uint8_t estado[1];
        uint8_t id_proceso[1];
        char nombre[12];
        fread(estado,1,1,MEM);
        fread(id_proceso,1,1,MEM);
        fread(nombre,1,12,MEM);
        //printf("nombre %s\n", nombre);
        int entro = 0;
        if (id_proceso[0]==process_id && estado[0]==1){
            printf("Proceso con id %d y nombre %s terminando\n", id_proceso[0], nombre);
            entro+=1;
            fseek(MEM, -14, SEEK_CUR);
            estado[0]=0;
            id_proceso[0]=0;
            fwrite(estado,1,1,MEM);
            fwrite(id_proceso,1,1,MEM);
            fwrite("",1,12,MEM);
            for (uint8_t i=0; i<10; i+=1){
                //printf("Puntero file: %ld\n",ftell(MEM));
                uint8_t validez[1];
                char nombre_archivo[12];
                uint8_t tamano_archivo[4];
                uint8_t direccion_virtual[4];
                fread(validez,1,1,MEM);
                fread(nombre_archivo,1,12,MEM);
                fread(tamano_archivo,1,4,MEM);
                fread(direccion_virtual,1,4,MEM);
                //if (strcmp(nombre_archivo,"")!=0 && id_proceso[0] == process_id && validez[0]==1){
                if (id_proceso[0] == process_id && validez[0]==1){
                    printf("Borrando archivo: %s con tamaño %d y direccion virtual %d\n", nombre_archivo, tamano_archivo[0], direccion_virtual[0]);
                }
                fseek(MEM,-21,SEEK_CUR);
                fwrite("",1,1,MEM);
                fwrite("",1,12,MEM);
                fwrite("",1,4,MEM);
                fwrite("",1,4,MEM);
            }
            //printf("Puntero file: %ld\n",ftell(MEM));
        }
        if (entro==1){
            fseek(MEM, 32, SEEK_CUR);
        }
        else {
            fseek(MEM,242,SEEK_CUR);
        }
    }

    fclose(MEM);
}

// FUNCIONES ARCHIVOS

// CrmsFile* cr_open(int process_id, char* file_name, char mode){
//   CrmsFile *cr_file = malloc(sizeof(CrmsFile));
//   cr_file->read_index = 0;
//   FILE* MEM = fopen(MEM_PATH, "r+b");
//   fseek(MEM, 0, SEEK_SET);

//   if (mode == 'r')
//   {
//     uint8_t entry_pcb;
//     for (uint8_t i = 0; i < 16; i++)
//     {
//       entry_pcb = i*256;
//       uint8_t valypros[2];
//       fread(valypros, 1, 2, MEM);
//       if (valypros[1] != process_id)
//       {
//         fseek(MEM, 256, SEEK_CUR);
//         continue;
//       } else {
//         fseek(MEM, 12, SEEK_CUR);
//         char* nombre;
//         fread(cr_file->validation_byte, 1, 1, MEM);
//         fread(nombre, 1, 12, MEM);

//       }
      
      
//     }
    
//   else if (mode == 'r')
//   {
//     os_strerror(invalid_read_file);
//     ret_file->mode = 'r';
//     return ret_file;
//   }
//   else if (mode == 'w' && !os_exists(filename))
//   {
//     for (int directory_entry = 0; directory_entry < bloque_directory->entry_quantity; directory_entry++)
//     {
//       if(!strcmp(bloque_directory->entries[directory_entry].filename, filename)){
//         ret_file->filename = filename;
//         //Si el archivo era el que buscaba, procedo a rellenar sus bloques de datos y retorno
//         uint64_t posicion_bloque_indice = 1024 + (bloque_directory->entries[directory_entry].relative_index)*2048 + (mbt->entry_container[partition]->location)*2048; // MBT + Particion + relative
//         // Ahora rellenamos el IndexBlock para obtener punteros
//         IndexBlock bloque_index = indexblock_init(disk, posicion_bloque_indice / REVISAREIS*/);
//         ret_file->index_block = bloque_index;
//         ret_file->mode = 'w';
//         // Ahora rellenamos los DataBlocks
//         datablocks_init(disk, ret_file);
//         fclose(disk);
//         return ret_file;
//       }
//     }
//   }
//   else if (mode == 'w')
//   { //Acá ya existe
//   for (int directory_entry = 0; directory_entry < bloque_directory->entry_quantity; directory_entry++)
//     {
//       if(!strcmp(bloque_directory->entries[directory_entry].filename, filename)){
//         ret_file->filename = filename;
//         //Si el archivo era el que buscaba, procedo a rellenar sus bloques de datos y retorno
//         uint64_t posicion_bloque_indice = 1024 + (bloque_directory->entries[directory_entry].relative_index)*2048 + (mbt->entry_container[partition]->location)*2048; // MBT + Particion + relative
//         // Ahora rellenamos el IndexBlock para obtener punteros
//         IndexBlock bloque_index = indexblock_init(disk, posicion_bloque_indice / REVISAREIS*/);
//         ret_file->index_block = bloque_index;
//         // Ahora rellenamos los DataBlocks
//         datablocks_init(disk, ret_file);
//         ret_file->mode = 'w';
//         fclose(disk);
//         os_strerror(invalid_write_file);
//         return ret_file;
//       }
//     }
//   }
//   else
//   {
//     for (int directory_entry = 0; directory_entry < bloque_directory->entry_quantity; directory_entry++)
//     {
//       if(!strcmp(bloque_directory->entries[directory_entry].filename, filename)){
//         ret_file->filename = filename;
//         //Si el archivo era el que buscaba, procedo a rellenar sus bloques de datos y retorno
//         uint64_t posicion_bloque_indice = 1024 + (bloque_directory->entries[directory_entry].relative_index)*2048 + (mbt->entry_container[partition]->location)*2048; // MBT + Particion + relative
//         // Ahora rellenamos el IndexBlock para obtener punteros
//         IndexBlock bloque_index = indexblock_init(disk, posicion_bloque_indice / REVISAREIS*/);
//         ret_file->index_block = bloque_index;
//         // Ahora rellenamos los DataBlocks
//         datablocks_init(disk, ret_file);
//         fclose(disk);
//         os_strerror(invalid_open_mode);
//         ret_file->mode = 'r';
//         return ret_file;
//       }
//     }
//   }
//   return NULL;
// }

//int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes){
    //char* bytes;
    //for (int i = 0; i < n_bytes; i++)
    //{
        // bytes[i] = buffer[i];
    //}
    
//}

//int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes){
    
//}

//void cr_delete_file(CrmsFile* file_desc){
    
//}

//void cr_close(CrmsFile* file_desc){
    
//}