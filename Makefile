NAME = ircserv

SRCDIR = srcs
OBJDIR = .objs

SOURCES = $(SRCDIR)/main.cpp \
		  $(SRCDIR)/Client.cpp \
		  $(SRCDIR)/Channel.cpp \
		  $(SRCDIR)/ServerSetup.cpp \
          $(SRCDIR)/ServerRun.cpp \
          $(SRCDIR)/Signals.cpp	\
		  $(SRCDIR)/commands/nick.cpp \
		  $(SRCDIR)/commands/user.cpp \
		  $(SRCDIR)/commands/roses.cpp \
		  $(SRCDIR)/commands/privMsg.cpp \
		  $(SRCDIR)/commands/notice.cpp \
		  $(SRCDIR)/commands/oper.cpp \
		  $(SRCDIR)/commands/pass.cpp \
		  $(SRCDIR)/commands/commands_utils.cpp \
		  $(SRCDIR)/commands/join.cpp \
		  $(SRCDIR)/commands/kick.cpp \
		  $(SRCDIR)/commands/quit.cpp \
		  $(SRCDIR)/commands/list.cpp \
		  $(SRCDIR)/commands/invite.cpp \
		  $(SRCDIR)/commands/mode.cpp \
		  $(SRCDIR)/commands/topic.cpp \
		  $(SRCDIR)/commands/names.cpp \
		  $(SRCDIR)/commands/part.cpp \
		  $(SRCDIR)/commands/mode_params/inviteMode.cpp \
		  $(SRCDIR)/commands/mode_params/protectedTopicMode.cpp \
		  $(SRCDIR)/commands/mode_params/keyMode.cpp \
		  $(SRCDIR)/commands/mode_params/operatorsMode.cpp \
		  $(SRCDIR)/commands/mode_params/clientLimitMode.cpp \

OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

INCLUDES = includes/IRC.hpp \
		   includes/Client.hpp \
		   includes/Channel.hpp \
		   includes/Server.hpp \
		   includes/Commands.hpp \
		   includes/NumericalReplies.hpp \

CC = c++
MKDIR = mkdir -p
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3

RM = rm -rf

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCLUDES) Makefile
	@$(MKDIR) $(@D)
	$(CC) $(FLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -lpthread

clean :
	$(RM) $(OBJDIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re
