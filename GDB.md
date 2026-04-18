# GDB

## 1形成调试文件

```shell
gcc -g main.c -o main
g++ -g main.cpp -o main

g++ -g -O0 main.cpp  -Wall -o main 
```



## 2main函数设置参数

```shell
gdb ./main
run 8080 ./www

set args 8080 ./www
run
```

**只能执行一次 start和run命令。**

**继续往后面执行continue   c指令**

**退出GDB:quit q**



## 3list指令

**list 文件名:行号**

**list 文件名:函数名**

**set listsize 行数**

**show list **



## 4断点

**重点关注某一个点**

**breakpoint。**

**b 行号**

**b 函数名**

























