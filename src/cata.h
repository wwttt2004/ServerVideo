#if ! defined ( CATA_H_ )
#define CATA_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h> //Pour STDIN_FILENO
#include "utils.h"

#define DATA_DIRECTORY "data/"
#define MAX_HEADER 256
#define MAX_STR 32
#define MAX_CATA 1024
#define BASE_FICHIERS 32 

char * build_date();

char * build_http_header(char * type, int size);

char * buildCatalogue (int epollfd, struct tabFichiers * tabFichiers);

#endif // CATA_H_