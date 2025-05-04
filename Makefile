TARGET_UTILITIES = src/util/class_def.h src/util/enum_def.h src/util/struct_def.h src/include/jsoncpp.cpp src/global.cpp src/util/draw.cpp src/util/font_loader.cpp
TARGET_COMPONENTS = src/components/sprite_component.cpp src/components/tween_object.cpp src/components/tween_component.cpp src/components/physics_component.cpp src/components/input_component.cpp
TARGET_OBJECTS = src/objects/game_object.cpp src/objects/baby.cpp src/objects/enemy.cpp src/objects/ghost.cpp src/objects/shikigami.cpp src/objects/fox.cpp src/objects/jizo.cpp src/objects/player.cpp src/objects/ui.cpp
TARGET = src/main.cpp
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

all:
	g++ -Isrc/include/SDL2 -Isrc/include/json -Isrc/include/tweeny -Lsrc/lib src/game.cpp ${TARGET_UTILITIES} ${TARGET_COMPONENTS} ${TARGET_OBJECTS} ${TARGET} -o game ${LDFLAGS}