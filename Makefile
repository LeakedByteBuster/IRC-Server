NAME = irc

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra

STD = -std=c++98

INC_DIR = $(shell find include -type d)
INCLUDE = $(addprefix -I, $(INC_DIR))

SRC = $(shell find src -type f -name "*.cpp")

OBJ_DIR = obj

OBJ = $(patsubst %, $(OBJ_DIR)/%, $(SRC:.cpp=.o))

RM = rm -rf

DEPS = $(patsubst %.o, %.d, $(OBJ))

HRED = "\e[1;91m"
HWHT = "\e[1;97m"
NC ='\033[0m'

all : $(NAME)

$(NAME) : $(OBJ)
	@ $(CXX) $(STD) $(CXXFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o : %.cpp
	@ mkdir -p $(dir $@)
	@ printf $(HRED)"[+]"$(HWHT)" COMPILING"$(HRED)" ⤐  "$(HWHT)"%s\n"$(NC) $(notdir $<)
	@ $(CXX) $(STD) $(CXXFLAGS) $(INCLUDE) -MMD -c $< -o $@

clean :
	@ $(RM) $(OBJ_DIR)

fclean : clean
	@ $(RM) $(NAME)

re : fclean all

-include $(DEPS)