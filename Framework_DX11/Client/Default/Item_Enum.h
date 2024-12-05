#pragma once

enum class ITEM_SPEC
{

};

enum class INVEN_ARRAY_TYPE
{
	// �κ��丮 ���� ������� ���� 
	TYPE_USING_BASIC, // ��� ������	�⺻������
	TYPE_USING_EXPENDABLE, // ��� ������	�Ҹ�ǰ
	TYPE_USING_THROW, // ��� ������	��ô��
	TYPE_USING_ERGO, // ��� ������	������
	TYPE_USING_RARE_ERGO, // ��� ������	��� ������

	TYPE_INGRADIENT_GROW, // ����	���� ����
	TYPE_INGRADIENT_SPECIAL, // ����	Ư�� ����

	TYPE_FUNTIONAL_COMMON, // ��� ������(��� Ȯ�� �ʿ�)

	TYPE_WEAPON_NORMAL_BLADE, // ����	�Ϲ� ����(��)
	TYPE_WEAPON_NORMAL_HANDLE, // ����	�Ϲ� ����(�ڷ�)
	TYPE_WEAPON_SPECIAL_BLADE, // ����	Ư�� ����(��)
	TYPE_WEAPON_SPECIAL_HANDLE, // ���� Ư�� ����(�ڷ�)


	TYPE_BLADE_PART, // �� / ������ ��
	TYPE_HANDEL_PART,// �� / ������ ������

	TYPE_REASON_ARM, // ���� ��

	TYPE_DEFENCE_FRAME, // ��� ����	������
	TYPE_DEFENCE_CONVERTOR, // ��� ����	������
	TYPE_DEFENCE_CARTRIGE, // ��� ����	īƮ����
	TYPE_DEFENCE_RAINER, // ��� ����	���̳�

	TYPE_AMULET_COMMON, // �ƹķ�

	TYPE_COUSTUME_CLOTHES, // �ڽ�Ƭ	�ǻ�
	TYPE_COUSTUME_MASK, // �ڽ�Ƭ	����
	TYPE_COUSTUME_ACC, // �ڽ�Ƭ	�Ǽ�����

	TYPE_GEUSTURE_COMMON, // ������

	TYPE_COLLECTION_NEWS, // ����ǰ	�Ź�
	TYPE_COLLECTION_BOOK, // ����ǰ	����
	TYPE_COLLECTION_MEMO, // ����ǰ	����


	TYPE_END
};

enum class EQUIP_SLOT
{
	// ���â ������� ���� 

	EQUIP_WEAPON_BLADE_0,  // ���	���� �� ���� �� 0
	EQUIP_WEAPON_HANDLE_0,

	EQUIP_WEAPON_BLADE_1,  // ���	���� �� ���� �� 1
	EQUIP_WEAPON_HANDLE_1,

	EQUIP_RESION_ARM,  // ���	���� �� ���� �� 2

	EQUIP_AMULET_0, // ���	�ƹķ� 0
	EQUIP_AMULET_1,// ���	�ƹķ� 1

	EQUIP_DEFENCE_FRAME, // ��� ����	������
	EQUIP_DEFENCE_CONVERTOR, // ��� ����	������
	EQUIP_DEFENCE_CARTRIGE, // ��� ����	īƮ����
	EQUIP_DEFENCE_RAINER, // ��� ����	���̳�

	EQUIP_USING_TOP_0, // ��� ������	�� ��Ʈ
	EQUIP_USING_TOP_1,
	EQUIP_USING_TOP_2,

	EQUIP_USING_BOTTOM_0,  // ��� ������	�Ʒ� ��Ʈ
	EQUIP_USING_BOTTOM_1,
	EQUIP_USING_BOTTOM_2,

	EQUIP_USING_BAG_0, // ��� ������	��������
	EQUIP_USING_BAG_1,
	EQUIP_USING_BAG_2,
	EQUIP_USING_BAG_3,

	EQUIP_HEAD_0, // �ڽ�Ƭ	�Ӹ�
	EQUIP_HEAD_1,
	EQUIP_CLOTHES, // �ڽ�Ƭ	�ǻ�

	EQUIP_END
};


