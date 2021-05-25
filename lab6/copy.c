#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

void helper()
{
    printf("\nHELP:\nSample usage of the program:\n    ./copy -h \n    ./copy -m <filename> <new_filename>\n    ./copy <filename> <new_filename>\n");
    printf("    ./copy\nWhere:\n    -h argument - triggers the help prompter\n    no arguments given - triggers the help prompter\n    -m argument - triggers the use of mmap()\n");
    printf("    only filenames given - triggers the copy with the use of read(), write()\n\n");
}

void copy_read_write(int fd_from, int fd_to)
{
    off_t curr_pos = lseek(fd_from, (size_t)0, SEEK_CUR); // pointer at the beginning of the file
    off_t from_size = lseek(fd_from, (size_t)0, SEEK_END); // iterate till the end -> get the buffer
    lseek(fd_from, curr_pos, SEEK_SET); // rewind pointer to the beginning

    char buff[from_size]; // buffer 

    read(fd_from, buff, from_size); // read input file
    write(fd_to, buff, from_size); // write output file

}

void copy_mmap(int fd_from, int fd_to)
{
    off_t curr_pos = lseek(fd_from, (size_t)0, SEEK_CUR); // pointer at the beginning of the file
    off_t from_size = lseek(fd_from, (size_t)0, SEEK_END); // iterate till the end -> get the buffer
    lseek(fd_from, curr_pos, SEEK_SET); // rewind pointer to the beginning

    char *src, *dest;
    src = mmap(NULL, from_size, PROT_READ, MAP_PRIVATE, fd_from, 0);
    dest = mmap(NULL, from_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to, 0);

    ftruncate(fd_to, from_size); // pre-sizing the destination file
    memcpy(dest, src, from_size); // Bus error

    munmap(src, from_size);
    munmap(dest, from_size);
}

int main(int argc, char *argv[])
{
    if(argc == 1){
        helper();
        exit(EXIT_SUCCESS);
    }
    
    int arg, i, mflag = 0;
    opterr = 0;
    
    while ((arg = getopt(argc, argv, "hm")) != -1){
        switch (arg){
            case 'h':
                if(argc > 2){
                    printf("Too many arguments were given\n");
                }
                helper();
                exit(EXIT_SUCCESS);
            case 'm':
                if(argc != 4){
                    printf("Error: ./copy -m needs two more arguments\n");
                    exit(EXIT_FAILURE);
                }
                mflag = 1;
                break;
            case '?':
                printf("Error: Non-option argument\n");
                exit(EXIT_FAILURE);
            default:
                abort();
        }
    }
     
    if(mflag == 0 && argc != 3)
    {
        printf("Error: Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }

    int fd_from = open(argv[optind], O_RDONLY);
    int fd_to = open(argv[optind+1], O_RDWR | O_CREAT, 0666); // 0666 is rw-rw-rw-
    if(fd_from == -1 || fd_to == -1){
        printf("Error: file could not be opened nor created\n");
        exit(EXIT_FAILURE);
    }
    
    if(mflag == 1){
        copy_mmap(fd_from, fd_to);
    }else{
        copy_read_write(fd_from, fd_to);
    }

    close(fd_from);
    close(fd_to);
    exit(EXIT_SUCCESS);
}