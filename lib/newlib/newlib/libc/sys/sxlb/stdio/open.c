#include "open.h"
#include "../../../../../../posixc/dirent.h"
#include "../../../../../../../src/system/syscall_defs.hpp"

/*int file_ptrs[MAX_FILES] =  { 1,1,1,0,
                              0,0,0,0,
                              0,0,0,0,
                              0,0,0,0 };*/
struct __file_ptr file_ptrs[MAX_FILES] = { { 1, 0 }, { 1, 0 }, { 1, 0 }, { 0, 0 },
                                           { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
                                           { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
                                           { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }};

//this "int" is a short, dont return anything longer than 2 bytes°!!!!
int open(const char *name, int flags, ...)
{
    int file_i = 0;
    while (file_ptrs[file_i].fs_node && file_i < MAX_FILES) file_i++;   //find next free file buffer

    if (file_i +1 >= MAX_FILES)     //MAX_FILES reached
        return -1;

    char* dir_path = NULL,* file_name = name,* last_slash;
    size_t dir_path_l = 0;
    DIR* dir;

    if (strchr(name, '/'))
    {
        //get directory path
        last_slash = strrchr(name, '/') +1;
        dir_path_l = last_slash - name;

        dir_path = (char*)malloc(dir_path_l +1);

        strncpy(dir_path, name, dir_path_l);
        dir_path[dir_path_l] = '\0';

        file_name += dir_path_l;
        dir = opendir(dir_path);
        free(dir_path);
    }
    else
    {
        dir = opendir(".");
    }    

    if (!dir)
        return -1;

    //dirent_t* ent;
    LPTR fs_node;

    SYSCALL_RET2(SYSCNUM_VFS_FIND_DIR, fs_node, dir->fs_node, file_name);
    closedir(dir);

    if (!fs_node)       //is the file in the directory?
    {
        return -1;      //no
    }
    else                //yee, lets open it
    {
        SYSCALL1(SYSCNUM_VFS_OPEN, fs_node);
        file_ptrs[file_i].fs_node = fs_node;
        file_ptrs[file_i].off = 0;
        return file_i;
    }
}