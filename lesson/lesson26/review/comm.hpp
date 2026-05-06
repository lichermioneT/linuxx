#ifndef __COMM_HPP__
#define __COMM_HPP__ 

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_SIZE 4096

#define PATHNAME "/tmp"
#define PROJ_ID  0X66 

