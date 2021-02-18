#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include "dirList.h"
/*
**
**      my_ls
**
**
**      input:ß
**          char* flags, a string of the flags to use with ls.
**          my_ls only supports -a, -t, or both.
**
**
**      OUTPUT:
**          same as ls :)
**
**/
typedef struct flagsObj
{
    u_int8_t flag_count;
    u_int8_t max_flag_count;
    u_int8_t a;
    u_int8_t t;
} flagsObj;

typedef struct dirNodeObj
{
    int time;
    char* name;

} dirNodeObj;
// returns 0 if a incorrect flag is used, 1 otherwise
void parseFlags(flagsObj* flags, char* flag_str)
{
    while (*flag_str != '\n' && *flag_str != '-' && *flag_str != '\0' && flag_str)
    {
        if (*flag_str == 'a')
        {
            flags->a = 1;
            flags->flag_count++;
        }
        else if (*flag_str == 't')
        {
            flags->t = 1;
            flags->flag_count++;
        }
        else
        {
            fprintf(stderr, "flag %c is not supported :(\n", *flag_str);
            exit(0);
        }
        flag_str++;
    }
}
void parseInput(flagsObj* flags, char* flags_str, dirList dL, dirList fL)
{
    char* token = strtok(flags_str, " ");
    while (token != NULL)
    {
        if (*token == '-')
        {
            token++;
            parseFlags(flags, token);
        }
        else
        {
            struct stat info; 
            if (lstat(token, &info) == 0)
            {
                if (S_ISDIR(info.st_mode))
                    addDir(dL, info.st_mtim.tv_sec, info.st_mtim.tv_nsec, token);
                else
                    addDir(fL, info.st_mtim.tv_sec, info.st_mtim.tv_nsec, token);
                
            }
            else
            {
                fprintf(stderr, "ls: cannot access '%s': %s\n", token, strerror(errno));
            }
        }
        token = strtok(NULL, " ");
    }
}
void handleDir(char* directory_name, flagsObj flags)
{
    dirList directory = newDirList();
    DIR* dirp;
    if ((dirp = opendir(directory_name)) == NULL)
    {
        fprintf(stderr, "Couldn't open %s: %s\n", directory_name, strerror(errno));
        return;
    }
    struct dirent* dp;
    char file_path[255];
    //strcat(file_path, "/");
    //adding each directory to a linked list of directory objects that is sorted as the items are added
    while ((dp = readdir(dirp)))
    {
        if (!flags.a && dp->d_name[0] == '.')
        {

        }
        else
        {
            if (flags.t)
            {
                //time flag is set
                struct stat info; 
                strcpy(file_path, dp->d_name);
                if (lstat(file_path, &info) == 0)
                {
                    addDir(directory, info.st_mtim.tv_sec, info.st_mtim.tv_nsec, dp->d_name);
                }
                else
                {
                    //printf("directory: %s\n", file_path);
                    fprintf(stderr, "ERROR with lstat in handleDir: %s\n", strerror(errno));
                }
            }
            else
            {
                addDir(directory, 0, 0, dp->d_name);
            }
        }
    }
    setTFlag(directory, flags.t);
    insertionSort(directory);
    printList(directory);
    dp = NULL;
    closedir(dirp);
    deleteDirList(directory);
    directory = NULL;
}

void my_ls(char* args)
{
    flagsObj flags = {0, 2, 0, 0};
    dirList inputDirs = newDirList();
    dirList inputFiles = newDirList();
    if (strlen(args) > 1)
        parseInput(&flags, args, inputDirs, inputFiles);
    if (getLength(inputDirs) > 0)
    {
        setTFlag(inputDirs, flags.t);
        insertionSort(inputDirs);
    }
    if (getLength(inputFiles) > 0)
    {
        setTFlag(inputFiles, flags.t);
        insertionSort(inputFiles);
    }
    //printf("length of input dirs %d, and length of input Files %d\n", getLength(inputDirs), getLength(inputFiles));
    if (getLength(inputDirs) == 0 && getLength(inputFiles) == 0)
        handleDir(".", flags);
    else if (getLength(inputDirs) == 1 && getLength(inputFiles) == 0)
        handleDir(getDirObjName(inputDirs, 0), flags);
    else
    {
        for (int i = 0; i < getLength(inputFiles); i++)
        {
            write(1, getDirObjName(inputFiles, i), strlen(getDirObjName(inputFiles, i)));
        }
        if (getLength(inputFiles) > 0)
        {
            write(1, "\n", 1);
            if(getLength(inputDirs) > 0)
                write(1, "\n", 1);
        }
        char dir[90];
        for (int i = 0; i < getLength(inputDirs); i++)
        {
            strcpy(dir, getDirObjName(inputDirs, i));
            write(1, dir, strlen(dir));
            write(1, ":\n", 2);
            handleDir(getDirObjName(inputDirs, i), flags);
            write(1, "\n", 1);
            if (i < getLength(inputDirs) - 1)
                write(1, "\n", 1);
        }
    }
    deleteDirList(inputDirs);
    deleteDirList(inputFiles);
    inputFiles = NULL;
    inputDirs = NULL;
    //printf("input: %s\n", inputFiles);
}

int main(int argc, char* argv[])
{
    char args[255] = {0};
    for (int i = 1; i < argc; i++)
    {
        strcat(args, argv[i]);
        strcat(args, " ");
    }
    //printf("strings %s\n", args);
    my_ls(args);
}
