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
      printf("\n***login:%s***\n", new->login);
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

int check_list_delog(t_list *l, int number_fd)
{
   l = l->first;
   while (l)
   {
     t_list *new;
     int number = 0;
     if ((number_fd == l->fd) /*&& (l->next) && (l->prev)*/)
       {
        if (l->prev)
        {
         l = l->prev;
         new = l->next;
         }
         else
         { 
           l = l->next;
           new = l->prev;
         }
         l->next = new->next;
         free(new);
         new = NULL;
         printf("OK\n");
         return(0);
       }
           // else
    //   {
    //     printf("\n   LAST   \n");
    //      new = l->last;
    //      number = new->fd;
    //      l->last = new->prev;
    //      if (l->last)
    //        l->last->next = NULL;
    //      else 
    //        l->first = NULL;
    //      return(0);
    //   }
    // else if ((number_fd == l->fd) && l->next)
    // {
    //   printf("\n   PREMIER  \n");
    //   new = l->first;
    //   printf("\n   A  \n");
    //   // new = l->next;
    //   printf("\n   B  \n");
    //   number = l->fd;
    //   printf("\n   C  \n");
    //   l->first = l->next;
    //   printf("\n   D  \n");
    //   if (l->first)
    //     l->first->prev = NULL;
    //   else 
    //     l->last = NULL;
    //   printf("\n   FIN  \n");
    //   return (0);
    // }
    // else if ((number_fd == l->fd) && l->last)
    // {
    //   printf("\n   LAST   \n");
    //   new = l->last;
    //   number = new->fd;
    //   l->last = new->prev;
    //   if (l->last)
    //     l->last->next = NULL;
    //   else 
    //     l->first = NULL;
    //   return(0);
    // }
      l = l->next;
  }
    return(1);
}

void delete_last(t_list *l, int number_fd)
{
  
}

void delete_first(t_list *l, int number_fd, int fdmax)
{
  
}

int a_check_list_delog(t_list *l, int number_fd, int fdmax)
{
  int number = 0;
  t_list *new;
  
  if (number_fd == 4)
  {
    printf("\n  LAST  \n");
    new = l->last;
    number = new->fd;
    l->last = new->prev;
    if (l->last)
      l->last->next = NULL;
    else 
      l->first = NULL;
  }
  else if (number_fd == fdmax)
  {
    printf("\n   PREMIER  \n");
    new = l->first;
    number = new->fd;
    l->first = new->next;
    if (l->first)
      l->first->prev = NULL;
    else 
      l->last = NULL;
  }
  else
  {
    l = l->first;
    while (l)
    {
      if (number_fd != 4)
      {
        printf("\n   OTHER   \n");
        new = l->next;
        l->next = new->next;
        free(new);
        new = NULL;
        l = l->next;
      }
    }
  }
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
