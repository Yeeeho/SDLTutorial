#pragma once

#include <SDL3/SDL.h>
#include <textures.h>

class Dot2 {
    public:
        //점의 크기
        static constexpr int kDotWidth = 20;
        static constexpr int kDotHeight = 20;

        //점의 최대 축속도
        static constexpr int kDotVel = 10;

        //생성자
        Dot2();

        //위치 설정
        void SetPos(int x, int y);

        //키가 눌리는 것을 감지하고 점의 속도를 조절함
        void HandleEvent(SDL_Event &e);

        //점을 움직임
        void Move(int levelWidth, int levelHeight);

        //점을 화면에 표시함
        void Render(SDL_Rect camera);

        //콜리전 박스 접근자
        SDL_Rect GetCollider();

    private:
        //점의 위치/크기
        SDL_Rect mCollisionBox;

        //점의 속도
        int mVelX, mVelY;
};

class House {
    public:
        //집의 크기
        static constexpr int kHouseWidth = 40;
        static constexpr int kHouseHeight = 40;

        //생성자
        House();

        //집의 위치/그래픽 설정
        void Set(int x, int y, LTexture* houseTexture);

        //집을 카메라에 상대적으로 렌더링함
        void Render(SDL_Rect camera);

        //콜리전 박스를 가져옴
        SDL_Rect GetCollider();
    
    private:
        //집의 그래픽
        LTexture* mHouseTexture;

        //집의 위치/크기
        SDL_Rect mCollisionBox;
};

class Door {
    public:
        //문의 크기
        static constexpr int kDoorWidth = 20;
        static constexpr int kDoorHeight = 40;

        //생성자
        Door();

        //문의 위치 설정
        void Set(int x, int y);

        //문을 보여줌
        void Render();

        //콜리전 박스를 가져옴
        SDL_Rect GetCollider();
    
    private:
        //문의 위치/크기
        SDL_Rect mCollisionBox;
};

class GameState {
    public:

        static const int kScreenWidth{640};
        static const int kScreenHeight{480};

        //상태 변환
        virtual bool Enter() = 0;
        virtual bool Exit() = 0;

        //메인 루프 함수
        virtual void HandleEvent(SDL_Event &e) = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;

        //자식 파괴자를 부른다.
        virtual ~GameState() = default;
};

class IntroState : public GameState {
    public:
        //정적 접근자
        static IntroState* Get();
        
        //변환
        bool Enter() override;
        bool Exit() override;

        //메인 루프 함수
        void HandleEvent(SDL_Event &e) override;
        void Update() override;
        void Render() override;

    private:
        //정적 인스턴스
        static IntroState sIntroState;

        //프라이빗 생성자
        IntroState();

        //인트로 배경
        LTexture mBackGroundTexture;

        //인트로 메시지
        LTexture mMessageTexture;
};

class TitleState : public GameState {
    public:
        //정적 접근자
        static TitleState* Get();

        //변환
        bool Enter() override;
        bool Exit() override;

        //메인 루프 함수
        void HandleEvent(SDL_Event &e) override;
        void Update() override;
        void Render() override;
        
    private:
        //정적 인스턴스
        static TitleState sTitleState;

        //프라이빗 생성자
        TitleState();

        //인트로 배경
        LTexture mBackGroundTexture;

        //인트로 메시지
        LTexture mMessageTexture;
};

class OverWorldState : public GameState {
    public:
        //정적 접근자
        static OverWorldState* Get();

        //변환
        bool Enter() override;
        bool Exit() override;

        //메인 루프 함수
        void HandleEvent(SDL_Event &e) override;
        void Update() override;
        void Render() override;

    private:
        //레벨 크기
        static constexpr int kLevelWidth = kScreenWidth * 2;
        static constexpr int kLevelHeight = kScreenHeight * 2;

        //정적 인스턴스
        static OverWorldState sOverWorldState;

        //프라이빗 생성자
        OverWorldState();

        //오버월드 텍스처들
        LTexture mBackGroundTexture;
        LTexture mRedHouseTexture;
        LTexture mBlueHouseTexture;

        //게임 오브젝트
        House mRedHouse;
        House mBlueHouse;
};

class RedRoomState: public GameState {
    public:
        //정적 접근자
        static RedRoomState* Get();
        //변환
        bool Enter() override;
        bool Exit() override;

        //메인 루프 함수
        void HandleEvent(SDL_Event &e) override;
        void Update() override;
        void Render() override;

    private:
        //레벨 크기
        static constexpr int kLevelWidth = kScreenWidth;
        static constexpr int kLevelHeight = kScreenHeight;

        //정적 인스턴스
        static RedRoomState sRedRoomState;

        //프라이빗 생성자
        RedRoomState();

        //방 텍스처
        LTexture mBackGroundTexture;
        
        //게임 오브젝트
        Door mExitDoor;
};

class BlueRoomState: public GameState {
    public:
        //정적 접근자
        static BlueRoomState* Get();
        //변환
        bool Enter() override;
        bool Exit() override;

        //메인 루프 함수
        void HandleEvent(SDL_Event &e) override;
        void Update() override;
        void Render() override;

    private:
        //레벨 크기
        static constexpr int kLevelWidth = kScreenWidth;
        static constexpr int kLevelHeight = kScreenHeight;

        //정적 인스턴스
        static BlueRoomState sBlueRoomState;

        //프라이빗 생성자
        BlueRoomState();

        //방 텍스처
        LTexture mBackGroundTexture;
        
        //게임 오브젝트
        Door mExitDoor;
};

class ExitState : public GameState {
    public:
        //정적 접근자
        static ExitState* Get();

        //변환
        bool Enter() override;
        bool Exit() override;

        //메인 루프 함수
        void HandleEvent(SDL_Event &e) override;
        void Update() override;
        void Render() override;

    private:
        //정적 인스턴스
        static ExitState sExitState;

        //프라이빗 생성자
        ExitState();
};