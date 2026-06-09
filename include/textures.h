# pragma once

#include <string>

#include <SDL3/SDL_render.h>

class LTexture {
    public:

    //기호 상수
    static constexpr float kOriginalSize = -1.f;

    //텍스처 변수 초기화
    LTexture();

    //텍스처 변수 치우기
    ~LTexture();

    //디스크에서 텍스처 불러오기
    bool LoadFromFile(std::string path);

    //텍스처 지우기
    void Destroy();

    //컬러 모듈레이션 설정
    void SetColor(Uint8 r, Uint8 g, Uint8 b);

    //알파값을 설정
    void SetOpacity(Uint8 alpha);

    //블렌드 모드 설정
    void SetBlendMode(SDL_BlendMode blendMode);

    //텍스처 그리기
    void Render(float x, float y);
    void Render(float x, float y, SDL_FRect* clip, float width, float height);
    void RenderFlip(float x, float y, SDL_FRect* clip = nullptr, float width = kOriginalSize, float height = kOriginalSize,
                double degrees = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flipmode = SDL_FLIP_NONE);

    //텍스처 속성 가져오기
    int GetWidth();
    int GetHeight();
    bool IsLoaded();

    int Loop();

    bool Init();
    bool LoadMedia();
    void Close();
    private:
    //텍스처 데이터를 담고있음
    SDL_Texture* mTexture;

    //텍스처 크기
    int mWidth;
    int mHeight;

};

//색 상수
constexpr int kColorMagnitudeCount = 3;
constexpr Uint8 kColorMagnitudes[kColorMagnitudeCount] = {0x00, 0x7F, 0xFF};

enum class eColorChannel {
    TextureRed = 0,
    TextureGreen = 1,
    TextureBlue = 2,
    TextureAlpha = 3,

    BackGroundRed = 4,
    BackGroundGreen = 5,
    BackGroundBlue = 6,

    Total = 7,
    Unknown = 8
};
