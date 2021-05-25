set -e
mkdir -p ./build/

run=0

for arg in "$@"; do
    if [[ "$arg" = "--hot-reload" ]]; then
        hotReload=1
    fi

    if [[ "$arg" = "--run" ]]; then
        run=1
    fi
done

echo "============================================";
./win64_build_core.bat

echo "Building game..."
gcc ./src/win64_game.c -o ./build/game \
    -g  \
    -Wall \
    -I ./include \
    -L ./lib \
    -l raylib -l opengl32 -l gdi32 -l winmm
echo "============================================";

if [ $run == 1 ]; then
    ./build/game
fi