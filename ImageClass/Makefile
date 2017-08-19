# Opções do compilador
CXX = g++
CXXFLAGS = -g -Wall

FONTES = ExemploDeManipulacaoDeImagens.cpp
OBJETOS = $(FONTES:.cpp=.o)

main: $(OBJETOS)
	$(CXX) $(OBJETOS) -o main ./SOIL/*.o ./ImageClass.o ./TextureClass.o -Llib -lGL -lGLU -lglut -ljpeg

clean:
	-@ rm -f $(OBJETOS) ${OBJAPP1} ${OBJAPP2}

depend:
	makedepend -- ${CFLAGS} -- ${FONTES}

# DO NOT DELETE
