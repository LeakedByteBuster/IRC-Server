NAME = ircserv

TESTS_NAME = testIRC

CXX = c++

CXXFLAGS = -Wall -Werror -Wextra

STD = -std=c++98

INC_DIR = $(shell find include -type d)

INCLUDE = $(addprefix -I, $(INC_DIR))

SRC = $(shell find src -type f -name "*.cpp")

OBJ_DIR = obj


OBJ = $(patsubst %, $(OBJ_DIR)/%, $(SRC:.cpp=.o))

BOT_OBJ_DIR = objBot
BOTNAME = ircBotter

BOTSRC = $(shell find ircBot -type f -name "*.cpp")

BOTOBJ = $(patsubst %, $(BOT_OBJ_DIR)/%, $(BOTSRC:.cpp=.o))

BOT_INC = $(shell find include/bot -type d)

INC_BOT = $(addprefix -I, $(BOT_INC))

RM = rm -rf

DEPS = $(patsubst %.o, %.d, $(OBJ))

HYEL = "\e[1;93m"
HWHT = "\e[1;97m"
NC ='\033[0m'

#	$> make re build=log
ifeq ($(build), log)
	CXXFLAGS+=-DLOG
else ifeq ($(build), sani)
	CXXFLAGS+=-fsanitize=address
endif

all : $(NAME)

$(NAME) : $(OBJ)
	@ $(CXX) $(STD) $(CXXFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

bot : $(BOTNAME)

$(BOTNAME) : $(BOTOBJ)
	@ $(CXX) $(STD) $(CXXFLAGS) $(INC_BOT) $(BOTOBJ) -o $(BOTNAME)


$(BOT_OBJ_DIR)/%.o : %.cpp
	@ mkdir -p $(dir $@) && printf $(HYEL)"[+] ==> "$(HWHT)
	$(CXX) $(STD) $(CXXFLAGS) $(INC_BOT) -MMD -o $@ -c $<
	@ printf $(NC)

$(OBJ_DIR)/%.o : %.cpp
	@ mkdir -p $(dir $@) && printf $(HYEL)"[+] ==> "$(HWHT)
#	@ printf $(HYEL)"[+]"$(HWHT)" COMPILING"$(HYEL)" ⤐  "$(HWHT)"%s\n"$(NC) $(notdir $<)
	$(CXX) $(STD) $(CXXFLAGS) $(INCLUDE) -MMD -o $@ -c $<
	@ printf $(NC)

clean :
	@ $(RM) $(OBJ_DIR)

fclean : clean
	@ $(RM) $(NAME)

cleanbot :
	@ $(RM) $(OBJ_DIR)

fcleanbot : clean
	@ $(RM) $(BOTNAME)

re : fclean all

rebot : fcleanbot cleanbot

-include $(DEPS)

.PHONY : all bot clean fclean cleanbot fcleanbot re
