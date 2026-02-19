NAME	:=	Motivation_Maker

SOURCES	:=	$(shell cd ./src | find -name '*.c')

OBJECTS	:=	$(SOURCES:".c"=.o)


CFLAGS	:=	-I./include -Wall -Wextra -Werror
LIBFLAGS	:= -lzip

#LIB_PATH	:= /mnt/c/Users/giros/Documents/matrix_operation_lib/

CC	:=	gcc

all:	$(NAME)

re:
	rm -f $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBFLAGS)

exec: all
	./$(NAME)