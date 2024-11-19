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
		ITEM_INFO()
		{
			vecDamege.resize(_int(DAMEGE_TYPE::DAMEGE_END));
			vecDefence.resize(_int(DEFENCE_TYPE::DEFENCE_END));
			vecAblity_Grade.resize(_int(ABLITY_TYPE::ABLITY_END));
		}

		~ITEM_INFO()
		{
			vecDamege.clear();
			vecDefence.clear();
			vecAblity_Grade.clear();
		}

		_wstring strName = {};
		_int iTexture_Index = -1;
		ITEM_TYPE eType_Index = ITEM_TYPE::ITEMTYPE_END; // <- � ���������� �ε����� ���
		_bool bStack = false; // ���� ���� ����
		_int iCount = 1; // ���� (�� ���� �������� ������ 1)

		// ��/�ڷ� ����
		_bool bModule_Weapon = false; // <- ��/�ڷ� �и��ϴ� �������� 
		_bool bIsHandele = false; // <- �ڷ� ������ ���, false�� ���� 
		_int iOtherPart_Index = -1; // <- ��/�ڷ��� ��� �ٸ� �� ������ ���� �ε���
		const ITEM_INFO* pOtherPart = { nullptr }; // <- �� �ڷ� ����� ��� �ٸ��� ������ ������ 

		// ������
		_float fDurable_Now = 0.f;
		_float fDurable_Max = 0.f;

		// ���� 
		_float fWeight = 0.f;

		// ������
		vector<_float> vecDamege;
		
		// ���
		vector<_float> vecDefence;
		
		// Ư������
		_wstring strAttack_Type = {};
		_float fType_Damege = 0.f;
		_float fType_Damege_Fatal_Ratio = 0.f;

		//���̺� ���� 
		_wstring strFable_Art_Name = {};
		_int iFable_Art_Cost = {};

		// �ɷ�(?) <- �Ƹ� �� �� �� ������ �ϴ� �����͸� ������ �ֱ� 
		vector<_wstring> vecAblity_Grade;

		_wstring strSlash_Grade = {};
		_wstring strPirce_Grade = {};

		_wstring strFabel_Desc = {};
		_wstring strItem_Desc = {};

	

	}ITEM;

	typedef struct ARRAY_INFO // �κ��� �����ϴ� �迭�� ���� 
	{
		ARRAY_INFO()
		{
			vecItemInfo.resize(5);
			for (auto& iter : vecItemInfo)
				iter = new ITEM;
		}

		~ARRAY_INFO()
		{
			for (auto& iter : vecItemInfo)
				Safe_Delete(iter);

			vecItemInfo.clear();
			ValidEquipIndexlist.clear();
		}

		void Add_Array()
		{
			for (_int i = 0; i < 5; ++i)
			{
				ITEM* pNew = new ITEM;
				vecItemInfo.push_back(pNew);
			}
		}

		_int Get_Array_Size() { return vecItemInfo.size(); }
			
		_int iRow = 1; // 
		vector<ITEM*> vecItemInfo;  // �迭�� �� ������ ����
		list<_int> ValidEquipIndexlist;
	}ARRAY;

	typedef struct EQUIP_INFO
	{
		EQUIP_INFO()
		{
			vecValid_InvenArray.resize(_int(INVEN_ARRAY_TYPE::TYPE_END));
		}
		~EQUIP_INFO()
		{
			vecValid_InvenArray.clear();
		}

		INVEN_ARRAY_TYPE eType = INVEN_ARRAY_TYPE::TYPE_END; // <- ���� ���� �������� � Array�� �ִ� ���� �ǹ��Ѵ� 
		_int iIndex = 0; // <- �������� ��ġ�� �ε��� 
		vector<_bool> vecValid_InvenArray; // �ش� �������Կ� ����� �� �ִ� �κ� Array�� ���
	}EQUIP;


private:
	CItem_Manager(CGameInstance* pGameInstance);
	virtual ~CItem_Manager() = default;


