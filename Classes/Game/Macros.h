/********************************************************************
	date  :	2016/01/30   15:35

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

/**
 *	全局宏定义
 */
#ifndef MACROS_H
#define MACROS_H

/*Debug开关*/
#ifndef GAME_DEBUG
//#define GAME_DEBUG
#endif // !GAME_DEBUG

/*定义最大浮点数*/
#define MAX_NUMBER 65535.0f

/*公用顶点Shader*/
#define VSH_SHADER_PATH "shader/common.vert"
/*光照用Shader*/
#define FSH_SHADER_PATH "shader/light.frag"
/*地图用Shader*/
#define FSH_MAP_SHADER_PATH "shader/map_mask.frag"

/*光精灵粒子文件*/
#define PARTICLE_LIGHT_SPRITE "particle/light.plist"
/*暗精灵粒子文件*/
#define PARTICLE_DARK_SPRITE "particle/dark.plist"

/*光精灵标准光照半径*/
#define LIGHT_STANDRAD_RADIUS 500.0f
/*光精灵的最大生命值*/
#define LIGHT_SPRITE_MAX_HP 200.0f

/*光精灵最大速度限制*/
#define LIGHT_SPRITE_RATE_LIMIT 200.0f
/*暗精灵最大速度限制*/
#define DARK_SPRITE_RATE_LIMIT 100.0f

/*暗精灵标准生命值，根据实际尺寸有调整*/
#define DARK_SPRITE_STANDARD_HP 100.0f

/*刚体碰撞测试掩码*/
#define BODY_CONTACT_TEST_BITMARK 0x0001
#define DARK_BODY_CONTACT_TEST_BITMARK 0x0010

/*地图里静态的墙的名称下界*/
#define WALL_NAME_MIN 0
/*机关名称的下界*/
#define MECHANISM_NAME_MIN 100
/*触发开关碰撞掩码，与光精灵发生碰撞反应*/
#define KEY_COLLISION_BITMASK 0x00000001

/*GlobalZOrder全局渲染层级定义*/
#define GLOBAL_Z_ORDER_JOYSTICK 13
#define GLOBAL_Z_ORDER_DARK 12
#define GLOBAL_Z_ORDER_LIGHT_PARTICLE 11
#define GLOBAL_Z_ORDER_FRAGMENT 10

/*结点Tag*/
#define TAG_DARK_SPRITE 1
#define TAG_LIGHT_SPRITE 2
#define TAG_LIGHT_FRAGMENT 4
#define TAG_MECHANISM 8

#endif // !MACROS_H