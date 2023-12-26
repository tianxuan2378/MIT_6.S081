#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void findFile(char *path, char *fileName) {
    //buf: A buffer to hold the path names. p: a pointer for string manipulation
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    //fstat is used to get information about the file (in this case, the directory)
    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    //reads each entry in the directory.
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        strcpy(buf, path);
        p = buf + strlen(buf); //now p points to the end of the current path stored in buf
        *p++ = '/'; // append a "/" 
        //Skip empty entries
        if(de.inum == 0)
            continue;
        //copies the name of the directory entry into buf at the position pointed to by p, forming the full path
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_FILE && strcmp(de.name, fileName) == 0) {
            printf("%s\n", buf);
        } else if (st.type == T_DIR) {
            if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
                findFile(buf, fileName);
        }
    }
    close(fd);
}


int main(int argc, char *argv[]) {
    if(argc < 3){
        fprintf(2, "Usage: find <path> <filename>\n");
        exit(1);
    }
    findFile(argv[1], argv[2]);
    exit(0);
}