public:
	// ����, ����
	ITEM_RESULT AddNewItem_Inven(INVEN_ARRAY_TYPE eIndex, _uint iCount = 1); // <- ���Ӱ� �������� ����� �κ��� �ִ´� 
	ITEM_RESULT EquipItem_Inven(INVEN_ARRAY_TYPE eIndex); // <- �κ��� �ִ� �������� ����Ѵ� 
	ITEM_RESULT UseItem_Equip(EQUIP_SLOT eSlot, _uint iCount = 1); // <- ���� �������� ����Ѵ�
	ITEM_RESULT UseItem_Inven(INVEN_ARRAY_TYPE eIndex, _uint iCount = 1); // <- �κ��� �ִ� �������� ���� ��� (

	const INVEN_ARRAY_TYPE& Get_Equip_ITEM_TYPE(EQUIP_SLOT eSlot) // ���� ���â�� ������ �������� �ε����� Ȯ��
	{
		//return m_vecArray_Inven[_int(m_vecEquip_ItemInfo[_int(eSlot)]->eType)]->vecItemInfo[_int(m_vecEquip_ItemInfo[_int(eSlot)]->iIndex)]->eIndex;
	} 
	const ITEM& Get_Equip_Item_Info(EQUIP_SLOT eSlot) // ���� ���â�� ������ �������� ���� ���۷��� ȹ��
	{ 
		return *m_vecArray_Inven[_int(m_vecEquip_ItemInfo[_int(eSlot)]->eType)]->vecItemInfo[_int(m_vecEquip_ItemInfo[_int(eSlot)]->iIndex)];
	}

	
	// �ܺο��� ������ �Ŵ����� �ʿ��� ���� ���� �ֳ�? 
	/*
	0. ���ο� �������� ����� ���� �κ��� �ִ´� 
	  -> ������ ���� Array���� �����ؼ� �κ��� �ִ� ��� �ʿ� + ������ ������ �� �־�� �Ѵ� (���� ������) 
	  -> ���� ���� �������̰� �̹� �κ��� �ִٸ� ���ڸ� �÷��� �Ѵ� 
	1. �κ��� �ִ� �������� ���� ��ġ�� �����Ѵ� 
	  -> ���� Ư���� �� ���� ���ƺ����� �ʴ´� -> �ļ����� �̷�� 
	2. ���� ������ �����Ѵ� 
	  -> ��� Array�� �� ��° �������� ����ϴ� ���� ����Ͽ� �����ϰ� �������� 
	  -> ���� ������ ĭ�� �����Ͽ��� �Ѵ� 
	  -> 
	
	
	
	*/




private:
	HRESULT Initialize_Item();

	// ����, ����
	ITEM_RESULT InputItem_Inven(ITEM* pItem, _uint iCount = 1); // <- �̹� ���ӿ� �����ϴ� �������� �κ��� �ִ´� 



private:
	CGameInstance* m_pGameInstance = { nullptr };

	vector<ITEM*> m_vecItem_BasicSpec; // ���� �� �����ϴ� ��� �������� �⺻ ����

	vector<_int> m_vecItem_InvenSlotIndex; // ������ Ÿ�� ���� �� �� �ִ� �κ� ���� ���� 
	/*
	��/�ڷ��� ��� 
	0. �Ϲ� ��ü �� +0;
	1. �Ϲ� ��ü �ڷ� +1;
	2. Ư�� ��ü �� +2;
	3. Ư�� ��ü �ڷ� +3;
	4. �Ϲ� �и� �� +4;
	5. �Ϲ� �и� �ڷ� +5;
	*/

	vector<ARRAY*> m_vecArray_Inven; // �κ��丮 ���� ���� (�� ����)
	vector<EQUIP*> m_vecEquip_ItemInfo; // ���� � ��� ���� ���� �� Ȯ�� 

	_int m_iInven_Array_Col_Count = 5; // <- �κ� �� �ٿ� �� ���� ���� ���� ��




public:
	static CItem_Manager* Create(CGameInstance* pGameInstance);
	virtual void Free() override;

};

END

