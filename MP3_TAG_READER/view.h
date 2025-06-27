#include "comman.h"
#ifndef VIEW_H
#define VIEW_H
#define SIZE_OF_TAG 6
#define DATA_SIZE 100

typedef struct VIEWTAG{
    FILE* fp;
    char* file_name;
    int len_of_data;
    char Tag[SIZE_OF_TAG];
    char data[DATA_SIZE];
}VIEWTAG;
//char *TAGS[]

status read_and_validate_for_view(int argc, char* argv[]);
status open_file_to_view(VIEWTAG* viewTag);
status check_header(VIEWTAG* viewTag);
status print_data(VIEWTAG* viewTag);
status get_len2(VIEWTAG* viewTag);
status view_data(VIEWTAG* viewTag, int argc, char* argv[]);
status help_print();
status read_and_validation_for_help(int argc, char* argv[]);

#endif