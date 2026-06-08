#pragma once

#include <SDL3/SDL.h>

/*전역 변수*/
//렌더링 할 윈도우
extern SDL_Window* gWindow {nullptr};

//윈도우를 렌더링할 렌더러
extern SDL_Renderer* gRenderer{nullptr};

//Background color defaults to white
extern SDL_Color bgColor { 0xFF, 0xFF, 0xFF, 0xFF };

/* 화면 크기 상수 */
extern const int kScreenWidth{640};
extern const int kScreenHeight{480};