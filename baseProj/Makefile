CXX = g++
CXXFLAGS = -g

SRC_SOIL = SOIL/image_DXT.cpp  SOIL/image_helper.cpp  SOIL/SOIL.cpp  SOIL/stb_image_aug.cpp
SRC = MainFile.cpp ImageClass.cpp
BIN = main.run
OBJECTS = $(SRC:.cpp=.o)
SOIL_OBJECTS = $(SRC_SOIL:.cpp=.o)

main: $(OBJECTS)
	$(CXX) $(OBJECTS) $(SOIL_OBJECTS) -o $(BIN) -lGL -lGLU -lglut -ljpeg

clean:
	-@ rm -f $(OBJECTS) $(BIN)
