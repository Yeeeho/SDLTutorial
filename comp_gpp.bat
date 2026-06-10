g++ -x c++-header^
 -I./include -I./lib -I./lib/SDL3/include -I./lib/SDL3_ttf/include^
 -I./lib/SDL3_image/include -I./lib/SDL3_mixer/include^
 include/pch.h -o include/pch.h.gch

g++ src/*.cpp^
 -I./include -I./lib -I./lib/SDL3/include -I./lib/SDL3_ttf/include^
 -I./lib/SDL3_image/include -I./lib/SDL3_mixer/include^

 -L./lib/SDL3/lib -L./lib/SDL3_ttf/lib -L./lib/SDL3_image/lib -L./lib/SDL3_mixer/lib^
 -lSDL3 -lSDL3_ttf -lSDL3_image -lSDL3_mixer^
 -include include/pch.h -o test.exe -H