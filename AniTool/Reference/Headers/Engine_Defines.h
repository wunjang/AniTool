#ifndef ENGINE_Defines_h__
#define ENGINE_Defines_h__

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <process.h>
#include <unordered_map>
#include <fstream>
#include <io.h>
#include <atlconv.h>
#include <tchar.h>
#pragma warning(disable : 4251)

#define	DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include <iostream>
using namespace std;

#include "ShaderTable.h"
#include "Engine_Typedef.h"
#include "Engine_Macro.h"

#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"
//////////////그래픽관련/////////////////////////////
#define MAX_LUMINANCE 6

#define MAX_PARTICLE_SIZE 1000

#define MAX_GODRAY_COUNT 5
/////////////충돌을 위한 것들////////////////////////

#define COL_SPHERE					0x00000001
#define COL_BOX						0x00000002
#define COL_SPHEREBOX				0x00000004
//플레이어 충돌상태
#define COLMSG_PLAYER_IDLE			0x00000001
#define COLMSG_PLAYER_MOVE			0x00000002
#define COLMSG_PLAYER_JUMP			0x00000004 
#define COLMSG_PLAYER_AIR_DIVING	0x00000008
#define COLMSG_PLAYER_THROW_CAP		0x00000010
#define COLMSG_PLAYER_HIP_DROP		0x00000020
#define COLMSG_PLAYER_FALLING		0x00000040
#define COLMSG_PLAYER_JUMPWANWAN	0x00000080
#define COLMSG_PLAYER_ESCAPECAPTURE	0x00000100
#define COLMSG_PLAYER_GET_ON_JAXI	0x00000200

//#define COLMSG_PLAYER_AIR_DIVING	0x0000002F
//#define COLMSG_PLAYER_AIR_DIVING	0x0000004F
//#define COLMSG_PLAYER_AIR_DIVING	0x0000008F
//#define COLMSG_PLAYER_AIR_DIVING	0x000000FF


#define COLMSG_MONSTER_STUN				0x80000000
#define COLMSG_MONSTER_ATTACK			0x40000000

// 멍멍이
#define COLMSG_WANWAN_DASH				0x00000001
#define COLMSG_WANWAN_CAPTURE			0x00000002
#define COLMSG_WANWAN_ISATTACKED		0x00000004

// 브리다
#define COLMSG_BREEDA_IDLE				0x00000002
#define COLMSG_BREEDA_PULLING			0x00000008
#define COLMSG_BREEDA_ATTACK			0x00000010

// 굼바
#define COLMSG_KURIBO_IDLE				0x00000001
#define COLMSG_KURIBO_DIE				0x00000002

// Popn
#define COLMSG_POPN_IDLE				0x00000001

// Mummy
#define COLMSG_MUMMY_IDLE				0x00000001
#define COLMSG_MUMMY_ATKED				0x00000002
#define COLMSG_MUMMY_DIE				0x00000004

// 2D BRICK
#define COLMSG_BRICK_IDLE				0x00000001
// 2D MARIO
#define COLMSG_MARIO2D_IDLE				0x00000001
#define COLMSG_MARIO2D_JUMP				0x00000002

// Trempoline
#define COLMSG_TREMPOLINE_IDLE			0x00000001
#define COLMSG_TREMPOLINE_MOVE			0x00000002

// Dokan
#define COLMSG_PIPE_GOTO_3D				0x00000001
#define COLMSG_PIPE_GOTO_2D				0x00000002
#define COLMSG_PIPE_GOTO_HOME			0x00000004
#define COLMSG_PIPE_GOTO_PYRAMID		0x00000008

// BombTailBomb
#define COLMSG_BOMBTAILBOMB_ENEMY		0x00000001
#define COLMSG_BOMBTAILBOMB_REFLECT		0x00000002

// BombTail
#define COLMSG_BOMBTAIL_DEFENSELESS		0x00000001
#define COLMSG_BOMBTAIL_DEFENSE			0x00000002
#define COLMSG_BOMBTAIL_WEAK			0x00000004

// Corner
#define COLMSG_CORNER_EXIT2D			0x00000001
#define COLMSG_CORNER_CIRCLE			0x00000002

// BossKnuckle
#define COLMSG_BOSSKNUCKLE_PUNCH		0x00000001





#define WINCX 1280
#define WINCY 720
#endif // ENGINE_Defines_h__
