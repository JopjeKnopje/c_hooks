NAME = app

SRC = main.c


LIB_NAME = libhook.so
LIB_SRC = libhook.c
LIB_FLAGS = -fPIC -shared -ldl

all: $(NAME) $(LIB_NAME)


$(NAME): $(SRC)
	$(CC) $(SRC) -o $(NAME) 

$(LIB_NAME): $(LIB_SRC)
	$(CC) $(LIB_SRC) -o $(LIB_NAME) $(LIB_FLAGS)

fclean:
	rm $(NAME) $(LIB_NAME)
