#include "pch.h"

#include "externs.h"
#include "textures.h"

//렌더할 png 이미지
LTexture gPngTexture;

//타이머 텍스트용 텍스처
LTexture gTimeTextTexture;

//텍스트용 텍스처
LTexture gTextTexture;

//버튼용 텍스처
LTexture gButtonSpriteTexture;

//스프라이트 시트
LTexture gSpriteSheetTexture;

//블렌딩과 모듈레이션을 적용할 텍스처
LTexture gColorsTexture;

TTF_Font* gFont{ nullptr };

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

#if defined (SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    //이미 존재하는 텍스처를 지움
    Destroy();    

    //텍스처 서페이스를 로드함
    SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), 0, textColor);
    if (textSurface == nullptr) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError());
    }
    else {
        //서페이스로부터 텍스처를 생성함
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == nullptr) {
            SDL_Log("Unable to create texture from rendered text, SDL Error: %s", SDL_GetError());
        }
        else {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //임시 서페이스를 해제함
        SDL_DestroySurface(textSurface);
    }

    //텍스처가 로드되면 성공을 리턴함
    return mTexture != nullptr;
}
#endif

void LTexture::Destroy()
{
    //텍스처를 지운다.
    SDL_DestroyTexture(mTexture);
    mTexture = nullptr;

    mWidth = 0;
    mHeight = 0;
}

void LTexture::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetTextureColorMod(mTexture, r, g, b);
}

