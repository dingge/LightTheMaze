/********************************************************************
	date  :	2016/01/30   15:35

	author:	TLeaves
	
	e-mail: 664570530@qq.com
	
	github: https://github.com/TLeaves
*********************************************************************/

/**
 *	ȫ�ֺ궨��
 */
#ifndef MACROS_H
#define MACROS_H

/*Debug����*/
#ifndef GAME_DEBUG
//#define GAME_DEBUG
#endif // !GAME_DEBUG

/*������󸡵���*/
#define MAX_NUMBER 65535.0f

/*���ö���Shader*/
#define VSH_SHADER_PATH "shader/common.vert"
/*������Shader*/
#define FSH_SHADER_PATH "shader/light.frag"
/*��ͼ��Shader*/
#define FSH_MAP_SHADER_PATH "shader/map_mask.frag"

/*�⾫�������ļ�*/
#define PARTICLE_LIGHT_SPRITE "particle/light.plist"
/*�����������ļ�*/
#define PARTICLE_DARK_SPRITE "particle/dark.plist"

/*�⾫���׼���հ뾶*/
#define LIGHT_STANDRAD_RADIUS 500.0f
/*�⾫����������ֵ*/
#define LIGHT_SPRITE_MAX_HP 200.0f

/*�⾫������ٶ�����*/
#define LIGHT_SPRITE_RATE_LIMIT 200.0f
/*����������ٶ�����*/
#define DARK_SPRITE_RATE_LIMIT 100.0f

/*�������׼����ֵ������ʵ�ʳߴ��е���*/
#define DARK_SPRITE_STANDARD_HP 100.0f

/*������ײ��������*/
#define BODY_CONTACT_TEST_BITMARK 0x0001
#define DARK_BODY_CONTACT_TEST_BITMARK 0x0010

/*��ͼ�ﾲ̬��ǽ�������½�*/
#define WALL_NAME_MIN 0
/*�������Ƶ��½�*/
#define MECHANISM_NAME_MIN 100
/*����������ײ���룬��⾫�鷢����ײ��Ӧ*/
#define KEY_COLLISION_BITMASK 0x00000001

/*GlobalZOrderȫ����Ⱦ�㼶����*/
#define GLOBAL_Z_ORDER_JOYSTICK 13
#define GLOBAL_Z_ORDER_DARK 12
#define GLOBAL_Z_ORDER_LIGHT_PARTICLE 11
#define GLOBAL_Z_ORDER_FRAGMENT 10

/*���Tag*/
#define TAG_DARK_SPRITE 1
#define TAG_LIGHT_SPRITE 2
#define TAG_LIGHT_FRAGMENT 4
#define TAG_MECHANISM 8

#endif // !MACROS_H