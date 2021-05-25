echo "Building core..."
gcc ./src/core.c -o ./build/core.o \
    -Wall \
    -g \
    -I ./include \
    -L ./lib \
    -framework CoreVideo \
    -framework IOKit \
    -framework Cocoa \
    -framework GLUT \
    -framework OpenGL \
    lib/libraylib.a \
    -dynamiclib