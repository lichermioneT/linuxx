#define _DEFAULT_SOURCE
#include "server.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_EVENTS 1024
#define REQ_BUF_SIZE 8192
#define LISTEN_BACKLOG 128

static int setNonBlock(int fd);
static int setBlock(int fd);
static int addEpollEvent(int epfd, int fd, uint32_t events);
static void removeClient(int epfd, int fd);

static int handleAccept(int epfd, int lfd);
static int handleClientRequest(int epfd, int cfd);
static int parseRequestLine(const char* line, int cfd);

static int sendAll(int fd, const void* buf, size_t len);
static int sendFile(const char* fileName, int cfd);
static int sendHeadMsg(int cfd, int status, const char* descr, const char* type, long long length);
static int sendDir(const char* dirName, int cfd);
static int sendSimpleErrorPage(int cfd, int status, const char* title, const char* msg);

static const char* getFileType(const char* name);
static void formatSize(off_t size, char* out, size_t len);
static int hexToDec(char c);
static void decodeMsg(char* to, const char* from);
static int containsParentRef(const char* path);
static void urlEncode(const char* src, char* dst, size_t dstSize);
static void htmlEscape(const char* src, char* dst, size_t dstSize);

int initListenFd(unsigned short port)
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(lfd);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(lfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(lfd);
        return -1;
    }

    if (listen(lfd, LISTEN_BACKLOG) == -1) {
        perror("listen");
        close(lfd);
        return -1;
    }

    if (setNonBlock(lfd) == -1) {
        perror("fcntl");
        close(lfd);
        return -1;
    }

    return lfd;
}

int epollRun(int lfd)
{
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        return -1;
    }

    if (addEpollEvent(epfd, lfd, EPOLLIN) == -1) {
        perror("epoll_ctl add listen fd");
        close(epfd);
        return -1;
    }

    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nready == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nready; ++i) {
            int fd = events[i].data.fd;
            uint32_t ev = events[i].events;

            if (fd == lfd) {
                if (handleAccept(epfd, lfd) == -1) {
                    fprintf(stderr, "handleAccept failed\n");
                }
                continue;
            }

            if (ev & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                removeClient(epfd, fd);
                continue;
            }

            if (ev & EPOLLIN) {
                handleClientRequest(epfd, fd);
            }
        }
    }

    close(epfd);
    return -1;
}

static int setNonBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static int setBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
}

static int addEpollEvent(int epfd, int fd, uint32_t events)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = events;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

static void removeClient(int epfd, int fd)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
}

static int handleAccept(int epfd, int lfd)
{
    while (1) {
        int cfd = accept(lfd, NULL, NULL);
        if (cfd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return 0;
            }
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
            return -1;
        }

        if (setNonBlock(cfd) == -1) {
            perror("setNonBlock client");
            close(cfd);
            continue;
        }

        if (addEpollEvent(epfd, cfd, EPOLLIN | EPOLLRDHUP) == -1) {
            perror("epoll_ctl add client");
            close(cfd);
            continue;
        }
    }
}

static int handleClientRequest(int epfd, int cfd)
{
    char buffer[REQ_BUF_SIZE + 1];
    ssize_t total = 0;

    while (1) {
        ssize_t n = recv(cfd, buffer + total, REQ_BUF_SIZE - total, 0);
        if (n > 0) {
            total += n;
            if (total >= REQ_BUF_SIZE) {
                if (setBlock(cfd) == 0) {
                    sendSimpleErrorPage(cfd, 414, "URI Too Long", "Request header is too large.");
                }
                removeClient(epfd, cfd);
                return -1;
            }
            continue;
        }

        if (n == 0) {
            removeClient(epfd, cfd);
            return 0;
        }

        if (errno == EINTR) {
            continue;
        }

        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            break;
        }

        perror("recv");
        removeClient(epfd, cfd);
        return -1;
    }

    buffer[total] = '\0';

    char* lineEnd = strstr(buffer, "\r\n");
    if (lineEnd == NULL) {
        if (setBlock(cfd) == 0) {
            sendSimpleErrorPage(cfd, 400, "Bad Request", "Cannot parse request line.");
        }
        removeClient(epfd, cfd);
        return -1;
    }

    *lineEnd = '\0';

    if (setBlock(cfd) == -1) {
        perror("setBlock client");
        removeClient(epfd, cfd);
        return -1;
    }

    parseRequestLine(buffer, cfd);
    removeClient(epfd, cfd);
    return 0;
}

