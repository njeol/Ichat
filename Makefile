NAME    =       ichat
SCRS    =       serveur_select3.c
OBJS    =       $(SRCS:.c=.o)
CC      =       gcc
RM      =       rm -rf

$(NAME) : $(OBJS)
	  $(CC) -o $(NAME) $(SCRS)

clean   :
	 $(RM) *.o
	 $(RM) \#*
	 $(RM) *~

all     : $(NAME)

fclean  : clean
	$(RM) $(NAME)

re      : fclean all