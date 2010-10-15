#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "init.h"

void		show_list(t_list l)
{
  t_list	*new;
  
  new = l.first;
  while (new)
    {
      printf("***login:%s***", new->login);
      printf("fd:%d \n", new->fd);
      new = new->next;
    }
  printf("\n");

}

char *check_list_return_login(t_list l, int number_fd)
{
  t_list *new;
  char *bad_return = "unfind";
  
  new = l.first;
  while (new)
  {
    if (number_fd == new->fd)
      return (new->login);
    new = new->next;
  }
  return (bad_return);
}

int     check_list_return_fd(t_list l, char *login_new)
{
  t_list	*new;
  
  new = l.first;
  while (new)
    {
        if (strcmp(new->login, login_new) == 0)
          return (new->fd);
      new = new->next;
    }
  printf("\n");
  return (0);
}

void		put_in_list_front(t_list *l, int fd_number, char *login)
{
  t_list	*new;

  new = malloc(sizeof(*new));
  new->fd = fd_number;
  new->login = login;
  new->next = l->first;
  new->prev = NULL;
  if (l->first)
    l->first->prev = new;
  else
    l->last = new;
  l->first = new;
}
