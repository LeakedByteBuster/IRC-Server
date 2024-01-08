NAME = ircserv

TESTS_NAME = testIRC

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra -g -fsanitize=address

STD = -std=c++98

INC_DIR = $(shell find include -type d)

INCLUDE = $(addprefix -I, $(INC_DIR))

SRC = $(shell find src -type f -name "*.cpp")

OBJ_DIR = obj

OBJ = $(patsubst %, $(OBJ_DIR)/%, $(SRC:.cpp=.o))

RM = rm -rf

DEPS = $(patsubst %.o, %.d, $(OBJ))

HYEL = "\e[1;93m"
HWHT = "\e[1;97m"
NC ='\033[0m'

#	$> make re build=tests
ifeq ($(build), tests)
	SRC=$(shell find tests -type f -name "*.cpp")
	NAME=testIRC
else ifeq ($(build), log)
	CXXFLAGS+=-DLOG
endif

all : $(NAME)

$(NAME) : $(OBJ)
	@ $(CXX) $(STD) $(CXXFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o : %.cpp
	@ mkdir -p $(dir $@) && printf $(HYEL)"[+] ==> "$(HWHT)
#	@ printf $(HYEL)"[+]"$(HWHT)" COMPILING"$(HYEL)" ⤐  "$(HWHT)"%s\n"$(NC) $(notdir $<)
	$(CXX) $(STD) $(CXXFLAGS) $(INCLUDE) -MMD -o $@ -c $<
	@ printf $(NC)

clean :
	@ $(RM) $(OBJ_DIR)

fclean : clean
	@ $(RM) $(NAME)

fcleanTest : 
	@ $(RM) $(TESTS_NAME)

re : fclean all

-include $(DEPS)