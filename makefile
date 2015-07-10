###CONFIG
CXX = g++
CXXFLAGS := -std=c++11
CPPFLAGS = -Wall -Wextra -pedantic 
INCLUDE = -IC:\Users\Austin\Desktop\Collision\depend\SDL2-2.0.3\i686-w64-mingw32\include \
-IC:\Users\Austin\Desktop\Collision\depend\SDL2-2.0.3\i686-w64-mingw32\include\SDL2 \
-IC:\Users\Austin\Desktop\Collision\depend\SDL2_image-2.0.0\i686-w64-mingw32\include \
-IC:\Users\Austin\Desktop\Collision\depend\SDL2_ttf-2.0.12\i686-w64-mingw32\include \
-IC:\Users\Austin\Desktop\Collision\depend\SDL2_mixer-2.0.0\i686-w64-mingw32\include

LIBDIR = -LC:\Users\Austin\Desktop\Collision\depend\SDL2-2.0.3\i686-w64-mingw32\lib \
-LC:\Users\Austin\Desktop\Collision\depend\SDL2_image-2.0.0\i686-w64-mingw32\lib \
-LC:\Users\Austin\Desktop\Collision\depend\SDL2_ttf-2.0.12\i686-w64-mingw32\lib \
-LC:\Users\Austin\Desktop\Collision\depend\SDL2_mixer-2.0.0\i686-w64-mingw32\lib

LIBS = -lmingw32 \
-lSDL2main \
-lSDL2 \
-lSDL2_image \
-lSDL2_ttf \
-lSDL2_mixer 

source := main base/TexturePool


###END OF CONFIG

source := $(addsuffix .o, $(source))
source := $(addprefix src/, $(source))

CXXFLAGS := $(CXXFLAGS) $(INCLUDE) $(LIBDIR) $(LIBS)


all: main.exe

main.exe: $(source)
	$(CXX) $^ $(CPPFLAGS) $(CXXFLAGS) -o $(addprefix bin/, $@) 
	
src/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@