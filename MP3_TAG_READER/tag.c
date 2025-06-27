#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "comman.h"
#include "tag.h"
status open_files(TAG* tag)
{
    tag->fptr = fopen(tag->file_name, "r+");
    tag->temp_fptr = fopen("temp.mp3", "w+");
    if(feof(tag->fptr)|| feof(tag->temp_fptr))
        return e_failure;
    return e_sucess;
}
status do_edit(TAG* tag, char* argv[])
{
    puts("INFO: Validation started");
    if(read_and_validation(tag, argv)==e_sucess)
    {
        puts("INFO: Read and validation Suceessful");
    }
    else
    {
        puts("ERROR: Validation Failed!!");
        return e_failure;
    }
    if(open_files(tag)==e_sucess)
    {
        printf("INFO: opened %s file\n", tag->file_name);
        printf("INFO: opened %s file\n", "temp");
    }
    else
    {
        puts("ERROR: failed opening files");
        return e_failure;
    }
    if(read_and_validate_mp3(tag, argv)==e_sucess)
    {
        puts("Read and validaton of mp3 successfull");
    }
    else
    {
        puts("Read and validaton of mp3 Failed");
        return e_failure;
    }
    if(copy_header_data(tag)==e_sucess)
    {
        puts("INFO: Coping header is completed");
    }
    else
    {
        puts("Coping Header Failed!!");
        return e_failure;
    }
    if(move_file_ptr(tag)==e_sucess)
    {
        pre_exit(tag);
        return e_sucess;
    }
    else
    {
        return e_failure;
    }
    
    
    
}

status read_and_validation(TAG* tag, char* argv[])
{
    if(argv[2] != NULL )
    {
        sscanf(argv[2], "-%c", &tag->operation);
        switch(tag->operation)
        {
            case 'a':
            strcpy(tag->operation_tag,"TPE1"); 
                puts("INFO: You selected to edit Artist name");
                break;
            case 't':
            strcpy(tag->operation_tag,"TIT2");
                puts("INFO: You selected to edit Title/song name");
                break;
            case 'A':
            strcpy(tag->operation_tag,"TALB"); //
                puts("INFO: You selected to edit Album");
                break;
            case 'y':
            strcpy(tag->operation_tag,"TYER"); 
            puts("INFO: You selected to edit Year");
                break;
            case 'g':
            strcpy(tag->operation_tag,"TCON");// 
                puts("INFO: You selected to edit content type");
                break;
            case 'c':
            strcpy(tag->operation_tag,"COMM"); 
                puts("INFO: You selected to edit comment");
                break;
            default:
                puts("ERROR: Invalid option!");
                puts("INFO: Select below options");
                print_menu();
                return e_failure;
        }
        if(argv[3]!=NULL)
        {
            tag->length_of_new_data = strlen(argv[3]);
            tag->new_data = (char*)malloc(tag->length_of_new_data+1);
            strcpy(tag->new_data, argv[3]);
            printf("%s\n", tag->new_data);
            if(argv[4]!=NULL)
            {
                if((strcmp(strchr(argv[4], '.'), ".mp3")==0))
                {
                    tag->file_name = argv[4];
                    return e_sucess;
                }
                else
                {
                    puts("ERROR: Invalid file name");
                    return e_failure;
                }
            }
            else
            {
                puts("ERROR: Audio file is not mentioned!");
                return e_failure;
            }
        }
        else
        {
            puts("ERROR: Data to be edit is not provided");
            return e_failure;
        }

    }
    else
    {
        puts("INFO: Select below option");
        print_menu();
    }
    
}
operation check_operation_type(char* argv[])
{
    if(strcmp(argv[1], "-e")==0)
    {
        return edit;
    }
    else if(strcmp(argv[1], "-v")==0)
    {
        return view;
    }
    else if(strcmp(argv[1], "--help")==0)
    {
        return help;
    }
}
status get_len(TAG *tag)
{
    char buff_len[5];
    fread(buff_len, sizeof(char), 4, tag->fptr);
    tag->length_of_old_data=0;
    for(int i=3; i>0; i--)
    {
        tag->length_of_old_data |= (int)buff_len[i];
    }
}
status read_and_validate_mp3(TAG* tag, char* argv[])
{
    fscanf(tag->fptr, "%3s", tag->operation_tag_file);
    if(strcmp(tag->operation_tag_file, "ID3")!=0)
    {
        puts("ERROR: ID3 not found");
        return e_failure;
    }
    unsigned short int x;
    fread(&x, sizeof(short int), 1, tag->fptr);
    if(x != 3)
    {
        puts("ERROR: Version is not matching");
        return e_failure;
    }
    return e_sucess;
}

