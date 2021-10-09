#include "crms_API.h"

// funciones generales
char* MEM_PATH;
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

void cr_finish_process(int process_id){
    
}

// FUNCIONES ARCHIVOS

CrmsFile* cr_open(int process_id, char* file_name, char mode){
  CrmsFile *cr_file = malloc(sizeof(CrmsFile));
  cr_file->read_index = 0;
  FILE* MEM = fopen(MEM_PATH, "r+b");
  fseek(MEM, 0, SEEK_SET);

  if (mode == 'r')
  {
    uint8_t entry_pcb;

    for (uint8_t i = 0; i < 16; i++)
    {
      entry_pcb = i*256;
      uint8_t valypros[2];
      fread(valypros, 1, 2, MEM);
      if (valypros[1] != process_id)
      {
        fseek(MEM, 256, SEEK_CUR);
        continue;
      } else {
        fseek(MEM, 12, SEEK_CUR);
        char* nombre;
        fread(cr_file->validation_byte, 1, 1, MEM);
        fread(nombre, 1, 12, MEM);

      }
      
      
    }
    
  else if (mode == 'r')
  {
    os_strerror(invalid_read_file);
    ret_file->mode = 'r';
    return ret_file;
  }
  else if (mode == 'w' && !os_exists(filename))
  {
    for (int directory_entry = 0; directory_entry < bloque_directory->entry_quantity; directory_entry++)
    {
      if(!strcmp(bloque_directory->entries[directory_entry].filename, filename)){
        ret_file->filename = filename;
        //Si el archivo era el que buscaba, procedo a rellenar sus bloques de datos y retorno
        uint64_t posicion_bloque_indice = 1024 + (bloque_directory->entries[directory_entry].relative_index)*2048 + (mbt->entry_container[partition]->location)*2048; // MBT + Particion + relative
        // Ahora rellenamos el IndexBlock para obtener punteros
        IndexBlock *bloque_index = indexblock_init(disk, posicion_bloque_indice /* REVISAREIS*/);
        ret_file->index_block = bloque_index;
        ret_file->mode = 'w';
        // Ahora rellenamos los DataBlocks
        datablocks_init(disk, ret_file);
        fclose(disk);
        return ret_file;
      }
    }
  }
  else if (mode == 'w')
  { //Acá ya existe
  for (int directory_entry = 0; directory_entry < bloque_directory->entry_quantity; directory_entry++)
    {
      if(!strcmp(bloque_directory->entries[directory_entry].filename, filename)){
        ret_file->filename = filename;
        //Si el archivo era el que buscaba, procedo a rellenar sus bloques de datos y retorno
        uint64_t posicion_bloque_indice = 1024 + (bloque_directory->entries[directory_entry].relative_index)*2048 + (mbt->entry_container[partition]->location)*2048; // MBT + Particion + relative
        // Ahora rellenamos el IndexBlock para obtener punteros
        IndexBlock *bloque_index = indexblock_init(disk, posicion_bloque_indice /* REVISAREIS*/);
        ret_file->index_block = bloque_index;
        // Ahora rellenamos los DataBlocks
        datablocks_init(disk, ret_file);
        ret_file->mode = 'w';
        fclose(disk);
        os_strerror(invalid_write_file);
        return ret_file;
      }
    }
  }
  else
  {
    for (int directory_entry = 0; directory_entry < bloque_directory->entry_quantity; directory_entry++)
    {
      if(!strcmp(bloque_directory->entries[directory_entry].filename, filename)){
        ret_file->filename = filename;
        //Si el archivo era el que buscaba, procedo a rellenar sus bloques de datos y retorno
        uint64_t posicion_bloque_indice = 1024 + (bloque_directory->entries[directory_entry].relative_index)*2048 + (mbt->entry_container[partition]->location)*2048; // MBT + Particion + relative
        // Ahora rellenamos el IndexBlock para obtener punteros
        IndexBlock *bloque_index = indexblock_init(disk, posicion_bloque_indice /* REVISAREIS*/);
        ret_file->index_block = bloque_index;
        // Ahora rellenamos los DataBlocks
        datablocks_init(disk, ret_file);
        fclose(disk);
        os_strerror(invalid_open_mode);
        ret_file->mode = 'r';
        return ret_file;
      }
    }
  }
  return NULL;
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