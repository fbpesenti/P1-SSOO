#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <byteswap.h>
#include<math.h>
#include "crmsFile.h"


enum cr_error{
  invalid_id,
  invalid_ls_processes,
  invalid_inputs,
  invalid_action,
  invalid_mode,
};

void cr_strerror(enum cr_error error);

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

int cr_read(CrmsFile* file_desc, uint8_t* buffer, int n_bytes);

void cr_delete_file(CrmsFile* file_desc);

void cr_close(CrmsFile* file_desc);