static int parseRequestLine(const char* line, int cfd)
{
    char method[16] = {0};
    char url[1024] = {0};
    char version[32] = {0};
    char decodedPath[1024] = {0};
    char filePath[PATH_MAX] = {0};

    int cnt = sscanf(line, "%15s %1023s %31s", method, url, version);
    if (cnt < 2) {
        return sendSimpleErrorPage(cfd, 400, "Bad Request", "Invalid request line.");
    }

    if (strcasecmp(method, "GET") != 0) {
        return sendSimpleErrorPage(cfd, 405, "Method Not Allowed", "Only GET is supported.");
    }

    decodeMsg(decodedPath, url);

    char* query = strchr(decodedPath, '?');
    if (query != NULL) {
        *query = '\0';
    }

    if (containsParentRef(decodedPath)) {
        return sendSimpleErrorPage(cfd, 403, "Forbidden", "Illegal path.");
    }

    if (strcmp(decodedPath, "/") == 0) {
        snprintf(filePath, sizeof(filePath), ".");
    } else {
        snprintf(filePath, sizeof(filePath), ".%s", decodedPath);
    }

    struct stat st;
    if (stat(filePath, &st) == -1) {
        if (access("./404.html", F_OK) == 0) {
            sendHeadMsg(cfd, 404, "Not Found", "text/html; charset=utf-8", -1);
            return sendFile("./404.html", cfd);
        }
        return sendSimpleErrorPage(cfd, 404, "Not Found", "Resource does not exist.");
    }

    if (S_ISDIR(st.st_mode)) {
        if (sendHeadMsg(cfd, 200, "OK", "text/html; charset=utf-8", -1) == -1) {
            return -1;
        }
        return sendDir(filePath, cfd);
    }

    if (sendHeadMsg(cfd, 200, "OK", getFileType(filePath), (long long)st.st_size) == -1) {
        return -1;
    }

    return sendFile(filePath, cfd);
}

static int sendAll(int fd, const void* buf, size_t len)
{
    const char* p = (const char*)buf;
    size_t left = len;

    while (left > 0) {
        ssize_t n = send(fd, p, left, 0);
        if (n > 0) {
            p += n;
            left -= (size_t)n;
            continue;
        }

        if (n == -1 && errno == EINTR) {
            continue;
        }

        perror("send");
        return -1;
    }

    return 0;
}

