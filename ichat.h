#ifndef __ICHAT_H__
#define __ICHAT_H__

/*
** INIT_FCT
*/

int   f_socket();
int   f_listen(int listener);
void  f_bind(int listener, struct sockaddr_in serveraddr);
void  f_setsockopt(int listener);
int   f_select(int fdmax, fd_set read_fds);

#endif /* __ICHAT_H__ */