enum class ITEM_TYPE
{
	ITEMTYPE_SWORD,
	ITEMTYPE_HAMMER,
	ITEMTYPE_BIGHAMMER,
	ITEMTYPE_BIGSWORD,
	ITEMTYPE_DAGGER,
	ITEMTYPE_SINGLEHAND,
	ITEMTYPE_TWOHAND,
	ITEMTYPE_POLE,
	ITEMTYPE_ARM,
	ITEMTYPE_AMULET,
	ITEMTYPE_FRAME,
	ITEMTYPE_CONVERTOR,
	ITEMTYPE_CARTRIGE,
	ITEMTYPE_RINER,
	ITEMTYPE_SPEND,
	ITEMTYPE_BASIC,
	ITEMTYPE_RAER_ERGO,
	ITEMTYPE_ERGO,
	ITEMTYPE_THROW,
	ITEMTYPE_BLADE_RAINFORCE,
	ITEMTYPE_SPECIAL_INGRADIANT,
	ITEMTYPE_ACTIVATE_FUNTION,
	ITEMTYPE_EXCHAINGE_INGRADIANT,
	ITEMTYPE_GRAINDER,
	ITEMTYPE_HANDLE_CONVERTOR,
	ITEMTYPE_LEVELUP_INGRADIENT,
	ITEMTYPE_END
};

enum class ITEM_RESULT
{
	RESULT_SUCCESS,
	RESULT_FULL,
	RESULT_INVALID,
	RESULT_END
};

enum class DAMEGE_TYPE
{
	DAMEGE_PHYSICS,
	DAMEGE_FIRE,
	DAMEGE_ELECTRIC,
	DAMEGE_ACID,
	DAMEGE_END
};

enum class DEFENCE_TYPE
{
	DEFENCE_FIRE,
	DEFENCE_ELECTRIC,
	DEFNECE_COLLAPSE,
	DEFENCE_CRASH,
	DEFENCE_BREAK,
	DEFENCE_SLASH,
	DEFNECE_HIT,
	DEFENCE_PIRCE,
	DEFENCE_END
};

enum class ABLITY_TYPE
{
	ABLITY_POWER,
	ABLITY_SKILL,
	ABLITY_EVOUTION,
	ABLITY_END
};


enum class SPECIAL_ITEM
{
	// Ư�� ����� ������ �ε��� ��� 
	SP_PULSE_BATTERY = 284, // �޽� ����, ���� ��ġ ���� �����ϸ� ��� ��, ü�� ȸ�� O
	SP_DUSTCAT = 238, // ����� ����, ��ô ���� 
	SP_GRINDER = 239, // �׶��δ�, ���� ������ ���� O 
	SP_LAMP = 243, // �𳪵��� ���, �ֺ� ��� ���� 
	SP_TELEPOT = 244, // �������� ȸ�߽ð�, �����̵� O
	SP_RESISTANCE = 255, // �Ӽ� ���� ����, �Ӽ� ���� ���� O
	SP_PURIFICATION = 256, // �Ӽ� ��ȭ ����, ����� ���� O
	SP_DEAD = 272, // ������ ����, ��� O
	SP_GRANADE = 275, // Ŭ������ ����ź O
	SP_THERMITE = 276, // �׸��� O
	SP_THROW_BATTERY = 279, // ��ô�� ���� O 

	SP_END
};

enum class ITEM_FUNC
{
	FUNC_USING, // <- �Ϲ� �������� ����Ͽ� ��� Ȱ��ȭ 
	FUNC_TO_INVEN, // <- �κ����� �̵�
	FUNC_TO_EQUIP, // <- ���� �̵�
	FUNC_EQUIP, // <- �Ϲ� ������ �� ������ ����
	FUNC_UNEQUIP, // <- ������ ���� ���� 
	FUNC_DELETE, // <- ������ ������ (����)

	FUNC_EQUIP_TOP, // <- �� ��Ʈ
	FUNC_EQUIP_BOTTOM, // <- �Ʒ� ��Ʈ 
	FUNC_EQUIP_BAG, // <- ���� 

	FUNC_EQUIP_WEAPON_FIRST,
	FUNC_EQUIP_WEAPON_SECOND,

	FUNC_EQUIP_TOP_0,
	FUNC_EQUIP_TOP_1,
	FUNC_EQUIP_TOP_2,

	FUNC_EQUIP_BOTTOM_0,
	FUNC_EQUIP_BOTTOM_1,
	FUNC_EQUIP_BOTTOM_2,

	FUNC_EQUIP_BAG_0,
	FUNC_EQUIP_BAG_1,
	FUNC_EQUIP_BAG_2,
	FUNC_EQUIP_BAG_3,

	FUNC_END
};


