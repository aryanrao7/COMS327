CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -Werror -O2 -funroll-loops
CXXFLAGS = -Wall -Werror -O2 -funroll-loops
LDFLAGS = -lncurses

BIN = games
OBJS = games.o player.o tictactoe.o

all: $(BIN) blackjack connect

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) blackjack connect *.d TAGS core vgcore.* gmon.out

blackjack:
	@$(ECHO) Compiling and linking blackjack.cpp
	@$(CXX) $(CXXFLAGS) -o blackjack blackjack.cpp $(LDFLAGS)
	@strip blackjack  # Strip debugging information

connect:
	@$(ECHO) Compiling and linking connect.cpp
	@$(CXX) $(CXXFLAGS) -o connect connect.cpp $(LDFLAGS)
	@strip connect  # Strip debugging information

