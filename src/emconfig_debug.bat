mkdir build
cd build

emcmake cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=mingw32-make -DFREETYPE_INCLUDE_DIRS=C:\local\freetype-2.7\emscripten\install\include\freetype2 -DFREETYPE_LIBRARY=C:\local\freetype-2.7\emscripten\install\lib\libfreetype.a -G "Unix Makefiles"

cd ..
