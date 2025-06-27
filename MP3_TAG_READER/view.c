#include <stdio.h>
#include <string.h>
#include "comman.h"
#include "view.h"
status view_data(VIEWTAG* viewTag, int argc, char* argv[])
{
    if(read_and_validate_for_view(argc, argv)==e_sucess)
    {
        viewTag->file_name = argv[2];
        printf("Read and validation is succesfull\n");
    }
    else
    {
        printf("\nRead and validation is Failed\n");
        return e_failure;
    }
    if(open_file_to_view(viewTag)==e_sucess)
    {
        printf("INFO: opened %s file\n", viewTag->file_name);
    }
    else
    {
        printf("ERROR: opening %s file\n", viewTag->file_name);
        return e_failure;
    }
    if(check_header(viewTag)==e_sucess)
    {
        printf("Check header successfull\n");
    }
    else
    {
        printf("Check header Failed\n");
    }
    if(print_data(viewTag)==e_sucess)
    {
        return e_sucess;
    }

}
status read_and_validate_for_view(int argc, char* argv[])
{
    if(argc == 3)
    {
        if(strcmp(strchr(argv[2], '.'), ".mp3") != 0)
        {
            printf("File is not \".mp3\"\n");
            return e_failure;
        }
        return e_sucess;
        
    }
    else 
    {
        printf("Invalid syntax\n");
        printf("\t./a.out <-v> <file_name.mp3>");
        return e_failure;
    }
}
status check_header(VIEWTAG* viewTag)
{
    fscanf(viewTag->fp, "%3s", viewTag->Tag);
    if(strcmp(viewTag->Tag, "ID3")!=0)
    {
        printf("ID3 not found\n");
        return e_failure;
    }
    unsigned short int x;
    fread(&x, sizeof(short int), 1, viewTag->fp);
    if(x != 3)
    {
        printf("Version is not matching\n");
        return e_failure;
    }
    return e_sucess;
}
status open_file_to_view(VIEWTAG* viewTag)
{
    viewTag->fp = fopen(viewTag->file_name, "r");
    if(viewTag->fp == NULL)
    {
        printf("ERROR: opening %s file\n", viewTag->file_name);
        return e_failure;
    }
    return e_sucess;
}
status print_data(VIEWTAG* viewTag)
{
    putchar('\n');
    printf("----------------------------------------------------------------------------------------------------------------\n");// 4*20=80+3
    printf("                                           MP3 TAG READER                                           \n");
    printf("----------------------------------------------------------------------------------------------------------------\n");// 4*20=80+3
    // skeeping the 10 bytes
    fseek(viewTag->fp, 10, SEEK_SET);
    for(int i=0; i<6; i++)
    {
        fscanf(viewTag->fp, "%4s", viewTag->Tag);
        printf("\t%s\t\t\t:\t", viewTag->Tag);
        get_len2(viewTag);
        fseek(viewTag->fp, 3, SEEK_CUR);
        viewTag->len_of_data-=1;
        fread(viewTag->data, sizeof(char), viewTag->len_of_data, viewTag->fp);
        printf("\t\t");
        for(int i=0; i<viewTag->len_of_data ; i++)
        {
            if(viewTag->data[i]=='\0')
            {
                putchar('.');
            }
            else
            {
                putchar(viewTag->data[i]);
            }
        }
        putchar('\n');
    }
    printf("----------------------------------------------------------------------------------------------------------------\n");// 4*20=80+3
    return e_sucess;
}
status get_len2(VIEWTAG* viewTag)
{
    char buff_len[4];
    fread(buff_len, sizeof(char), 4, viewTag->fp);
    viewTag->len_of_data = 0;
    for(int i=3; i>0; i--)
    {
        viewTag->len_of_data |= (int)buff_len[i];
    }
}
/////// printing help menu////////////
status help_print(int argc, char* argv[])
{
    if(read_and_validation_for_help(argc,argv)==e_failure)
    {
        return e_failure;
    }
    putchar('\n');
    puts("------------------------------------------HELP MENU------------------------------------------");
    puts("1. -v -> To view mp3 file content");
    puts("2. -e -> To edit mp3 file content");
    puts("         2.1. -t -> to edit song title");
    puts("         2.2. -a -> to edit artist name");
    puts("         2.1. -A -> to edit album name");
    puts("         2.1. -y -> to edit year");
    puts("         2.1. -g -> to edit content");
    puts("         2.1. -c -> to edit comment");
    puts("-----------------------------------------------------------------------------------------------");
    putchar('\n');
}
status read_and_validation_for_help(int argc, char* argv[])
{
    if(argc!=2)
    {
        puts("Error: invalid syntax for help");
        puts("INFO: ./a.out --help");
        return e_failure;
    }
    return e_sucess;
}