#pragma once

enum class ITEM_SPEC
{

};

enum class ITEM_TYPE
{
	// �κ��丮 ���� ������� ���� 
	TYPE_USING_BASIC, // ��� ������	�⺻������
	TYPE_USING_EXPENDABLE, // ��� ������	�Ҹ�ǰ
	TYPE_USING_THROW, // ��� ������	��ô��
	TYPE_USING_RARE_ERGO, // ��� ������	��� ������

	TYPE_INGRADIENT_GROW, // ����	���� ����
	TYPE_INGRADIENT_SPECIAL, // ����	Ư�� ����
	
	TYPE_FUNTIONAL_COMMON, // ��� ������(��� Ȯ�� �ʿ�)

	TYPE_WEAPON_NORMAL_BLADE, // ����	�Ϲ� ����(��)
	TYPE_WEAPON_NORMAL_HANDLE, // ����	�Ϲ� ����(�ڷ�)
	TYPE_WEAPON_SPECIAL, // ����	Ư�� ����

	TYPE_BLADE_HANDEL_PART, // �� / ������

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
	EQUIP_WEAPON_BLADE_0,  // ���	���� �� ���� �� 0
	EQUIP_WEAPON_HANDLE_0,
	EQUIP_WEAPON_SPECIAL_0,

	EQUIP_WEAPON_BLADE_1,  // ���	���� �� ���� �� 1
	EQUIP_WEAPON_HANDLE_1,
	EQUIP_WEAPON_SPECIAL_1,

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


enum class ITEM_INDEX
{
	INDEX_END
};

enum class ITEM_RESULT
{
	RESULT_SUCCESS,
	RESULT_FULL,
	RESULT_INVALID,
	RESULT_END
};






