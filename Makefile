TARGET = src/include/jsoncpp.cpp src/global.cpp src/util/draw.cpp src/util/font_loader.cpp src/game_objects/sprite.cpp src/game_objects/tween_object.cpp src/game_objects/game_object.cpp src/game_objects/seal.cpp src/game_objects/enemy.cpp src/game_objects/ghost.cpp src/game_objects/player.cpp src/main.cpp
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

all:
	g++ -Isrc/include/SDL2 -Isrc/include/json -Isrc/include/tweeny -Lsrc/lib ${TARGET} -o game ${LDFLAGS}