#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "simplefs.h"

int main(int argc, char **argv)
{
    int ret;
    int fd1, fd2, fd; 
    int i;
    char c; 
    char buffer[1024];
    char buffer2[8] = {50, 50, 50, 50, 50, 50, 50, 50};
    int size;
    char vdiskname[200]; 

    printf ("started\n");

    if (argc != 2) {
        printf ("usage: app  <vdiskname>\n");
        exit(0);
    }
    strcpy (vdiskname, argv[1]); 
    
    ret = sfs_mount (vdiskname);
    if (ret != 0) {
        printf ("could not mount \n");
        exit (1);
    }

    printf ("creating files\n"); 
    printf("%d\n", sfs_create ("file1.bin"));
    printf("%d\n", sfs_create ("file2.bin"));
    // printf("%d\n", sfs_create ("file3.bin"));

    fd1 = sfs_open ("file1.bin", MODE_APPEND);
    fd2 = sfs_open ("file2.bin", MODE_APPEND);
    // fd = sfs_open("file3.bin", MODE_READ);

    // printf("File descriptor - 1: %d\n", fd1);
    // printf("File descriptor - 2: %d\n", fd2);
    // printf("File descriptor - 3: %d\n", fd);

    for (i = 0; i < 5000; ++i) {
        buffer[0] = (char) 65;
        buffer[1] = (char) 66;
        buffer[2] = (char) 67;
        buffer[3] = (char) 68;
        sfs_append(fd2, (void *) buffer, 4);
    }

    for (i = 0; i < 10000; ++i) {
        buffer[0] = (char) 69;
        buffer[1] = (char) 70;
        buffer[2] = (char) 71;
        sfs_append (fd1, (void *) buffer, 3);
    }

    for (i = 0; i < 5000; ++i) {
        buffer[0] = (char) 65;
        buffer[1] = (char) 66;
        buffer[2] = (char) 67;
        buffer[3] = (char) 68;
        sfs_append(fd2, (void *) buffer, 4);
    }
    
    printf("Size of file - 1: %d\n", sfs_getsize(fd1));
    printf("Size of file - 2: %d\n", sfs_getsize(fd2));

    sfs_close(fd1);
    sfs_close(fd2);

    sfs_delete("file1.bin");

    // fd = sfs_open("file3.bin", MODE_APPEND);
    // for (i = 0; i < 10000; ++i) {
    //     memcpy (buffer, buffer2, 8); // just to show memcpy
    //     sfs_append(fd, (void *) buffer, 8);
    // }
    // sfs_close (fd);

    // fd = sfs_open("file3.bin", MODE_READ);
    // size = sfs_getsize (fd);
    // for (i = 0; i < size; ++i) {
    //     sfs_read (fd, (void *) buffer, 1);
    //     c = (char) buffer[0];
    //     c = c + 1;
    // }
    // sfs_close (fd);
    // ret = sfs_umount();
}
