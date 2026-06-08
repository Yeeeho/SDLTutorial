#pragma once

#include <SDL3/SDL.h>

class helloSDL {
    public:
        /* 전역 변수 */

    //렌더할 윈도우
    SDL_Window* gWindow{nullptr};

    //윈도우가 가진 서페이스
    SDL_Surface* gScreenSurface{nullptr};

    //화면에 출력할 이미지
    SDL_Surface* gHelloWorld{nullptr};

    /* 화면 크기 상수 */
    const int kScreenWidth{640};
    const int kScreenHeight{480};
    //constexpr => 컴파일 시점에 상수 정의

    int Loop();
    int Init();
    int LoadMedia();
    void Close();
};