#include <stdio.h>
#include <dirent.h>

int main() {
    const char *path = "/root/linuxx";  // 当前目录
    DIR *dir = opendir(path);
    
    if (dir == NULL) {
        perror("打开目录失败");
        return 1;
    }
    
    // 读取目录内容
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("文件名: %s\n", entry->d_name);
    }
    
    // 关闭目录
    closedir(dir);
    return 0;
}
