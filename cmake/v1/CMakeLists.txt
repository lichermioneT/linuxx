cmake_minimum_required(VERSION 3.0) # 最低版本，不能超过下载的
project(test)                       # 项目名称


add_executable(          # 括号注意的
    calc                 # 可执行的程序 
    add.cpp              # 源文件 可以空格，可以分号
    div.cpp     
    mul.cpp     
    sub.cpp     
    main.cpp    
)