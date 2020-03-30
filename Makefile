CXX = g++

LIB = -Llib -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2
INCLUDE = -Iinclude

CXXFLAGS = -Wall -c -std=c++11 $(INCLUDE)
LDFLAGS = -lmingw32 -mwindows -mconsole $(LIB)

SRC = lesson0.cpp
OBJ = $(SRC:.cpp=.o)
EXE = $(SRC:.cpp=.exe)

all: $(EXE)


$(EXE): $(OBJ)
	$(CXX) $< $(LDFLAGS) -o $@

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	del *.o && del $(EXE)
