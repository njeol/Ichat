#ifndef   __ICHAT_H__
 #define  __ICHAT_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*
** INIT.C
*/

int   f_socket();
int   f_listen(int listener);
void  f_bind(int listener, struct sockaddr_in serveraddr);
void  f_setsockopt(int listener);
int   f_select(int fdmax, fd_set read_fds);

/*
** GLOBAL.C
*/
void  send_msg(int listener, int fdmax, int i, fd_set master);
int   new_member(int flag, int fdmax);
void  get_init(char **argv);

#endif /* __ICHAT_H__ */