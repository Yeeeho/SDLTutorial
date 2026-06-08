#include "pch.h"

#include "externs.h"
#include "textures.h"

//렌더할 png 이미지
LTexture gPngTexture;

//스프라이트 시트
LTexture gSpriteSheetTexture;

//방향에 맞춰서 출력되는 이미지
LTexture gUpTexture, gDownTexture, gLeftTexture, gRightTexture;

LTexture::LTexture():
    //텍스처 변수를 초기화함, 초기화 리스트
    mTexture { nullptr },
    mWidth { 0 },
    mHeight { 0 }
    //상수가 아니라는 뜻같은데
{ }

LTexture::~LTexture()
{
    //텍스처 비우기
    Destroy();
}

bool LTexture::LoadFromFile(std::string path)
{
    //텍스처가 이미 존재한다면 비운다.
    Destroy();

    //서페이스를 로드한다.
    if ( SDL_Surface* loadedSurface = IMG_Load( path.c_str() ); loadedSurface == nullptr )
    {
        SDL_Log("unable to load image %s SDL Error: %s\n", path.c_str(), SDL_GetError());        
    }
    else {
        if (SDL_SetSurfaceColorKey(loadedSurface, true, SDL_MapSurfaceRGB(loadedSurface, 0xFF, 0xFF, 0xFF)) == false) {
            SDL_Log("Unable to color key, SDL Error: %s", SDL_GetError());
        }
        else {
            //서페이스에서 텍스처를 생성한다.
            mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            if (mTexture == nullptr) {
                SDL_Log("Unable to create texture from loaded pixels, SDL error: %s\n", SDL_GetError());      
            }
            else {
                //이미지 크기를 가져온다.
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }
    
            //로드된 서페이스를 치운다.
            SDL_DestroySurface(loadedSurface);
        }
    }

    //텍스처가 로드되었다면 참을 반환
    return mTexture != nullptr;
}

void LTexture::Destroy()
{
    //텍스처를 지운다.
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;

    mWidth = 0;
    mHeight = 0;
}

void LTexture::Render(float x, float y, SDL_FRect *clip, float width, float height)
{
    //텍스처 위치 설정
    SDL_FRect dstRect { x, y, static_cast<float>(mWidth), static_cast<float>(mHeight)};

    //클립이 주어졌을 때 클립의 크기가 기본이 됨
    if (clip != nullptr) {
        dstRect.w = clip->w;
        dstRect.h = clip->h;
    }

    //길이가 주어졌을 경우 크기를 바꿈
    if (width > 0) {
        dstRect.w = width;
    }
    if (height > 0) {
        dstRect.h = height;
    }

    //텍스처 렌더링
    SDL_RenderTexture(gRenderer, mTexture, clip, &dstRect);
}

void LTexture::RenderFlip(float x, float y, SDL_FRect *clip, float width, float height, double degrees, SDL_FPoint *center, SDL_FlipMode flipmode)
{
    //텍스처 위치 설정
    SDL_FRect dstRect { x, y, static_cast<float>(mWidth), static_cast<float>(mHeight)};

    //클립이 주어지면 클립 길이에 맞춤
    if (clip != nullptr) {
        dstRect.w = clip->w;
        dstRect.h = clip->h;
    }

    //새로운 길이가 주어지면 길이를 바꿈
    if (width > 0) {
        dstRect.w = width;
    }
    if (height > 0) {
        dstRect.h = height;
    }

    //텍스처를 렌더링함
    SDL_RenderTextureRotated(gRenderer, mTexture, clip, &dstRect, degrees, center, flipmode);
}

void LTexture::Render(float x, float y)
{
    //텍스처 위치 설정
    SDL_FRect dstRect { x, y, static_cast<float>(mWidth), static_cast<float>(mHeight) };

    //텍스처를 렌더링한다
    SDL_RenderTexture(gRenderer, mTexture, nullptr, &dstRect);
}

int LTexture::GetWidth()
{
    return mWidth;
}

int LTexture::GetHeight()
{
    return mHeight;
}

bool LTexture::IsLoaded()
{
    return mTexture != nullptr;
}

bool LTexture::Init()
{
    bool success { true };

    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize. SDL error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //윈도우와 렌더러 생성
        if(SDL_CreateWindowAndRenderer("SDL# Tutorial: Textures and Extenstion libs", kScreenWidth, kScreenHeight, 0, &gWindow, &gRenderer) == false) {
            SDL_Log("Window could not be created. SDL error: %s\n", SDL_GetError());
            success = false;    
        }
    }

    return success;
}

bool LTexture::LoadMedia()
{
    bool success { true };

    if (gSpriteSheetTexture.LoadFromFile("images/dots.png") == false) {
        SDL_Log("Unable to load png image\n");
        success = false;
    }

    //스플래시 이미지를 로드함
    if ( gUpTexture.LoadFromFile( "images/meloon.png") == false) {
        SDL_Log("Unable to load png image\n");
        success = false;
    }
    else if ( gDownTexture.LoadFromFile( "images/meloon2.png") == false) {
        SDL_Log("Unable to load png image\n");
        success = false;
    }
    else if ( gLeftTexture.LoadFromFile( "images/meloon3.png") == false) {
        SDL_Log("Unable to load png image\n");
        success = false;
    }
    else if ( gRightTexture.LoadFromFile( "images/meloon4.png") == false) {
        SDL_Log("Unable to load png image\n");
        success = false;
    }

    return success;
}

void LTexture::Close()
{
    //텍스처 비우기
    gPngTexture.Destroy();

    //윈도우 지우기
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    //SDL 서브시스템 종료
    SDL_Quit();
}

