#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "fs.hpp"

using namespace std;

int help()
{
    cout << endl << "Usage: fs [input files] output_file\nexample: fs input_file1.txt input_file2.txt output_file.txt" << endl;
    return 1;
}

#define COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))

int main(int files_count, char** files)
{
    files++; files_count--;

    unsigned int headers_c = files_count -1;

    fs_node_t nodes[headers_c];
    unsigned off = sizeof(nodes) + sizeof(fs_t);

    for (int i = 0; i < headers_c; ++i)
    {
        FILE* input = fopen(files[i], "r");
        if (!input)
            return printf("file: '%s' not found!", files[i]);

        fseek(input, 0, SEEK_END);
        nodes[i].data = off;
        nodes[i].data_length = ftell(input);
        nodes[i].magic = FS_MAGIC;
        nodes[i].inode = i+1;
        strcpy(nodes[i].name, files[i]);

        off += nodes[i].data_length;
        fclose(input);
    }

    FILE* ostream = fopen(files[headers_c], "w");
    unsigned char* data = (unsigned char*)malloc(off);
    fwrite(&headers_c, sizeof(int), 1, ostream);
    fwrite(nodes, sizeof(fs_node_t), COUNTOF(nodes), ostream);

    for(int i = 0; i < headers_c; i++)
    {
        FILE* input = fopen(files[i], "r");
        unsigned char* buffer = (unsigned char*)malloc(nodes[i].data_length);
        fread(buffer, 1, nodes[i].data_length, input);
        fwrite(buffer, 1, nodes[i].data_length, ostream);
        fclose(input);
        free(buffer);
    }

    fclose(ostream);
    free(data);
    return 0;
}
