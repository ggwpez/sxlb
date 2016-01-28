//###
int read(int file, void *buf, size_t len)
{
    int n = pread(file, buf, len, file_ptrs[file].off);
    file_ptrs[file].off += n;
    return n;
}

ssize_t pread(int file, void *buf, size_t count, off_t off)
{
    if (!count) return 0;

    if (file == 0)          //stdin
    {
        int c = __getchar();
        *(char*)buf = c;

        return 1;
    }
    else
    {
        int s;
        SYSCALL_RET4(SYSCNUM_VFS_READ, s, file_ptrs[file].fs_node, off, count, buf);
        return s;
    }
    
    return 0;
}