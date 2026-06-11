#include "pch.h"

#include "textures.h"
#include "state_ex.h"

//SDL을 시작하고 윈도우 생성
bool Init();

bool LoadMedia();

void Close();

//충돌 검사
bool CheckCollision(SDL_Rect a, SDL_Rect b);

//상태 매니저
void SetNextState(GameState* nextState);
bool ChangeState();

/*전역 변수들*/
//전역 에셋
LTexture gDot2Texture;