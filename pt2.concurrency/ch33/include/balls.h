
#ifndef _SERVER_H
#define _SERVER_H 
#define BUF_SIZE 1024

typedef enum { PATH = 1, STRING = 2 } MessageType;
struct message {
  MessageType type;
  char content[BUF_SIZE];
};

#endif
