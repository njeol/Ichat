#include <stdlib.h>
#include "ichat.h"

void		init_list(t_list *l)
{
  l->first = NULL;
  l->last = NULL;
}


int       main(int argc, char **argv)
{
  t_list  chat;
  
  init_list(&chat);
  get_init(argv, chat);  
  return 0;
}