#include "executable.hpp"
#include "elf.hpp"
#include "../memory/memory.hpp"
#include "../task/task.hpp"

int32_t execve(vfs::fs_node_t* start_node, char* path, char* const* argv, char* const* env)
{
    vfs::fs_node_t* file = vfs::resolve_path(start_node, path);

    LPTR buffer = memory::k_malloc(file->length, 0, nullptr);
    vfs::read(file, 0, file->length, buffer);

    elf::status_t st;
    LPTR entry = elf::load_file(buffer, &st);
    memory::k_free(buffer);

    if (st != elf::status_t::Ok)
        return -1;

    uint32_t argc = 0;
    while (argv[argc]) argc++;

    return task::create(entry, argc, argv, USER_RPL);
}

/*
 * char* buffer = memory::k_malloc(dat->length, 0, nullptr);
    vfs::read(dat, 0, dat->length, buffer);

    elf::elf_status_t st;
    LPTR* con = elf::load_file(buffer, &st);
    memory::k_free(buffer);

    if (st != elf::elf_status_t::Ok)
        return false;
    //sprintf_s(start_path, sizeof(start_path), "/initrd/%s", file_name);
    return task::create(con, 1, &start_path, 0);
 * */
