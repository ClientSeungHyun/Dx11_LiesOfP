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
		ACTION_NONE,
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
		PAGE_END

		/*PAGE_MENU,
		PAGE_INVEN,
		PAGE_EQUIP,
		PAGE_STAT,
		PAGE_LEVELUP,
		PAGE_SKILL,*/

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
		TYPE_FLOAT2,
		TYPE_FLOAT3,
		TYPE_FLOAT4,
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