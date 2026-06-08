#include "pch.h"

#include "helloSDL.h"

int helloSDL::Loop()
{

    //종료 코드
    int exitCode {0};

    if (Init() == false) {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    } 
    else {
        //미디어 로드
        if (LoadMedia() == false) {
            SDL_Log("Unable to load media!\n");
            exitCode = 2;
        }
        else {
            //종료 플래그 변수
            bool quit {false};
            //이벤트 데이터
            SDL_Event e;
            SDL_zero(e);
        
            //메인 루프
            while(quit == false) {
                //이벤트 데이터 습득
                while (SDL_PollEvent(&e) == true) {
                    //이벤트가 종료 타입일 때
                    if (e.type == SDL_EVENT_QUIT) {
                        //메인 루프를 종료함.
                        quit = true;
                    }
                }

                //서페이스를 하얀색으로 채움
                SDL_FillSurfaceRect(gScreenSurface, nullptr, SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF));

                //이미지를 렌더링함
                SDL_BlitSurface(gHelloWorld, nullptr, gScreenSurface, nullptr);

                //서페이스를 업데이트함
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    //마무리
    Close();

    return exitCode;
}

int helloSDL::Init()
{
    //플래그 변수 초기화
    bool success {true};

    //SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize. SDL error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //윈도우 생성
        if (gWindow = SDL_CreateWindow("SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, 0); gWindow == nullptr) {
            SDL_Log("Window could not be created. SDL error: %s\n", SDL_GetError());
            success = false;    
        }
        else {
            //윈도우 서페이스 가져오기
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

int helloSDL::LoadMedia()
{
    //파일 로드 플래그 변수
    bool success {true};

    //이미지 로드
    std::string imagePath {"images/meloon.png"};
    if (gHelloWorld = SDL_LoadPNG(imagePath.c_str()); gHelloWorld == nullptr) {
        SDL_Log("unable to load image %s SDL Error: %s\n", imagePath.c_str(), SDL_GetError());
        success = false;
    }

    return success;
}

void helloSDL::Close()
{
    //서페이스 비우기
    SDL_DestroySurface(gHelloWorld);
    gHelloWorld = nullptr;

    //윈도우 없애기
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gScreenSurface = nullptr;

    //SDL 서브시스템 종료
    SDL_Quit();
}