void LTexture::SetOpacity(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::SetBlendMode(SDL_BlendMode blendMode)
{
    SDL_SetTextureBlendMode(mTexture, blendMode);
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
        else {
            //폰트 초기화
            if (TTF_Init() == false) {
                SDL_Log("SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError());
                success = false;
            }
        }
    }

    return success;
}

bool LTexture::LoadMedia()
{
    bool success { true };

    //씬 폰트를 로드함
    std::string fontPath{ "fonts/lazy.ttf" };
    gFont = TTF_OpenFont(fontPath.c_str(), 28);
    if (gFont == nullptr) {
        SDL_Log("Could not load %s, SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError());
        success = false;
    }
    else {
        //텍스트 로드
        SDL_Color textColor { 0x00, 0x00, 0x00, 0xFF };
        if (gTextTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor) == false) {
            SDL_Log("Could not load text texture %s! SDL_ttf Error: %s", fontPath.c_str(), SDL_GetError());
            success = false;
        }
    }

    if (gButtonSpriteTexture.LoadFromFile("images/button.png") == false) {
    SDL_Log("Unable to load png image\n");
        success = false;
    }

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

    //폰트 해제
    TTF_CloseFont(gFont);
    gFont = nullptr;

    //윈도우 지우기
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    //SDL 서브시스템 종료
    SDL_Quit();
    TTF_Quit();
}

int LTexture::Loop()
{
    int exitCode {0};

    //앱 타이머
    LTimer timer;

    //타이머 시작 시간
    Uint64 startTime = 0;

    //텍스트 스트림
    std::stringstream timeText;

    //버튼 위치시킴
    constexpr int kButtonCount = 4;
    LButton buttons[kButtonCount];
    buttons[0].SetPosition(0, 0);
    buttons[1].SetPosition(kScreenWidth - LButton::kButtonWidth, 0);
    buttons[2].SetPosition(0, kScreenHeight - LButton::kButtonHeight);
    buttons[3].SetPosition(kScreenWidth - LButton::kButtonWidth, kScreenHeight - LButton::kButtonHeight);

    //색 초기화
    Uint8 colorChannelsIndices[static_cast<int>(eColorChannel::Total)];
    colorChannelsIndices[static_cast<int>(eColorChannel::TextureRed)] = 2;
    colorChannelsIndices[static_cast<int>(eColorChannel::TextureGreen)] = 2;
    colorChannelsIndices[static_cast<int>(eColorChannel::TextureBlue)] = 2;
    colorChannelsIndices[static_cast<int>(eColorChannel::TextureAlpha)] = 2;

    colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundRed)] = 2;
    colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundGreen)] = 2;
    colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundBlue)] = 2;
    
    //블렌딩 초기화
    gColorsTexture.SetBlendMode(SDL_BLENDMODE_BLEND);

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

                    //엔터 키를 누르면 시간이 리셋됨
                    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RETURN) {
                        //새 시작 시간을 설정함
                        startTime = SDL_GetTicks();
                    }

                    //버튼 이벤트 핸들러
                    for (int i = 0; i < kButtonCount; i++) {
                        buttons[i].HandleEvent(&e);
                    }

                    if( e.type == SDL_EVENT_KEY_DOWN )
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

                        eColorChannel channelToUpdate = eColorChannel::Unknown;
                        if (e.key.key == SDLK_A) {
                            channelToUpdate = eColorChannel::TextureRed;
                        }
                        else if (e.key.key == SDLK_S) {
                            channelToUpdate = eColorChannel::TextureGreen;
                        }
                        else if (e.key.key == SDLK_D) {
                            channelToUpdate = eColorChannel::TextureBlue;
                        }
                        else if (e.key.key == SDLK_F) {
                            channelToUpdate = eColorChannel::TextureAlpha;
                        }

                        else if (e.key.key == SDLK_Q) {
                            channelToUpdate = eColorChannel::BackGroundRed;
                        }
                        else if (e.key.key == SDLK_W) {
                            channelToUpdate = eColorChannel::BackGroundGreen;
                        }
                        else if (e.key.key == SDLK_E) {
                            channelToUpdate = eColorChannel::BackGroundBlue;
                        }

                        if (channelToUpdate != eColorChannel::Unknown) {
                            //채널 값들을 순회
                            colorChannelsIndices[static_cast<int>(channelToUpdate)]++;
                            if (colorChannelsIndices[static_cast<int>(channelToUpdate)] >= kColorMagnitudeCount) {
                                colorChannelsIndices[static_cast<int>(channelToUpdate)] = 0;
                            }

                            //색 값을 콘솔에 출력
                            SDL_Log("Texture- R:%d, G:%d, B:%d, A:%d | Background- R:%d, G:%d, B:%d",
                                kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureRed)]],
                                kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureGreen)]],
                                kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureBlue)]],
                                kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureAlpha)]],
                                kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundRed)]],
                                kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundGreen)]],
                                kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundBlue)]]
                            );
                        }
                    }
                }
                
                //타이머가 시작되었다면
                if (startTime != 0) {
                    //텍스트 업데이트
                    timeText.str(""); //스트링 스트림을 빈 문자열로 초기화
                    timeText << "milliseconds since last time" << SDL_GetTicks() - startTime;
                    SDL_Color textColor {0x00, 0x00, 0x00, 0xFF};
                    gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor);
                }

                //키 상태에 맞게 배경색을 바꾼다.
                const bool* keyStates = SDL_GetKeyboardState(nullptr);
                if (keyStates[SDL_SCANCODE_UP] == true) {
                    bgColor.r = 0xFF;
                    bgColor.g = 0x00;
                    bgColor.b = 0x00;
                }

                //배경색을 채움
                SDL_SetRenderDrawColor(gRenderer,
                    kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundRed)]], 
                    kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundGreen)]], 
                    kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::BackGroundBlue)]], 
                    0xFF);
                SDL_RenderClear(gRenderer);
                
                //텍스트 렌더링
                gTextTexture.Render((kScreenWidth - gTextTexture.GetWidth()) / 2.f, (kScreenHeight - gTextTexture.GetHeight()) / 2.f);
                
                //버튼 렌더링
                buttons[2].Render();
                buttons[3].Render();
                
                //타이머 텍스트 렌더링
                gTimeTextTexture.Render(0.f, (gTimeTextTexture.GetHeight()));

                //색깔 텍스처의 색을 설정하고 렌더링함
                currentTexture->SetColor(
                    kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureRed)]],
                    kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureGreen)]],
                    kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureBlue)]]
                );
                currentTexture->SetOpacity(kColorMagnitudes[colorChannelsIndices[static_cast<int>(eColorChannel::TextureAlpha)]]);

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

LButton::LButton(): 
    mPosition{0.f, 0.f},
    mCurrentSprite{eButtonSprite::MouseOut}
{
}

void LButton::SetPosition(float x, float y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::HandleEvent(SDL_Event *e)
{
    //마우스 이벤트가 발생했다면
    if (e->type == SDL_EVENT_MOUSE_MOTION ||
        e->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
        e->type == SDL_EVENT_MOUSE_BUTTON_UP
    ) {
        //마우스 위치를 가져온다
        float x = -1.f, y = -1.f;
        SDL_GetMouseState(&x, &y);

        //마우스가 버튼 안에 있는지 확인하는 플래그
        //모든 조건에 충족되지 않으면 참
        bool inside = true;

        if (x < mPosition.x) {
            inside = false;
        }
        else if (x > mPosition.x + kButtonWidth) {
            inside = false;
        }
        else if (y < mPosition.y) {
            inside = false;
        }
        else if (y > mPosition.y + kButtonHeight) {
            inside = false;
        }

        //마우스가 밖에 있을 때
        if (!inside) {
            mCurrentSprite = eButtonSprite::MouseOut;
        }
        //마우스가 안에 있을 때
        else {
            //마우스 오버 스프라이트 설정
            switch(e->type) {
                case SDL_EVENT_MOUSE_MOTION:
                    mCurrentSprite = eButtonSprite::MouseOverMotion;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    mCurrentSprite = eButtonSprite::MouseDown;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    mCurrentSprite = eButtonSprite::MouseUp;
                    break;
            }
        }
    }
}

void LButton::Render()
{
    //스프라이트 정의
    SDL_FRect spriteClips[] = {
        {0.f, 0 * kButtonHeight, kButtonWidth, kButtonHeight},
        {0.f, 1 * kButtonHeight, kButtonWidth, kButtonHeight},
        {0.f, 2 * kButtonHeight, kButtonWidth, kButtonHeight},
        {0.f, 3 * kButtonHeight, kButtonWidth, kButtonHeight}
    };

    gButtonSpriteTexture.RenderFlip(mPosition.x, mPosition.y, &spriteClips[static_cast<int>(mCurrentSprite)]);
}

LTimer::LTimer() :
    mStartTicks{ 0 },
    mPausedTicks{ 0 },

    mPaused{ false },
    mStarted{ false }
{
}

void LTimer::Start()
{
    //타이머를 시작
    mStarted = true;

    //타이머의 일시정지 해제
    mPaused = false;

    //현재 시간을 가져옴
    mStartTicks = SDL_GetTicksNS();
    mPausedTicks = 0;
}

void LTimer::Stop()
{
    //타이머를 멈춤
    mStarted = false;

    //타이머의 일시정지 해제
    mPaused = false;

    //시간 변수들을 0으로 돌림
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::Pause()
{
    //타이머가 돌아가고 있고 멈추지 않았다면
    if (mStarted && !mPaused) {
        //타이머를 일시정지함
        mPaused = true;

        //일시정지 시간을 계산
        mPausedTicks = SDL_GetTicksNS() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::Unpause()
{
    //타이머가 돌아가는 중이고 일시정지 상태라면
    if (mStarted && mPaused) {
        //타이머 일시정지 해제
        mPaused = false;

        //시작 시간을 리셋함
        mStartTicks = SDL_GetTicksNS() - mPausedTicks;

        //일시정지 시간을 리셋함
        mPausedTicks = 0;
    }
}

Uint64 LTimer::GetTicksNS()
{
    //실제 타이머 시간
    Uint64 time{0};

    //타이머가 돌아가고 있을 때
    if (mStarted) {
        //타이머가 일시정지 했을 때
        if (mPaused) {
            //타이머가 언제 일시정지 되었는지를 반환함
            time = mPausedTicks;
        }
        else {
            //현재 시간에서 시작 시간을 뺀 값을 반환함
            time = SDL_GetTicksNS() - mStartTicks;
        }
    }

    return time;
}