static int sendFile(const char* fileName, int cfd)
{
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        perror("open file");
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return -1;
    }

    off_t offset = 0;
    while (offset < st.st_size) {
        ssize_t n = sendfile(cfd, fd, &offset, (size_t)(st.st_size - offset));
        if (n > 0) {
            continue;
        }

        if (n == -1 && errno == EINTR) {
            continue;
        }

        perror("sendfile");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

static int sendHeadMsg(int cfd, int status, const char* descr, const char* type, long long length)
{
    char buf[1024];
    int n = snprintf(buf, sizeof(buf),
                     "HTTP/1.1 %d %s\r\n"
                     "Content-Type: %s\r\n",
                     status, descr, type);
    if (n < 0 || (size_t)n >= sizeof(buf)) {
        errno = EOVERFLOW;
        perror("snprintf header");
        return -1;
    }

    if (length >= 0) {
        int m = snprintf(buf + n, sizeof(buf) - (size_t)n,
                         "Content-Length: %lld\r\n", length);
        if (m < 0 || (size_t)m >= sizeof(buf) - (size_t)n) {
            errno = EOVERFLOW;
            perror("snprintf content-length");
            return -1;
        }
        n += m;
    }

    int m = snprintf(buf + n, sizeof(buf) - (size_t)n,
                     "Connection: close\r\n"
                     "\r\n");
    if (m < 0 || (size_t)m >= sizeof(buf) - (size_t)n) {
        errno = EOVERFLOW;
        perror("snprintf end header");
        return -1;
    }
    n += m;

    return sendAll(cfd, buf, (size_t)n);
}

static int sendSimpleErrorPage(int cfd, int status, const char* title, const char* msg)
{
    char body[1024];
    int bodyLen = snprintf(body, sizeof(body),
                           "<!DOCTYPE html>"
                           "<html lang=\"zh-CN\">"
                           "<head><meta charset=\"UTF-8\"><title>%d %s</title></head>"
                           "<body><h1>%d %s</h1><p>%s</p></body></html>",
                           status, title, status, title, msg);
    if (bodyLen < 0) {
        return -1;
    }

    if (sendHeadMsg(cfd, status, title, "text/html; charset=utf-8", bodyLen) == -1) {
        return -1;
    }
    return sendAll(cfd, body, (size_t)bodyLen);
}

static int sendDir(const char* dirName, int cfd)
{
    struct dirent** namelist = NULL;
    int num = scandir(dirName, &namelist, NULL, alphasort);

    char buf[4096];
    int n = snprintf(buf, sizeof(buf),
                     "<!DOCTYPE html>"
                     "<html lang=\"zh-CN\">"
                     "<head>"
                     "<meta charset=\"UTF-8\">"
                     "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                     "<title>目录索引</title>"
                     "<style>"
                     "body{font-family:Arial,\"Microsoft YaHei\",sans-serif;background:#f5f7fa;margin:0;padding:30px;color:#333;}"
                     ".container{max-width:900px;margin:0 auto;background:#fff;border-radius:12px;box-shadow:0 4px 20px rgba(0,0,0,.08);padding:24px;}"
                     "h1{margin-top:0;font-size:28px;color:#222;}"
                     ".path{color:#666;margin-bottom:20px;font-size:14px;word-break:break-all;}"
                     "table{width:100%;border-collapse:collapse;}"
                     "th,td{padding:12px 14px;border-bottom:1px solid #eee;text-align:left;}"
                     "th{background:#f0f3f8;color:#444;}"
                     "tr:hover{background:#f9fbff;}"
                     "a{text-decoration:none;color:#1677ff;}"
                     "a:hover{text-decoration:underline;}"
                     ".dir{color:#d48806;font-weight:bold;}"
                     ".footer{margin-top:18px;color:#888;font-size:13px;}"
                     "</style>"
                     "</head>"
                     "<body><div class=\"container\">"
                     "<h1>目录索引</h1>"
                     "<div class=\"path\">当前路径：%s</div>"
                     "<table><tr><th>名称</th><th>类型</th><th>大小</th></tr>",
                     dirName);
    if (n < 0 || sendAll(cfd, buf, (size_t)n) == -1) {
        if (num >= 0 && namelist != NULL) {
            for (int i = 0; i < num; ++i) {
                free(namelist[i]);
            }
            free(namelist);
        }
        return -1;
    }

    if (num < 0) {
        const char* msg = "<tr><td colspan=\"3\">无法读取目录</td></tr>";
        sendAll(cfd, msg, strlen(msg));
    } else {
        for (int i = 0; i < num; ++i) {
            const char* name = namelist[i]->d_name;

            if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
                free(namelist[i]);
                continue;
            }

            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirName, name);

            struct stat st;
            if (stat(fullPath, &st) == -1) {
                free(namelist[i]);
                continue;
            }

            char href[1024];
            char display[1024];
            char sizeBuf[64];

            urlEncode(name, href, sizeof(href));
            htmlEscape(name, display, sizeof(display));

            if (S_ISDIR(st.st_mode)) {
                snprintf(sizeBuf, sizeof(sizeBuf), "-");
                n = snprintf(buf, sizeof(buf),
                             "<tr><td><a class=\"dir\" href=\"%s/\">📁 %s/</a></td><td>目录</td><td>%s</td></tr>",
                             href, display, sizeBuf);
            } else {
                formatSize(st.st_size, sizeBuf, sizeof(sizeBuf));
                n = snprintf(buf, sizeof(buf),
                             "<tr><td><a href=\"%s\">📄 %s</a></td><td>文件</td><td>%s</td></tr>",
                             href, display, sizeBuf);
            }

            if (n > 0) {
                sendAll(cfd, buf, (size_t)n);
            }
            free(namelist[i]);
        }
        free(namelist);
    }

    n = snprintf(buf, sizeof(buf),
                 "</table><div class=\"footer\">Simple HTTP Server Directory Listing</div></div></body></html>");
    if (n > 0) {
        sendAll(cfd, buf, (size_t)n);
    }
    return 0;
}

