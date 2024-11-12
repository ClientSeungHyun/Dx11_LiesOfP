#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Interface_Enums.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

/*
[Item_Manager]
1. ����
 0) �������� �⺻ ���� ������ ����
 1) �κ��丮 ����
 2) ���� ������ ����
 3) ������ ȹ��, ��� ����

 �� �߰� ���� �ʿ��� ����
 �Ʒ��� Ȯ���� �ƴϸ� ���� �ʿ�
 4) NPC ���� ������ ����
 5) ���� �� ��� ������ ����
 6) ����Ʈ, �̺�Ʈ�� ȹ���ϴ� �������� ���� ����

2. �ν��Ͻ�
 1) ������ ���� ����
 2) �κ� ����
 3) ���� ������ ����


3. ��� ����
 1) �׽�Ʈ �������� ���÷� �ٲ� �� �ִ� ���� �ý���
 2) �ټ��� ������ ������ ������ �����ϵ��� ����

4. ��Ģ
 1) �ű� ������ ���� & ���� -> ���� ����
 2) ������ �̵� -> ���� ���� �� ���� ������ nullptr ó�� (Delete ���� ���� ����)
 3) ������ ���� -> Safe_Delete ó��
 4) ������ ���� -> ���� ���縸 ����
 5) ������ ���� -> Equip�ʿ��� nullptr ó���� ���� (���� Delete ���� ����)
 6) �ܺ� ���� -> �ݵ�� ���۷�����



*/

class CItem_Manager : public CBase
{
public:
	typedef struct ITEM_INFO // ���ӿ� �����ϴ� ������ 1���� ��� ���� ���� 
	{
		ITEM_INDEX eIndex = ITEM_INDEX::INDEX_END; // <- � ���������� �ε����� ���
		ITEM_TYPE eType = ITEM_TYPE::TYPE_END; // <- �������� Ÿ�� 
		_bool bStack = false; // ���� ���� ����
		_int iCount = 1; // ���� (�� ���� �������� ������ 1)
	}ITEM;

	typedef struct ARRAY_INFO // �κ��� �����ϴ� �迭�� ���� 
	{
		ARRAY_INFO()
		{

		}

		~ARRAY_INFO()
		{
			for (auto& iter : vecItemInfo)
				Safe_Delete(iter);

			vecItemInfo.clear();
			ValidEquipIndexlist.clear();
		}

		_int iRow = 1; // 
		vector<ITEM*> vecItemInfo;  // �迭�� �� ������ ����
		list<_int> ValidEquipIndexlist;
	}ARRAY;



private:
	CItem_Manager(CGameInstance* pGameInstance);
	virtual ~CItem_Manager() = default;


public:
	// ����, ����
	ITEM_RESULT AddNewItem_Inven(ITEM_INDEX eIndex, _uint iCount = 1); // <- ���Ӱ� �������� ����� �κ��� �ִ´� 
	ITEM_RESULT EquipItem_Inven(ITEM_INDEX eIndex); // <- �κ��� �ִ� �������� ����Ѵ� 
	ITEM_RESULT UseItem_Equip(EQUIP_SLOT eSlot, _uint iCount = 1); // <- ���� �������� ����Ѵ�
	ITEM_RESULT UseItem_Inven(ITEM_INDEX eIndex, _uint iCount = 1); // <- �κ��� �ִ� �������� ���� ��� (

	const ITEM_INDEX& Get_Equip_Item_Index(EQUIP_SLOT eSlot) { return m_vecEquip_ItemInfo[_int(eSlot)]->eIndex; } // ���� ���â�� ������ ������ Ȯ��
	const ITEM& Get_Equip_Item_Info(EQUIP_SLOT eSlot) { return *m_vecEquip_ItemInfo[_int(eSlot)]; }






private:
	HRESULT Initialize_Item();

	// ����, ����
	ITEM_RESULT InputItem_Inven(ITEM* pItem, _uint iCount = 1); // <- �̹� ���ӿ� �����ϴ� �������� �κ��� �ִ´� 



private:
	CGameInstance* m_pGameInstance = { nullptr };

	vector<ITEM*> m_vecItem_BasicSpec; // ���� �� �����ϴ� ��� �������� �⺻ ����

	vector<ARRAY*> m_vecArray_Inven; // �κ��丮 ���� ���� 

	vector<ITEM*> m_vecEquip_ItemInfo; // ���� � ��� ���� ���� �� Ȯ�� 






public:
	static CItem_Manager* Create(CGameInstance* pGameInstance);
	virtual void Free() override;

};

END

