#include "pch.h"

#include "textures.h"

/*전역 변수*/
//렌더링 할 윈도우
SDL_Window* gWindow {nullptr};

//윈도우를 렌더링할 렌더러
SDL_Renderer *gRenderer{nullptr};

//렌더할 png 이미지
LTexture gPngTexture;

LTexture::LTexture():
    //텍스처 변수를 초기화함
    mTexture { nullptr },
    mWidth { 0 },
    mHeight { 0 }
{
}

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

    //텍스처가 로드되었다면 참을 반환
    return mTexture != nullptr;
}