status move_file_ptr(TAG* tag)
{
   fscanf(tag->fptr, "%4s", tag->operation_tag_file);

   if(strcmp(tag->operation_tag_file, tag->operation_tag)==0)
   {
        if(edit_tag(tag)==e_sucess)
        {
            return e_sucess;
        }
        else
        {
            return e_failure;
        }
   }
   else
   {
    copy_content_to_temp_file(tag);
    move_file_ptr(tag);
   }

}
status copy_content_to_temp_file(TAG* tag)
{
    fwrite(tag->operation_tag_file, sizeof(char), 4, tag->temp_fptr);
    get_len(tag);
    char*ptr = (char*)&tag->length_of_old_data;
    for(int i=3; i>=0; i--)
    {
        fwrite(ptr+i, sizeof(char), 1, tag->temp_fptr);
    }
    ptr = malloc((tag->length_of_old_data+2)); // to get the 2 flags 
    fread(ptr, sizeof(char), (tag->length_of_old_data+2),tag->fptr); 
    fwrite(ptr, sizeof(char), (tag->length_of_old_data+2), tag->temp_fptr);
    free(ptr);
    return e_sucess;
}
status edit_tag(TAG* tag)
{
    fwrite(tag->operation_tag_file, sizeof(char), 4, tag->temp_fptr);
    tag->length_of_new_data+=1; // because of the null character at starting

    char* ptr = (char*)&tag->length_of_new_data;
    for(int i=3; i>=0; i--)
    {
        fwrite(ptr+i, sizeof(char), 1, tag->temp_fptr);
    }
    fseek(tag->fptr, 4, SEEK_CUR);
    for(int i=0; i<2; i++)
    {
        char ch = fgetc(tag->fptr);
        fputc(ch, tag->temp_fptr);
    }
    fseek(tag->fptr, -6, SEEK_CUR);
    fputc('\0', tag->temp_fptr);
    tag->length_of_new_data-=1;
    fwrite(tag->new_data, sizeof(char), tag->length_of_new_data, tag->temp_fptr);
    if(copy_data_to_temp(tag)==e_sucess)
    {
        puts("INFO: DATA copied successfully");
        return e_sucess;
    }
    else
    {
        puts("ERROR: data copied failed");
        return e_failure;
    }

}
status copy_data_to_temp(TAG* tag)
{
    char* buff = malloc(10240);
    get_len(tag);
    tag->length_of_old_data+=2;
    fseek(tag->fptr, tag->length_of_old_data, SEEK_CUR);
    int bytes_read;
    while((bytes_read= fread(buff, sizeof(char), sizeof(buff), tag->fptr))>0)
    {
        fwrite(buff, sizeof(char), bytes_read, tag->temp_fptr);
    }
    free(buff);
    return e_sucess;
}
status copy_header_data(TAG* tag)
{
    rewind(tag->fptr);
    char buff[10];
    fread(buff, sizeof(char), 10, tag->fptr);
    fwrite(buff, sizeof(char), 10, tag->temp_fptr);
    return e_sucess;
}
status print_menu()
{
    puts("\n-----------------------");
    puts("Title/song name (-t)");
    puts("Artist name (-a)");
    puts("Album (-A)");
    puts("Year (-y)");
    puts("content type (-g)");
    puts("Composer (-c)");
    puts("-----------------------");
}
status pre_exit(TAG* tag)
{
    // removing orignal file
    remove(tag->file_name);
    // renaming temp file with the orignal file name
    rename("temp.mp3", tag->file_name);
    free(tag->new_data);
    fclose(tag->fptr);
    fclose(tag->temp_fptr);
}