static const char* getFileType(const char* name)
{
    const char* dot = strrchr(name, '.');
    if (dot == NULL) return "text/plain; charset=utf-8";

    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0) return "text/html; charset=utf-8";
    if (strcmp(dot, ".txt") == 0) return "text/plain; charset=utf-8";
    if (strcmp(dot, ".css") == 0) return "text/css; charset=utf-8";
    if (strcmp(dot, ".js") == 0) return "application/javascript; charset=utf-8";
    if (strcmp(dot, ".json") == 0) return "application/json; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(dot, ".png") == 0) return "image/png";
    if (strcmp(dot, ".gif") == 0) return "image/gif";
    if (strcmp(dot, ".svg") == 0) return "image/svg+xml";
    if (strcmp(dot, ".mp3") == 0) return "audio/mpeg";
    if (strcmp(dot, ".wav") == 0) return "audio/wav";
    if (strcmp(dot, ".mp4") == 0) return "video/mp4";
    if (strcmp(dot, ".pdf") == 0) return "application/pdf";

    return "application/octet-stream";
}

static void formatSize(off_t size, char* out, size_t len)
{
    if (size < 1024) {
        snprintf(out, len, "%lld B", (long long)size);
    } else if (size < 1024LL * 1024) {
        snprintf(out, len, "%.2f KB", size / 1024.0);
    } else if (size < 1024LL * 1024 * 1024) {
        snprintf(out, len, "%.2f MB", size / (1024.0 * 1024));
    } else {
        snprintf(out, len, "%.2f GB", size / (1024.0 * 1024 * 1024));
    }
}

static int hexToDec(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

static void decodeMsg(char* to, const char* from)
{
    while (*from != '\0') {
        if (from[0] == '%' &&
            isxdigit((unsigned char)from[1]) &&
            isxdigit((unsigned char)from[2])) {
            *to++ = (char)(hexToDec(from[1]) * 16 + hexToDec(from[2]));
            from += 3;
        } else if (*from == '+') {
            *to++ = ' ';
            ++from;
        } else {
            *to++ = *from++;
        }
    }
    *to = '\0';
}

static int containsParentRef(const char* path)
{
    return strstr(path, "..") != NULL;
}

static void urlEncode(const char* src, char* dst, size_t dstSize)
{
    static const char hex[] = "0123456789ABCDEF";
    size_t j = 0;

    for (size_t i = 0; src[i] != '\0' && j + 1 < dstSize; ++i) {
        unsigned char c = (unsigned char)src[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '/') {
            dst[j++] = (char)c;
        } else {
            if (j + 3 >= dstSize) {
                break;
            }
            dst[j++] = '%';
            dst[j++] = hex[c >> 4];
            dst[j++] = hex[c & 0x0F];
        }
    }
    dst[j] = '\0';
}

static void htmlEscape(const char* src, char* dst, size_t dstSize)
{
    size_t j = 0;

    for (size_t i = 0; src[i] != '\0' && j + 1 < dstSize; ++i) {
        const char* rep = NULL;
        switch (src[i]) {
            case '&': rep = "&amp;"; break;
            case '<': rep = "&lt;"; break;
            case '>': rep = "&gt;"; break;
            case '"': rep = "&quot;"; break;
            case '\'': rep = "&#39;"; break;
            default: break;
        }

        if (rep != NULL) {
            size_t repLen = strlen(rep);
            if (j + repLen >= dstSize) {
                break;
            }
            memcpy(dst + j, rep, repLen);
            j += repLen;
        } else {
            dst[j++] = src[i];
        }
    }

    dst[j] = '\0';
}