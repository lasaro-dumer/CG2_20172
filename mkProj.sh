echo "Creating project" $1
rm -rf $1
mkdir -p $1
ln -s ../ImageClass/SOIL $1
ln -s ../baseProj/Helpers $1
cp ImageClass/ImageClass.cpp $1/ImageClass.cpp
cp ImageClass/ImageClass.hpp $1/ImageClass.hpp
cp baseProj/MainFile.cpp $1/MainFile.cpp
cp baseProj/Makefile $1/Makefile
