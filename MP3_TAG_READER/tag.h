#include <stdio.h>
#include "comman.h"
#ifndef TAG_H
#define TAG_H
#define TAG_SIZE 5
typedef struct tag{
    // data of tag file
    char* file_name;
    char* new_data;
    int length_of_new_data;
    int length_of_old_data;
    FILE* fptr;
    char operation;
    char operation_tag[TAG_SIZE];
    char operation_tag_file[TAG_SIZE];

    // temp file
    FILE* temp_fptr;
}TAG;
status edit_tag(TAG* tag);
status do_edit(TAG* tag, char* argv[]);
status print_menu();
status read_and_validation(TAG* tag, char* argv[]);
status read_and_validate_mp3(TAG* tag, char* argv[]);
status move_file_ptr(TAG* tag);
status assign_operation_type(TAG* tag);
operation check_operation_type(char* argv[]);
status copy_data_to_temp(TAG* tag);
status copy_header_data(TAG* tag);
status open_files(TAG* tag);
status get_len(TAG *tag);
status copy_content_to_temp_file(TAG* tag);
status pre_exit(TAG* tag);
#endif
