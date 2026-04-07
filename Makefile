NAME	= raytracing
CPP	= c++
CPPFLAG	= -Wall -Wextra -Werror -std=c++11
# CPPFLAG	+= -fsanitize=address -g
INC_DIR	= ./includes
SRCS	= srcs/main.cpp

OBJS	= $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CPP) $(CPPFLAG) -I$(INC_DIR) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAG) -I$(INC_DIR) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean