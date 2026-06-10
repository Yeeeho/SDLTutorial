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

    //SDL_ttf 라이브러리가 포함되었는지 확인함
    #if defined (SDL_TTF_MAJOR_VERSION)
    //텍스트에서 텍스처 생성하기
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
    #endif

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

class LButton {
    public:
        //버튼 크기
        static constexpr int kButtonWidth = 300;
        static constexpr int kButtonHeight = 200;

        LButton();

        //좌측 상단 위치 설정
        void SetPosition(float x, float y);

        //마우스 이벤트 관리
        void HandleEvent(SDL_Event *e);

        //버튼 스프라이트 표시
        void Render();

    private:
        enum class eButtonSprite {
            MouseOut = 0,
            MouseOverMotion = 1,
            MouseDown = 2,
            MouseUp = 3
        };

        //좌측 상단 위치
        SDL_FPoint mPosition;

        //현재 사용된 전역 스프라이트
        eButtonSprite mCurrentSprite;
};

class LTimer {
    public:
        //생성자
        LTimer();

        //시계 동작
        void Start();
        void Stop();
        void Pause();
        void Unpause();

        //타이머의 시간을 가져옴
        Uint64 GetTicksNS();

        //타이머의 상태를 확인함
        bool isStarted();
        bool isPaused();

    private:
        //타이머가 시작했을 때 시간
        Uint64 mStartTicks;

        //타이머가 일시정지했을 때 저장된 시간
        Uint64 mPausedTicks;

        //타이머 상태
        bool mPaused;
        bool mStarted;
};

class Dot {
    public:
        //점의 크기
        static constexpr int kDotWidth = 20;
        static constexpr int kDotHeight = 20;

        //점의 최대 축 속력
        static constexpr int kDotVel = 10;

        //생성자
        Dot();

        //버튼이 눌리는걸 감지하고 점의 속력을 조절함
        void HandleEvent(SDL_Event &e);

        //점을 움직임
        void Move();

        //화면에 점을 보여줌
        void Render();
    private:
        //점의 x,y축의 오프셋
        int mPosX, mPosY;

        //점의 속력
        int mVelX, mVelY;
};