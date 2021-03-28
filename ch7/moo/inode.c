/*********** inode.c file **********/

#include   <stdio.h>
#include   <stdlib.h>
#include   <fcntl.h>
#include   <time.h>
#include   <sys/types.h>
#include   <unistd.h>
#include   "ext2_fs.h"

#define BLKSIZE 1024

SUPER* sp;
GD* gp;
INODE* ip;
DIR* dp;

char   buf[BLKSIZE];
int    fd, firstdata, inodesize, blksize, iblock;


///////////////////////////////////////////////////////////////////
// get_block() - reads a disk block into a buf[ ]
///////////////////////////////////////////////////////////////////

int  get_block(int fd, int blk, char* buf)
{
    lseek(fd, (long)blk * BLKSIZE, SEEK_SET);
    return  read(fd, buf, BLKSIZE);
}

///////////////////////////////////////////////////////////////////
/// inode() - Display root Inode
///////////////////////////////////////////////////////////////////

int  inode(char* device)
{
    int  i;

    fd = open(device, O_RDONLY);

    if (fd < 0) {
        printf("open failed\n");
        exit(1);
    }
    get_block(fd, 1, buf);              // get superblock
    sp = (SUPER*)buf;
    firstdata = sp->s_first_data_block;
    inodesize = sp->s_inode_size;
    blksize = 1024 * (1 << sp->s_log_block_size);
    printf("first_data_block=%d block_size=%d inodesize=%d\n",
        firstdata, blksize, inodesize);

    get_block(fd, (firstdata + 1), buf);        // get group descriptor
    gp = (GD*)buf;
    printf("bmap_block=%d imap_block=%d inodes_table=%d ",
        gp->bg_block_bitmap, gp->bg_inode_bitmap,
        gp->bg_inode_table, gp->bg_free_blocks_count);

    printf("free_blocks=%d free_inodes=%d\n",
        gp->bg_free_inodes_count, gp->bg_used_dirs_count);

    iblock = gp->bg_inode_table;
    printf("root inode information:\n", iblock);
    printf("---------------------–\n");

    get_block(fd, iblock, buf);
    ip = (INODE*)buf + 1;                // ip point at #2 INODE
    printf("mode=%4x ", ip->i_mode);
    printf("uid=%d gid=%d\n", ip->i_uid, ip->i_gid);
    printf("size=%d\n", ip->i_size);
    printf("ctime=%s", ctime((time_t*)&ip->i_ctime));
    printf("links=%d\n", ip->i_links_count);

    for (i = 0; i < 15; i++) {          // print disk block numbers
        if (ip->i_block[i])               // print non-zero blocks only
            printf("i_block[%d]=%d\n", i, ip->i_block[i]);
    }
}

///////////////////////////////////////////////////////////////////
/// main - main function
///////////////////////////////////////////////////////////////////

char* device = "mydisk";                       // default device

int  main(int argc, char* argv[])
{
    if (argc > 1)
        device = argv[1];
    inode(device);
}