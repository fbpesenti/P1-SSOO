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
            if (strcmp(nombre_archivo,file_name)==0 && validez[0] == 1){
                printf("El archivo %s existe\n", nombre_archivo);
                retornar+=1;
            }
        }
        fseek(MEM, 32, SEEK_CUR);
    }
    if (retornar==0){
        printf("Función cr_exists retorno 0\n");
        fclose(MEM);
        return 0;
    }
    else{
        printf("Función cr_exists retorno 1\n");
        fclose(MEM);
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
            uint32_t tamano_archivo[1];
            //char tamano_archivo[4];
            //char direccion_virtual[4];
            uint32_t direccion_virtual[1];
            fread(validez,1,1,MEM);
            fread(nombre_archivo,1,12,MEM);
            fread(tamano_archivo,4,1,MEM);
            fread(direccion_virtual,4,1,MEM);
            if (nombre_archivo!=NULL && id_proceso[0] == process_id && validez[0]==1){
                printf("Archivo: %s con tamaño %u y direccion virtual %u\n", nombre_archivo, bswap_32(tamano_archivo[0]), bswap_32(direccion_virtual[0]));
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

CrmsFile* cr_open(int process_id, char* file_name, char mode){
  CrmsFile *cr_file = malloc(sizeof(CrmsFile));
  cr_file->index = 0; // index siempre parte en 0
  cr_file->process_id = process_id; // guardo proceso padre
  cr_file->mode = mode; // guardo modo
  FILE* MEM = fopen(MEM_PATH, "r+b"); // abro memoria
  fseek(MEM, 0, SEEK_SET); //apunto al inicio de stream
  if (mode == 'r') // si es modo lectura
  {


    for (uint8_t i = 0; i < 16; i++) // itero sobre PCB
    {
      uint8_t valypros[2];           // guarda estado y processo
      fread(valypros, 1, 2, MEM);
      // printf("%d) Estado proceso: %u\n", i, (unsigned)valypros[0]);

      if (valypros[1] == process_id && valypros[0] == 1){
        // printf("proceso: %u\n", (unsigned)valypros[1]);
        fseek(MEM, 12, SEEK_CUR);
        for (uint8_t j = 0; j < 10; j++){ // itero sobre 10 subentradas de archivos.
          char nombre[12];
          uint8_t validation_byte[1];
          fread(validation_byte, 1, 1, MEM);
          if (validation_byte[0] == 1)
          {
            fread(nombre, 1, 12, MEM);
            // printf("nombre archivo: %s\n", nombre);
            if (strcmp(nombre, file_name) == 0)
            {
              cr_file->validation_byte = validation_byte[0];
              cr_file->name = file_name;
              printf("nombre: %s\n", nombre);
              printf("file_name: %s\n", file_name);
              uint32_t file_size[1];
              uint32_t virtual_dir[1];
              fread(file_size, 4, 1, MEM);
              fread(virtual_dir, 4, 1, MEM);
              printf("tamaño de archivo: %u\n", bswap_32(file_size[0]) );
              uint8_t vpn = (uint8_t)(bswap_32(virtual_dir[0])>>23);
              uint32_t offset = bswap_32(virtual_dir[0]) & 0b00000000011111111111111111111111;
              cr_file->virtual_dir = bswap_32(virtual_dir[0]);
              cr_file->VPN = vpn;
              cr_file->offset = offset;
              cr_file->file_size = bswap_32(file_size[0]);
              cr_file->dir_TP =  256*i + 14 + (21*10);
              // printf("dir_tp: %lu\n", dir_tp);
              printf("vpn: %x\n", vpn);
              printf("offset: %x\n", offset);
              break;
            } else {
              fseek(MEM,-12,SEEK_CUR);
            }
            
          }
          fseek(MEM, 20, SEEK_CUR); //muevo stream a posicion inicial de siguiente archivo
        } if (cr_file->validation_byte != 1)
        {
          printf("no existe archivo: %s en proceso %d\n", file_name, process_id);
          fclose(MEM);
          free(cr_file);
          return NULL;
        }
        break;
      } else {
        fseek(MEM, 256-2, SEEK_CUR); //muevo stream a posicion inicial de siguiente entrada PCB
      }
      
    }
  }

  if (mode == 'w')
  {
    if(cr_exists(process_id, file_name)){
      fclose(MEM);
      free(cr_file);
      return NULL;
    } else{
      for (uint8_t i = 0; i < 16; i++) // itero sobre PCB
      {
        uint8_t valypros[2];           // guarda estado y processo
        fread(valypros, 1, 2, MEM);
        // printf("%d) Estado proceso: %u\n", i, (unsigned)valypros[0]);

        if (valypros[1] == process_id && valypros[0] == 1){
          // printf("proceso: %u\n", (unsigned)valypros[1]);
          fseek(MEM, 12, SEEK_CUR);
          uint32_t virtual_dir[1];
          uint32_t virtual_dir_mayor = 0;
          uint32_t file_size_mayor;
          uint8_t last_empty_entry;
          for (uint8_t j = 0; j < 10; j++){ // itero sobre 10 subentradas de archivos.
            char nombre[12];
            uint8_t validation_byte[1];
            fread(validation_byte, 1, 1, MEM);
            if (validation_byte[0] == 1)
            {
              fread(nombre, 1, 12, MEM);
              // printf("nombre archivo: %s\n", nombre);
              uint32_t file_size[1];
              fread(file_size, 4, 1, MEM);
              fread(virtual_dir, 4, 1, MEM);

              //uint8_t vpn_test = (uint8_t)(virtual_dir[0]>>23);
              if (bswap_32(virtual_dir[0]) > virtual_dir_mayor)
              {
                file_size_mayor = bswap_32(file_size[0]);
                virtual_dir_mayor = bswap_32(virtual_dir[0]);
              }
            }else {
              last_empty_entry = j;
              fseek(MEM,-12,SEEK_CUR);
            }
            fseek(MEM, 20, SEEK_CUR);
          } 
          fseek(MEM, (256*i) + 14 + (21*last_empty_entry), SEEK_SET); // mueve stream a la entrada de info del primer archivo vacío
          uint8_t byte_validez[1];
          byte_validez[0] = 1;
          uint32_t dir_vir[1];
          uint32_t file_size[1];
          file_size[0] = 0;
          dir_vir[0] = file_size_mayor + virtual_dir_mayor;
          if ((uint8_t)(dir_vir[0] >> 23) > (uint8_t)32) //no abre si no existe pagina.
          {
            printf("dir_vir a escribir: %u", (uint8_t)(dir_vir[0]) >> 23);
            fclose(MEM);
            free(cr_file);
            return NULL;
          }
          uint8_t vpn = dir_vir[0] >> 23;
          uint32_t offset = dir_vir[0] & 0b00000000011111111111111111111111;
          printf("offset a guardar: %x", offset);
          cr_file->validation_byte = byte_validez[0];
          cr_file->VPN = vpn;
          cr_file->virtual_dir = dir_vir[0];
          cr_file->offset = offset;
          cr_file->file_size = 0;
          cr_file->name = file_name;

          cr_file->dir_TP = 256*i + 14 + (21*10);
          dir_vir[0] = bswap_32(dir_vir[0]);
          fwrite(byte_validez, 1, 1, MEM);
          fwrite(file_name, 1, 12, MEM);
          fwrite(file_size, 4, 1, MEM);
          fwrite(dir_vir, 4, 1, MEM);
          break;
        } else {
          fseek(MEM, 256-2, SEEK_CUR); //muevo stream a posicion inicial de siguiente entrada PCB
        }
        
      }
    }
  }
  fclose(MEM);
  return cr_file;

}

uint32_t next_frame(CrmsFile* file_desc, uint8_t vpn, FILE* MEM){

  uint8_t VPN_actual = (file_desc->virtual_dir + file_desc->index) >> 23;  
  uint32_t position = file_desc->dir_TP + VPN_actual;
  uint32_t offset_actual = (file_desc->virtual_dir + file_desc->index) & 0b00000000011111111111111111111111;

  printf("dir_TP + VPN actual: %x\n", position);
  fseek(MEM, 0, SEEK_SET); 
  fseek(MEM, position , SEEK_CUR);
  uint8_t info_mem[1]; 
  fread(info_mem, 1, 1, MEM);
  printf("info pcb: %x\n",info_mem[0]);
  printf("Sacamos el bit de validez....\n");
  uint8_t validez = info_mem[0] >> 7;
  if (validez == 0){
    uint32_t PFN = 1073741825;
    return PFN;    
  }
  else{
    printf("bit validez: %x\n", validez);
    printf("Obtenemos los 7 bit y agragamos offset...\n");
    uint32_t PFN = ((info_mem[0] & 0b01111111)<<23) | offset_actual;
    printf("pfn: %x\n", info_mem[0] & 0b01111111);
    printf("New PFN: %x\n", PFN);
    return PFN;
  }
  
}

int cr_read(CrmsFile* file_desc, uint8_t* buffer, int n_bytes){
  //printf("\nentre a cr_read\n");
  //printf("vpn: %x\n", file_desc->VPN);
  //printf("dir_TP: %x\n", file_desc->dir_TP);
  int read_now = 0;
  FILE* MEM = fopen(MEM_PATH, "r+b");
  uint8_t VPN_actual = (file_desc->virtual_dir + file_desc->index) >> 23;
  uint32_t offset_actual = (file_desc->virtual_dir + file_desc->index) & 0b00000000011111111111111111111111;

  //printf("offset %u\n",file_desc->offset);
  //printf("offset_actual %u\n", offset_actual);
  //printf("vpn actual: %u\n", VPN_actual);
  uint32_t position = file_desc->dir_TP + VPN_actual;
  //printf("dir_TP + VPN actual: %x\n", position);

  //printf("Nos pocisionmos al principio del archivo .....\n");
  fseek(MEM, 0, SEEK_SET); //nos posicionamos al principio del archivo (PCB)
  //printf("Nos movemos a la entreda correspondiente de tabla PCB.....\n");
  fseek(MEM, position , SEEK_CUR);//nos movemos para llegar a la entrada correspondiente al archivo en la tabla PCB
  printf("pointing to: %lu", ftell(MEM));

  //ahora deberia estar en la entrada correcta
  uint8_t info_mem[1]; 
  fread(info_mem, 1, 1, MEM);//guardar el byte de informacion
  //printf("\ninfo pcb: %x\n",info_mem[0]);
  //printf("\nSacamos el bit de validez....\n");
  uint8_t validez = info_mem[0] >> 7;
  if (validez == 0){
    fclose(MEM);
    return read_now;
    
  }
  else{
  //printf("bit validez: %x\n", validez);
  //printf("\nObtenemos los 7 bit y agragamos offset...\n");

  uint32_t PFN = ((info_mem[0] & 0b01111111)<<23) | offset_actual;
  printf("pfn: %x\n", info_mem[0] & 0b01111111);
  printf("dir_real: %u\n", PFN);
  printf("offset actual: %u", offset_actual);
  printf("\nAhora vamos al FRAME..\n");
  fseek(MEM, 0, SEEK_SET); //nos posicionamos al principio del archivo (PCB)
  fseek(MEM, 4112, SEEK_CUR);//nos movemos   hasta los frame///
  fseek(MEM, PFN, SEEK_CUR);//llegamos al frame correspondiente
  //fseek(MEM, file_desc->index, SEEK_CUR);//nos movemos segun el index:que parte del archivo lei
  

  uint32_t bytes_to_read = 8388608 - offset_actual;
  if (bytes_to_read > n_bytes){
    
    for (int j = 0; j < n_bytes; j++)//itero segun los nbit
    {
      fread((buffer+j), 1, 1, MEM);
      // printf("estoy escribiendo: %i\n", buffer[j]);
      read_now ++;
      bytes_to_read --;
    }
    
    file_desc->index = file_desc->index + n_bytes;
    fclose(MEM);    
    return read_now;
  }
  
  else{//CASO 2: se me acaba el frame
    //printf("se me va a acabar el frame\n");
    while (bytes_to_read < n_bytes){//mientras los bytes que me quedan sean menor a los que debo leer
     
      for (int j = 0; j < bytes_to_read; j++)//itero segun los nbit
      {
        fread((buffer+j), 1, 1, MEM);
        read_now ++;
        n_bytes  = n_bytes -1;
        
      }
      
      uint32_t new = file_desc->index + read_now;
      file_desc->index = new; 
      //file_desc->index = file_desc->index + read_now;//index repocisionado
      uint32_t new_PFN = next_frame(file_desc, file_desc->VPN, MEM);

      if (new_PFN == 1073741825){
        fclose(MEM);
        return n_bytes;
      }
      else{
      //printf("\nAhora vamos a al FRAME..\n");
      fseek(MEM, 0, SEEK_SET); //nos posicionamos al principio del archivo (PCB)
      fseek(MEM, 4112, SEEK_CUR); //nos movemos   hasta los frame///
      fseek(MEM, new_PFN, SEEK_CUR); //llegamos al frame correspondiente
      //fseek(MEM, file_desc->index, SEEK_CUR); //nos movemos segun el index
      //printf("offset actual2: %u\n", offset_actual);
      uint32_t offset_actual = (file_desc->virtual_dir + file_desc->index) & 0b00000000011111111111111111111111;
      //printf("offset actual3: %u\n", offset_actual);
      bytes_to_read = 8388608 - offset_actual;
      
      }
    }
    int index_read_now = read_now;
    if (bytes_to_read > n_bytes){
      for (int j = 0; j < n_bytes; j++)//itero segun los nbit
      {
        fread((buffer+j+index_read_now), 1, 1, MEM);
        read_now ++;
      }
      file_desc->index = file_desc->index + n_bytes;
      fclose(MEM);
      return read_now;
    }
  }
   return 7;
}
}

void cr_delete_file(CrmsFile* file_desc){
  FILE* MEM = fopen(MEM_PATH, "r+b");
  fseek(MEM, 0, SEEK_SET);
  uint32_t last_vir_dir = file_desc->file_size + file_desc->virtual_dir;
  uint8_t last_page = (uint8_t)(last_vir_dir >> 23);

  for (uint8_t i = 0; i <= (last_page - file_desc->VPN); i++)
  {
    fseek(MEM, file_desc->dir_TP + (file_desc->VPN + i), SEEK_SET);
    uint8_t pfn[2];
    fread(pfn, 1, 1, MEM);
    pfn[0] = pfn[0] & 0b01111111;
    fseek(MEM, -1, SEEK_CUR);
    fwrite(pfn, 1, 1, MEM);
    fseek(MEM, 4096+(pfn[0]/8), SEEK_SET);
    uint8_t bitmap_byte[0];
    fread(bitmap_byte, 1, 1, MEM);
    fseek(MEM, -1, SEEK_CUR);
    uint8_t bit = pfn[0] - (pfn[0]/8);
    switch (bit)
    {
    case 0:
      bitmap_byte[0] = bitmap_byte[0] & 0b01111111;
      break;
    case 1:
      bitmap_byte[0] = bitmap_byte[0] & 0b10111111;
      break;
    case 2:
      bitmap_byte[0] = bitmap_byte[0] & 0b11011111;
      break;
    case 3:
      bitmap_byte[0] = bitmap_byte[0] & 0b11101111;
      break;
    case 4:
      bitmap_byte[0] = bitmap_byte[0] & 0b11110111;
      break;
    case 5:
      bitmap_byte[0] = bitmap_byte[0] & 0b11111011;
      break;
    case 6:
      bitmap_byte[0] = bitmap_byte[0] & 0b11111101;
      break;
    case 7:
      bitmap_byte[0] = bitmap_byte[0] & 0b11111110;
      break;
    default:
      break;
    }
    fwrite(bitmap_byte, 1, 1, MEM);
  }
  fseek(MEM, 0, SEEK_SET);
  for (uint8_t i = 0; i < 16; i++) // itero sobre PCB
    {
      uint8_t valypros[2];           // guarda estado y processo
      fread(valypros, 1, 2, MEM);
      // printf("%d) Estado proceso: %u\n", i, (unsigned)valypros[0]);

      if (valypros[1] == file_desc->process_id && valypros[0] == 1){
        // printf("proceso: %u\n", (unsigned)valypros[1]);
        fseek(MEM, 12, SEEK_CUR);
        for (uint8_t j = 0; j < 10; j++){ // itero sobre 10 subentradas de archivos.
          char nombre[12];
          uint8_t validation_byte[2];
          fread(validation_byte, 1, 1, MEM);
          if (validation_byte[0] == 1)
          {
            fread(nombre, 1, 12, MEM);
           // printf("nombre archivo: %s\n", nombre);
            if (strcmp(nombre, file_desc->name) == 0)
            {
              validation_byte[0] = 0;
              fseek(MEM,-13,SEEK_CUR);
              fwrite(validation_byte, 1, 1, MEM);
              file_desc->validation_byte = validation_byte[0];
            } else {
              uint32_t file_size[1];
              uint32_t virtual_dir[2];
              fread(file_size, 4, 1, MEM);
              fread(virtual_dir, 4, 1, MEM);
              virtual_dir[1] = bswap_32(virtual_dir[0]) + bswap_32(file_size[0]); 
              uint8_t vpn_test[2];
              vpn_test[0] = (uint8_t)(bswap_32(virtual_dir[0])>>23);
              vpn_test[1] = (uint8_t)(virtual_dir[1]>>23);
              long pos = ftell(MEM);
              for (uint8_t k = 0; k < 2; k++)
              {
                fseek(MEM, (file_desc->dir_TP)+vpn_test[k], SEEK_SET);
                uint8_t pfn[2];
                fread(pfn, 1, 1, MEM);
                pfn[0] = pfn[0] | 0b10000000;
                fseek(MEM, -1, SEEK_CUR);
                fwrite(pfn, 1, 1, MEM);
                pfn[1] = pfn[0] & 0b01111111;
                uint32_t bitmap_dir = 4096+(pfn[1]/8);
                fseek(MEM, bitmap_dir, SEEK_SET);
                uint8_t bitmap_byte[0];
                fread(bitmap_byte, 1, 1, MEM);
                fseek(MEM, -1, SEEK_CUR);
                uint8_t bit = pfn[1] - (pfn[1]/8);
                switch (bit)
                {
                case 0:
                  bitmap_byte[0] = bitmap_byte[0] | 0b10000000;
                  break;
                case 1:
                  bitmap_byte[0] = bitmap_byte[0] | 0b01000000;
                  break;
                case 2:
                  bitmap_byte[0] = bitmap_byte[0] | 0b00100000;
                  break;
                case 3:
                  bitmap_byte[0] = bitmap_byte[0] | 0b00010000;
                  break;
                case 4:
                  bitmap_byte[0] = bitmap_byte[0] | 0b00001000;
                  break;
                case 5:
                  bitmap_byte[0] = bitmap_byte[0] | 0b00000100;
                  break;
                case 6:
                  bitmap_byte[0] = bitmap_byte[0] | 0b00000010;
                  break;
                case 7:
                  bitmap_byte[0] = bitmap_byte[0] | 0b00000001;
                  break;
                default:
                  break;
                }
                // printf("\nidnice: %u\n", k);
                fwrite(bitmap_byte, 1, 1, MEM);
              }
              fseek(MEM, pos, SEEK_SET);
              fseek(MEM, -8, SEEK_CUR);
              fseek(MEM,-12,SEEK_CUR);
            }
          }
          fseek(MEM, 20, SEEK_CUR); //muevo stream a posicion inicial de siguiente archivo
        }
        break;
      } else {
        fseek(MEM, 256-2, SEEK_CUR); //muevo stream a posicion inicial de siguiente entrada PCB
      }
    }
  fclose(MEM);
}

void cr_close(CrmsFile* file_desc){
    free(file_desc);
}