set -e
mkdir -p ./build/

run=0

for arg in "$@"; do
    if [[ "$arg" = "--run" ]]; then
        run=1
    fi
done

echo "=================================";
./mac_build_core.sh

echo "Building game..."
gcc ./src/mac_game.c -o ./build/game \
    -Wall \
    -g \
    -I ./include \
    -L ./lib \
    -framework CoreVideo \
    -framework IOKit \
    -framework Cocoa \
    -framework GLUT \
    -framework OpenGL \
    lib/libraylib.a
echo "=================================";

if [ $run == 1 ]; then
    ./build/game
fi