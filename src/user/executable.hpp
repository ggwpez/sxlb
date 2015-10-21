#pragma once

#include "../types.hpp"
#include "../fs/vfs.hpp"

int32_t execve(vfs::fs_node_t* start_node, char* path, char* const* argv, char* const* env);
