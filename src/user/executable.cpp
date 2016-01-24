#include "executable.hpp"
#include "elf.hpp"
#include "../memory/memory.hpp"
#include "../task/task.hpp"

int32_t execve(vfs::fs_node_t* start_node, char* path, char* const* argv, char* const* env)
{
    vfs::fs_node_t* file = vfs::resolve_path(start_node, path);
    NOT_NULL(file);

    LPTR buffer = memory::k_malloc(file->length, 0, nullptr);
    NOT_NULL(buffer);

    vfs::read(file, 0, file->length, buffer);

    elf::status_t st;
    LPTR entry = elf::load_file(buffer, &st);
    memory::k_free(buffer);
    NOT_NULL(entry);

    if (st != elf::status_t::Ok)
        return -1;

    uint32_t argc = 0;
    while (argv && argv[argc]) argc++;

    return task::create(entry, argc, argv, USER_RPL);
}
