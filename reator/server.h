#ifndef SERVER_H
#define SERVER_H

int initListenFd(unsigned short port);
int epollRun(int lfd);

#endif