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

#define MAX_PARTICLE_SIZE 3000

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
#define COLMSG_PLAYER_HIP_DROP		0x00000011
#define COLMSG_PLAYER_FALLING		0x00000012


//#define COLMSG_PLAYER_AIR_DIVING	0x0000002F
//#define COLMSG_PLAYER_AIR_DIVING	0x0000004F
//#define COLMSG_PLAYER_AIR_DIVING	0x0000008F
//#define COLMSG_PLAYER_AIR_DIVING	0x000000FF





#define COLMSG_WANDASH    0x00000001
#define COLMSG_WANCAPTURE 0x00000002
#define COLMSG_WANSTATE   0x00000004 
#define COLMSG_WANSTATE2   0x00000008





#endif // ENGINE_Defines_h__
