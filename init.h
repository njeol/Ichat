#ifndef __INIT_H__
#define __INIT_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct	    s_list
{
  char		          *login;
  int		            fd;
  struct s_list	    *next;
  struct s_list	    *prev;
  struct s_list	    *first;
  struct s_list	    *last;
}                   t_list;

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
char  *send_msg(int listener, int fdmax, int i, fd_set master, int flag_login, t_list chat);
int   new_member(int flag_new_member, int fdmax, int flag_login_register, int i);
void  get_init(char **argv, t_list chat);

/*
** FUNC_LIST
*/

int     check_list(t_list l, char *login_new);
void		show_list(t_list l);
void		put_in_list_front(t_list *l, int fd_number, char *login);
  

#endif /* __ICHAT_H__ */