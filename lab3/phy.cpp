#include <cstdio>
#include <unistd.h>
#include <inttypes.h>
#include <cstdint>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef uint64_t uint64;
typedef uint32_t uint32;

#define page_map_file "/proc/self/pagemap"

const uint64 PFN_MASK = ((((uint64)1) << 55) - 1);
const uint64 PFN_PRESENT_FLAG = (((uint64)1) << 63);

unsigned long vir_to_phy(unsigned long vir)
{
    int fd, page_size = getpagesize();
    uint32 vir_page_idx = vir / page_size, pfn_item_offset = vir_page_idx * sizeof(uint64);
    uint64 pfn_item;
    fd = open(page_map_file, O_RDONLY);
    if (fd < 0)
    {
        printf("open %s failed\n", page_map_file);
        return -1;
    }
    if ((off_t)-1 == lseek(fd, pfn_item_offset, SEEK_SET))
    {
        printf("lseek %s failed\n", page_map_file);
        return -1;
    }
    if (sizeof(uint64) != read(fd, &pfn_item, sizeof(uint64)))
    {
        printf("read %s failed\n", page_map_file);
        return -1;
    }
    if (0 == (pfn_item & PFN_PRESENT_FLAG))
    {
        printf("page is not present\n");
        return -1;
    }
    return (pfn_item & PFN_MASK) * page_size + vir % page_size;
}

int main()
{
    uint32 a = 0x12345678;
    printf("vitual address=%p, physical address=0x%x\n", &a, vir_to_phy((unsigned long)&a));
    return 0;
}