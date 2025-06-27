#include <stdio.h>
#include <string.h>
#include "view.h"
#include "tag.h"
int main(int argc, char* argv[])
{
    TAG tag;
    VIEWTAG viewTag;
    if(argc<2)
    {
        puts("\n------------------------------------------------------------------------------------------\n");
        puts("ERROR: ./a.out: INVALID ARGUMENTS");
        puts("USAGE: ");
        puts("To view please pass like: ./a.out -v mp3filename");
        puts("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c \"changing text \" mp3filename");
        puts("To get help pass like: ./a.out --help");
        puts("------------------------------------------------------------------------------------------\n");
        return e_failure;
    }
    switch (check_operation_type(argv))
    {
    case edit:
        printf("You selected edit\n");
        if(do_edit(&tag, argv)==e_sucess)
        {
            puts("INFO: Edit tag successful");
            return e_sucess;
        }
        else
        {
            puts("ERROR: Edit tag failed");
            return e_failure;
        }
    case view:
        if(view_data(&viewTag, argc, argv)==e_sucess)
        {
            return e_sucess;
        }
        else
        {
            puts("ERROR: in view option");
            return e_failure;
        }
    case help:
        if(help_print(argc, argv)==e_sucess)
        {
            return e_sucess;
        }
        else
        {
            puts("ERROR: In help option");
            return e_failure;
        }
    default:
        puts("\n------------------------------------------------------------------------------------------\n");
        puts("ERROR: ./a.out: INVALID ARGUMENTS");
        puts("USAGE: ");
        puts("To view please pass like: ./a.out -v mp3filename");
        puts("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c \"changing text \" mp3filename");
        puts("To get help pass like: ./a.out --help");
        puts("------------------------------------------------------------------------------------------\n");
        return e_failure;
    }
}