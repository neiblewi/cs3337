/*********** superblock.c program ************/

#include   <stdio.h>
#include   <stdlib.h>
#include   <fcntl.h>
#include   <time.h>
#include   <sys/types.h>
#include   <unistd.h>
#include   "ext2_fs.h"


///////////////////////////////////////////////////////////////////
/// print - print line of data
///////////////////////////////////////////////////////////////////

int  print(char* s, u32 x)
{
    printf("%-30s = %8d\n", s, x);
}

///////////////////////////////////////////////////////////////////
/// read_super_block - read and print super block
///////////////////////////////////////////////////////////////////

SUPER* sp;
char    buf[1024];
int     fd, blksize;

int  read_super_block(char* device)
{
    fd = open(device, O_RDONLY);

    if (fd < 0) {
        printf("open %s failed\n", device);
        exit(1);
    }
    lseek(fd, (long)1024 * 1, 0);        // block 1 or offset 1024
    read(fd, buf, 1024);
    sp = (SUPER*)buf;                  // as a super block structure

    // check for EXT2 FS magic number:

    printf("Checking Storage Device:\n");
    printf("%-30s = %8x hex ", "s_magic", sp->s_magic);

    if (sp->s_magic != 0xEF53) {
        printf(" -> STORAGE DEVICE IS NOT an EXT2 FS\n");
        exit(2);
    }
    printf(" -> EXT2 FS OK\n");

    // print key fields in the super block

    printf("Listing Storage Device Super Block:\n");
    print("s_inodes_count", sp->s_inodes_count);
    print("s_blocks_count", sp->s_blocks_count);
    print("s__r_blocks_count", sp->s_r_blocks_count);
    print("s_free_inodes_count", sp->s_free_inodes_count);
    print("s_free_blocks_count", sp->s_free_blocks_count);
    print("s_first_data_blcok", sp->s_first_data_block);
    print("s_log_block_size", sp->s_log_block_size);
    print("s_blocks_per_group", sp->s_blocks_per_group);
    print("s_inodes_per_group", sp->s_inodes_per_group);
    print("s_mnt_count", sp->s_mnt_count);
    print("s_max_mnt_count", sp->s_max_mnt_count);

    printf("%-30s = %8x hex\n", "s_magic", sp->s_magic);

    // print time stamps

    printf("%-30s = %s", "s_mtime", ctime((time_t*)&sp->s_mtime));
    printf("%-30s = %s", "s_wtime", ctime((time_t*)&sp->s_wtime));

    blksize = 1024 * (1 << sp->s_log_block_size);

    printf("%-30s = %8d\n", "block size", blksize);
    printf("%-30s = %8d\n", "inode size", sp->s_inode_size);
}

///////////////////////////////////////////////////////////////////
/// main - main function
///////////////////////////////////////////////////////////////////

char* device = "mydisk";         // default device name

int  main(int argc, char* argv[])
{
    if (argc > 1)
        device = argv[1];
    read_super_block(device);
}