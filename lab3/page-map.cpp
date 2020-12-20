#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstring>

using namespace std;

typedef uint64_t uint64;
typedef uint32_t uint32;

#define PAGEMAP_ENTRY 8
#define get_bit(X, Y) ((X & ((uint64)1 << Y)) >> Y)
#define get_pfn(X) (X & 0x7FFFFFFFFFFFFF)

const int __endian_bit = 1;
#define is_bigendian() ((*(char *)&__endian_bit) == 0)

int pid;
uint32 va;
uint64 read_val, file_offset, page_size = getpagesize();
char path_buf[0x100] = {};
FILE *f;
char *end;

int read_pagemap(char *path_buf, uint32 va);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        exit(-1);
    }
    pid = getpid();
    if (argc < 3 || !strcmp(argv[1], "self"))
    {
        sscanf(argv[1], "%x", &va);
        sprintf(path_buf, "/proc/self/pagemap");
    }
    else
    {
        sscanf(argv[1], "%d", &pid);
        sscanf(argv[2], "%x", &va);
        sprintf(path_buf, "/proc/%u/pagemap", pid);
    }
    printf("pid=%d\n", pid);
    return read_pagemap(path_buf, va);
}

int read_pagemap(char path_buf[], uint32 va)
{
    printf("Big-Endian %d\n", is_bigendian());
    f = fopen(path_buf, "rb");
    if (!f)
    {
        printf("Error! Cannot open %s\n", path_buf);
        return -1;
    }
    //Shifting by virt-addr-offset number of bytes
    //and multiplying by the size of an address (the size of an entry in pagemap file)
    file_offset = va / page_size * PAGEMAP_ENTRY;
    printf("VA=0x%x Page-Size=%ld, Entry-Size=%d\n", va, page_size, PAGEMAP_ENTRY);
    printf("Reading %s at 0x%lx\n", path_buf, (uint64)file_offset);
    if (fseek(f, file_offset, SEEK_SET))
    {
        perror("Failed to do fseek!");
        return -1;
    }
    unsigned char c_buf[PAGEMAP_ENTRY];
    for (int i = 0; i < PAGEMAP_ENTRY; ++i)
    {
        int c = getc(f);
        if (c == EOF)
        {
            printf("\nReached end of the file\n");
            return 0;
        }
        if (is_bigendian())
            c_buf[i] = c;
        else
            c_buf[PAGEMAP_ENTRY - i - 1] = c;
        printf("[%d]0x%x ", i, c);
    }
    for (int i = 0; i < PAGEMAP_ENTRY; ++i)
    {
        read_val = (read_val << 8) + c_buf[i];
    }
    printf("\nResult: 0x%lx\n", (uint64)read_val);
    if (get_bit(read_val, 63))
    {
        uint64 pfn = get_pfn(read_val);
        printf("PFN: 0x%lx (0x%lx)\n", pfn, pfn * page_size + va % page_size);
    }
    else
        printf("Page not present\n");
    if (get_bit(read_val, 62))
        printf("Page swapped\n");
    fclose(f);
    return 0;
}