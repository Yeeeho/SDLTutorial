g++ -x c++-header^
 -I./include -I./lib -I./lib/SDL3/include -I./lib/SDL3_ttf/include^
 -I./lib/SDL3_image/include^
 include/pch.h -o include/pch.h.pch

g++ src/*.cpp^
 -I./include -I./lib -I./lib/SDL3/include -I./lib/SDL3_ttf/include^
 -I./lib/SDL3_image/include^

 -L./lib/SDL3/lib -L./lib/SDL3_ttf/lib -L./lib/SDL3_image/lib^
 -lSDL3 -lSDL3_ttf -lSDL3_image^
 -include include/pch.h -o test.exe -H