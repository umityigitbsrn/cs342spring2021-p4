#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "simplefs.h"


// Global Variables =======================================
int vdisk_fd; // Global virtual disk file descriptor. Global within the library.
              // Will be assigned with the vsfs_mount call.
              // Any function in this file can use this.
              // Applications will not use  this directly. 
// ========================================================



// read block k from disk (virtual disk) into buffer block.
// size of the block is BLOCKSIZE.
// space for block must be allocated outside of this function.
// block numbers start from 0 in the virtual disk. 
int read_block (void *block, int k)
{
    int n;
    int offset;

    offset = k * BLOCKSIZE;
    lseek(vdisk_fd, (off_t) offset, SEEK_SET);
    n = read (vdisk_fd, block, BLOCKSIZE);
    if (n != BLOCKSIZE) {
	printf ("read error\n");
	return -1;
    }
    return (0); 
}

// write block k into the virtual disk. 
int write_block (void *block, int k)
{
    int n;
    int offset;

    offset = k * BLOCKSIZE;
    lseek(vdisk_fd, (off_t) offset, SEEK_SET);
    n = write (vdisk_fd, block, BLOCKSIZE);
    if (n != BLOCKSIZE) {
	printf ("write error\n");
	return (-1);
    }
    return 0; 
}

void set_bit(void *bitmap, int nblock, unsigned int bit_index){
    void *curr = bitmap + 4096 * nblock;
    int char_index = bit_index / 8;
    int index = 7 - (bit_index % 8);
    curr = curr + char_index * sizeof(char);
    *(char *) curr |= 1UL << index;
}

int get_bit(void *bitmap, int nblock, unsigned int bit_index){
    void *curr = bitmap + 4096 * nblock;
    int char_index = bit_index / 8;
    int index = 7 - (bit_index % 8);
    curr = curr + char_index * sizeof(char);
    if ((*(char *) curr & 1UL << index) == 0)
        return 0;
    else
        return 1;
}

void clear_bit(void *bitmap, int nblock, unsigned int bit_index){
    void *curr = bitmap + 4096 * nblock;
    int char_index = bit_index / 8;
    int index = 7 - (bit_index % 8);
    curr = curr + char_index * sizeof(char);
    *(char *) curr &= ~(1UL << index);
}

/**********************************************************************
   The following functions are to be called by applications directly. 
***********************************************************************/

// this function is partially implemented.
int create_format_vdisk (char *vdiskname, unsigned int m)
{
    char command[1000];
    int size;
    int num = 1;
    int count;
    int stat;
    size  = num << m;
    count = size / BLOCKSIZE;
    // printf ("%d %d %d", m, size, count);
    sprintf (command, "dd if=/dev/zero of=%s bs=%d count=%d",
             vdiskname, BLOCKSIZE, count);
    //printf ("executing command = %s\n", command);
    system (command);

    // now write the code to format the disk below.
    // .. your code...
    void *bitmap_buff = calloc(BLOCKSIZE, BLOCKSIZE);

    //superblock
    vdisk_fd = open(vdiskname, O_RDWR);
    void *block = calloc(BLOCKSIZE, BLOCKSIZE);
    *(int *) block = size;
    stat = write_block(block, 0);
    free(block);
    if (stat == -1)
        return -1;

    //bitmaps
    unsigned int i;
    for (i = 0; i < 13; ++i)
        set_bit(bitmap_buff, 0, i);

    stat = write_block(bitmap_buff, 1);
    if (stat == -1)
        return -1;

    //root directory
    block = calloc(BLOCKSIZE / 128, BLOCKSIZE);
    void *curr = block;
    for (i = 0; i < 128; ++i){
        *(int *) (curr + 110) = -1;    
        curr += 128;
    }

    for (i = 0; i < 4; ++i){
        stat = write_block(block, (i + 5));
        if (stat == -1)
            return -1;
    }

    free(block);

    fsync(vdisk_fd);
    close(vdisk_fd);
    free(bitmap_buff);
    return (0); 
}


// already implemented
int sfs_mount (char *vdiskname)
{
    // simply open the Linux file vdiskname and in this
    // way make it ready to be used for other operations.
    // vdisk_fd is global; hence other function can use it. 
    vdisk_fd = open(vdiskname, O_RDWR); 
    return(0);
}


// already implemented
int sfs_umount ()
{
    fsync (vdisk_fd); // copy everything in memory to disk
    close (vdisk_fd);
    return (0); 
}


int sfs_create(char *filename)
{
    return (0);
}


int sfs_open(char *file, int mode)
{
    return (0); 
}

int sfs_close(int fd){
    return (0); 
}

int sfs_getsize (int  fd)
{
    return (0); 
}

int sfs_read(int fd, void *buf, int n){
    return (0); 
}


int sfs_append(int fd, void *buf, int n)
{
    return (0); 
}

int sfs_delete(char *filename)
{
    return (0); 
}

