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
		FONT_INFO_12,
		FONT_INFO_18,
		FONT_INFO_24,
		FONT_INFO_36,
		FONT_INFO_48,
		FONT_INFO_60,
		FONT_INFO_72,
		FONT_TITLE_12,
		FONT_TITLE_18,
		FONT_TITLE_24,
		FONT_TITLE_36,
		FONT_TITLE_48,
		FONT_TITLE_60,
		FONT_TITLE_72,
		FONT_END
	};
