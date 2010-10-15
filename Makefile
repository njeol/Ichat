NAME    =       ichat
SCRS    =       global.c    \
                init.c      \
                main.c      \
                func_list.c
OBJS    =       $(SRCS:.c=.o)
CC      =       gcc
RM      =       rm -rf

$(NAME) : $(OBJS)
	  $(CC) -o $(NAME) $(SCRS)

all     : $(NAME)

clean   :
	 $(RM) *.o
	 $(RM) \#*
	 $(RM) *~


fclean  : clean
	$(RM) $(NAME)

re      : fclean all