int LTexture::Loop()
{
    int exitCode {0};

    if (Init() == false) {
        SDL_Log("unable to init program\n");
        exitCode = 1;
    }
    else {
        //미디어를 로드함
        if (LoadMedia() == false) {
            SDL_Log("Unable to load media\n");
            exitCode = 2;
        }
        else {
            //종료 플래그
            bool quit {false};

            //이벤트 데이터 
            SDL_Event e;
            SDL_zero(e);

            //회전각도
            double degrees = 0.0;

            //회전유형
            SDL_FlipMode flipMode = SDL_FLIP_NONE;

            LTexture* currentTexture = &gUpTexture;

            //메인 루프
            while (quit == false) {
                //이벤트 데이터를 가져옴
                while (SDL_PollEvent(&e) == true) {
                    //이벤트가 종료 타입이라면 
                    if (e.type == SDL_EVENT_QUIT) {
                        //메인 루프를 종료함
                        quit = true;
                    }
                    else if( e.type == SDL_EVENT_KEY_DOWN )
                    {
                        //Set texture
                        if( e.key.key == SDLK_UP )
                        {
                            currentTexture = &gUpTexture;
                        }
                        else if( e.key.key == SDLK_DOWN )
                        {
                            currentTexture = &gDownTexture;
                        }

                        //왼쪽/ 오른쪽 방향키로 회전시킴.
                        else if( e.key.key == SDLK_LEFT )
                        {
                            degrees -= 36;
                        }
                        else if( e.key.key == SDLK_RIGHT )
                        {
                            degrees += 36;
                        }

                        //1,2,3 번 키를 눌러서 플립모드를 바꿈.
                        else if (e.key.key == SDLK_1) {
                            flipMode = SDL_FLIP_HORIZONTAL;
                        }
                        else if (e.key.key == SDLK_2) {
                            flipMode = SDL_FLIP_NONE;
                        }
                        else if (e.key.key == SDLK_3) {
                            flipMode = SDL_FLIP_VERTICAL;
                        }
                    }
                }
                
                //키 상태에 맞게 배경색을 바꾼다.
                const bool* keyStates = SDL_GetKeyboardState(nullptr);
                if (keyStates[SDL_SCANCODE_UP] == true) {
                    bgColor.r = 0xFF;
                    bgColor.g = 0x00;
                    bgColor.b = 0x00;
                }

                //배경을 흰색으로 채움
                SDL_SetRenderDrawColor(gRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
                SDL_RenderClear(gRenderer);

                //이미지의 중심을 모서리에서부터 정의함
                SDL_FPoint center {currentTexture->GetWidth() / 2.f, currentTexture->GetHeight() / 2.f};

                //뒤집히거나 회전한 텍스처를 그림
                currentTexture->RenderFlip((kScreenWidth - currentTexture->GetWidth()) / 2.f, (kScreenHeight - currentTexture->GetHeight()) / 2.f,
                                            nullptr, kOriginalSize, kOriginalSize, degrees, &center, flipMode);

                                            
                //스프라이트 클립을 초기화함
                constexpr float kSpriteSize = 100.f;
                SDL_FRect spriteClip{ 0.f, 0.f, kSpriteSize, kSpriteSize};
                
                //스프라이트 크기 초기화
                SDL_FRect spriteSize{ 0.f, 0.f, kSpriteSize, kSpriteSize};

                //좌측상단 스프라이트 
                spriteClip.x = 0.f;
                spriteClip.y = 0.f;

                //스프라이트를 원래 크기로 정함
                spriteSize.w = kSpriteSize;
                spriteSize.h = kSpriteSize;

                //원래 크기의 스프라이트를 그림.
                gSpriteSheetTexture.Render(0.f, 0.f, &spriteClip, spriteSize.w, spriteSize.h);

                //우측상단 스프라이트
                spriteClip.x = kSpriteSize;
                spriteClip.y = 0.f;

                //스프라이트를 절반 크기로 줄임
                spriteSize.w = kSpriteSize * 0.5f;
                spriteSize.h = kSpriteSize * 0.5f;

                //절반 크기의 스프라이트를 그림
                gSpriteSheetTexture.Render(kScreenWidth - spriteSize.w, 0.f, &spriteClip, spriteSize.w, spriteSize.h);

                //좌측하단 스프라이트
                spriteClip.x = 0.f;
                spriteClip.y = kSpriteSize;

                //스프라이트를 두배로 키움
                spriteSize.w = kSpriteSize * 2.f;
                spriteSize.h = kSpriteSize * 2.f;

                //두배로 키운 스프라이트 그림
                gSpriteSheetTexture.Render(0.f , kScreenHeight - spriteSize.h, &spriteClip, spriteSize.w, spriteSize.h);

                //우측하단 스프라이트
                spriteClip.x = kSpriteSize;
                spriteClip.y = kSpriteSize;

                //세로로 스프라이트를 짜부시킴
                spriteSize.w = kSpriteSize;
                spriteSize.h = kSpriteSize * 0.5f;

                //짜부시킨 스프라이트 그리기
                gSpriteSheetTexture.Render(kScreenWidth - spriteSize.w, kScreenHeight - spriteSize.h, &spriteClip, spriteSize.w, spriteSize.h);

                //화면에 이미지를 렌더링함, 회전하지 않는 버전.
                // currentTexture->Render((kScreenWidth - currentTexture->GetWidth()) / 2.f, (kScreenHeight - currentTexture->GetHeight()) / 2.f);


                //화면을 업데이트함
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    Close();

    return exitCode;
}
