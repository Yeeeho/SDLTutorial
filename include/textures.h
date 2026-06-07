# pragma once

#include <string>

#include <SDL3/SDL_render.h>

class LTexture {
    public:
    //텍스처 변수 초기화
    LTexture();

    //텍스처 변수 치우기
    ~LTexture();

    //디스크에서 텍스처 불러오기
    bool LoadFromFile(std::string path);

    //텍스처 지우기
    void Destroy();

    //텍스처 그리기
    void Render(float x, float y);

    //텍스처 속성 가져오기
    int GetWidth();
    int GetHeight();
    bool IsLoaded();

    private:
    //텍스처 데이터를 담고있음
    SDL_Texture* mTexture;

    //텍스처 크기
    int mWidth;
    int mHeight;
};