#pragma once

// UI Part/Page�� ���� ��� �� �޸� ���� ������ ���� �̳� ���
// �ʿ��� Ŭ������ ��� ��� ���� (
// �� ���� Client_Defines�� ���� �� ���� �ʿ�

	enum class MOVETYPE
	{
		TYPE_STATIC, // �������� ����
		TYPE_MOVE, // ������
		TYPE_BAR, // �� ���·� ǥ�� (HP�� ��)
		TYPE_END
	};

	enum class PAGEACTION
	{
		ACTION_INACTIVE,
		ACTION_ACTIVE,
		ACTION_OPENING,
		ACTION_CLOSING,
		ACTION_SHAKING,
		ACTION_END
	};

	enum class UIPAGE
	{
		PAGE_MAIN,
		PAGE_LOADING,
		PAGE_PLAY,
		PAGE_MENU,
		PAGE_INVEN,
		PAGE_EQUIP,
		PAGE_STAT,
		PAGE_LEVELUP,
		PAGE_SKILL,
		PAGE_TOOLTIP,
		PAGE_ORTHO,
		PAGE_END
	};


	enum class UI_FONT
	{
		FONT_NORMAL_8,
		FONT_NORMAL_12,
		FONT_NORMAL_16,
		FONT_NORMAL_24,
		FONT_NORMAL_32,
		FONT_NORMAL_40,
		FONT_NORMAL_48,
		FONT_BOLD_8,
		FONT_BOLD_12,
		FONT_BOLD_16,
		FONT_BOLD_24,
		FONT_BOLD_32,
		FONT_BOLD_40,
		FONT_BOLD_48,
		FONT_END
	};


	enum class TEST_PAGE_VALUE_TYPE
	{
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_VEC2,
		TYPE_VEC3,
		TYPE_VEC4,
		TYPE_END
	};


	enum class TEST_PAGE_NAME
	{
		NAME_TEAMJANG,
		NAME_MAP,
		NAME_EFFECT,
		NAME_ANIM,
		NAME_PLAYER_STAT,
		NAME_ITEM,
		NAME_END
	};

	enum class UI_ORTHO_OBJ_TYPE
	{
		ORTHO_NORMAL_MONSTER,
		ORTHO_ELITE_MONSTER,
		ORTHO_BOSS_0,
		ORTHO_BOSS_1,
		ORTHO_BOSS_2,
		ORTHO_NPC_0,
		ORTHO_NPC_1,
		ORTHO_NPC_2,
		ORTHO_OBJ_0,
		ORTHO_OBJ_1,
		ORTHO_OBJ_2,
		ORTHO_END
	};

	enum class CHECK_MOUSE
	{
		// �Ʒ� ������ ��Ʈ ������ Ȯ�� (Ư�� Tap, Hold)
		MOUSE_CURSOR, // Ŀ���� ��ħ
		MOUSE_TAP, // �̹� �����ӿ� Ŭ����
		MOUSE_HOLD, // ���� ������ + �̹� ������ ��� Ŭ����
		MOUSE_NONE, // ���콺 ���� ���� 
		MOUSE_END
	};

	enum class TEXT_TYPE
	{
		TEXT_LEFT,
		TEXT_CENTER,
		TEXT_RIGHT,
		TEXT_END
	};

	enum class SCROLL_AREA
	{
		SCROLL_NONE,
		SCROLL_INVEN,
		SCROLL_END
	};
