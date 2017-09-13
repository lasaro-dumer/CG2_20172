CXX = g++
CXXFLAGS = -g -std=c++11

SRC_SOIL = SOIL/image_DXT.cpp  SOIL/image_helper.cpp  SOIL/SOIL.cpp  SOIL/stb_image_aug.cpp
SRC = MainFile.cpp Helpers/ImageClass.cpp Helpers/Image.cpp Helpers/util.cpp Helpers/carousel.hpp
BIN = main.run
OBJECTS = $(SRC:.cpp=.o)
SOIL_OBJECTS = $(SRC_SOIL:.cpp=.o)

main: $(OBJECTS)
	$(CXX) $(OBJECTS) $(SOIL_OBJECTS) -o $(BIN) -lGL -lGLU -lglut -ljpeg

clean:
	-@ rm -f $(OBJECTS) $(BIN)
