#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
lastName(char *path)
{ // 返回路径中最后的一个斜杠后的字符串
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  return ++p;
}

void
find(char *path, char *fileName)
{
  char buf[20], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
    case T_FILE: //如果是文件的话，比较文件名看是否是所要找的文件
      if (strcmp(lastName(path), fileName) == 0) {
        printf("%s\n", path);
      }
      break;

    case T_DIR: //是directory，就把当前的路径存到buf中，在后面加一个斜杠并把指针p指向最后
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      // while循环，遍历当前directory下面的每一个directory
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        
        //因为strcmp的参数要用const char *，所以这里新声明了一个变量
        const char *temp = de.name;
        if (strcmp(temp, ".") == 0 || strcmp(temp, "..") == 0) { //忽略.和..，不然会死循环
          continue;
        }
        //把当前directory的名字加到buf后面形成新的路径，然后递归调用find查找
        strcpy(p, temp);
        find(buf, fileName);
      }
      break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3) { //调用find参数个数必须符合要求
    fprintf(2, "Failed to pass two arguments!\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
