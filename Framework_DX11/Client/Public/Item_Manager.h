#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Interface_Enums.h"

#include "Player.h"

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



/*
�ƹķ�, ������� ���� ����ȭ 

�ƹķ� : ���� ������ �� �ִ� ������ �͸� �ֱ�

������� :
������ : ���� ���� 
������ : ȭ�� ���� ���� 
īƮ���� : ���� ���� ���� 
���̳� : �꼺 ���� ���� 



*/

class CItem_Manager : public CBase
{
public:
	typedef struct ITEM_INFO // ���ӿ� �����ϴ� ������ 1���� ��� ���� ���� 
	{
		ITEM_INFO()
		{
			vecDamege.resize(_uint(DAMEGE_TYPE::DAMEGE_END));
			vecDefence.resize(_uint(DEFENCE_TYPE::DEFENCE_END));
			vecAblity_Grade.resize(_uint(ABLITY_TYPE::ABLITY_END));
		}

		~ITEM_INFO()
		{
			vecDamege.clear();
			vecDefence.clear();
			vecAblity_Grade.clear();
		}

		_uint iItem_Index = -1;
		_wstring strName = {};
		_uint iTexture_Index = -1;
		ITEM_TYPE eType_Index = ITEM_TYPE::ITEMTYPE_END; // <- � ���������� �ε����� ���
		_bool bStack = false; // ���� ���� ����
		_uint iCount = 1; // ���� (�� ���� �������� ������ 1)

		// ��/�ڷ� ����
		_bool bModule_Weapon = false; // <- ��/�ڷ� �и��ϴ� �������� 
		_bool bIsHandele = false; // <- �ڷ� ������ ���, false�� ���� 
		_uint iOtherPart_Index = -1; // <- ��/�ڷ��� ��� �ٸ� �� ������ ���� �ε���
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

		// ����
		_float fPable_Charge = 0.f;
		_float fPulse_Charge = 0.f;

		// ���� �� ���ذ��� 
		_float fGuard_Damege_Reduce = 0.f;

		//���̺� ���� 
		_wstring strFable_Art_Name = {};
		_uint iFable_Art_Cost = {};

		// �ɷ�(?) <- �Ƹ� �� �� �� ������ �ϴ� �����͸� ������ �ֱ� 
		vector<_wstring> vecAblity_Grade;

		_wstring strSlash_Grade = {};
		_wstring strPirce_Grade = {};

		_wstring strFabel_Desc = {};
		_wstring strItem_Desc = {};

		// �� ����
		_float Arm_Gauge_Now = 1000.f;
		_float Arm_Gauge_Max = 1000.f;

		// ���� ���� 
		EQUIP_SLOT eSlot = EQUIP_SLOT::EQUIP_END;

		// ȹ�� ���� 
		_bool bIsNew = false;
		_bool bIsNewMark_Show = false;

		// ����, ������
		_bool bIsAvailable_Shop = false;
		_bool bIsAvailable_Chest = false;

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
			for (_uint i = 0; i < 5; ++i)
			{
				ITEM* pNew = new ITEM;
				vecItemInfo.push_back(pNew);
			}
		}

		_uint Get_Array_Size() { return _uint(vecItemInfo.size()); }
		ITEM* Get_Item_Info(_uint iIndex)
		{
			if ((iIndex < 0) || (iIndex >= Get_Array_Size()))
				return nullptr;
			return vecItemInfo[iIndex];
		}
		ITEM_RESULT Input_Item(ITEM* pNew, _uint iCount)
		{
			if (pNew->bStack)
			{
				for (auto& iter : vecItemInfo)
					if (iter->iItem_Index == pNew->iItem_Index)
					{
						iter->iCount += iCount;
						return ITEM_RESULT::RESULT_SUCCESS;
					}

				pNew->iCount = iCount;
			}
				

			if (iNextIndex >= Get_Array_Size())
				Add_Array();

			*vecItemInfo[iNextIndex] = *pNew;

			//memcpy(&vecItemInfo[iNextIndex], pNew, sizeof(ITEM));

			vecItemInfo[iNextIndex]->bIsNew = true;
			vecItemInfo[iNextIndex]->bIsNewMark_Show = false;

			++iNextIndex;

			return ITEM_RESULT::RESULT_SUCCESS;
		}
		ITEM_RESULT Use_Item(_uint iIndex, _uint iCount)
		{
			if ((iIndex < 0) || (iIndex >= Get_Array_Size()))
				return ITEM_RESULT::RESULT_INVALID;
			if (vecItemInfo[iIndex]->eType_Index == ITEM_TYPE::ITEMTYPE_END)
				return ITEM_RESULT::RESULT_INVALID;

			if (!vecItemInfo[iIndex]->bStack)
				return ITEM_RESULT::RESULT_SUCCESS;
			else if (vecItemInfo[iIndex]->iCount >= iCount)
			{
				vecItemInfo[iIndex]->iCount -= iCount;

				return ITEM_RESULT::RESULT_SUCCESS;
			}
			else
				return ITEM_RESULT::RESULT_INVALID;
		}
		ITEM_RESULT Remove_Item(_uint iIndex)
		{
			if ((iIndex < 0) || (iIndex >= Get_Array_Size()))
				return ITEM_RESULT::RESULT_INVALID;
			if (vecItemInfo[iIndex]->eType_Index == ITEM_TYPE::ITEMTYPE_END)
				return ITEM_RESULT::RESULT_INVALID;

			Safe_Delete(vecItemInfo[iIndex]);
			vecItemInfo[iIndex] = new ITEM;

			return ITEM_RESULT::RESULT_SUCCESS;
		}

		_wstring strInven_Array_Name = {};
		_uint iNextIndex = 0; // 
		vector<ITEM*> vecItemInfo;  // �迭�� �� ������ ����
		list<_uint> ValidEquipIndexlist;
	}ARRAY;

	typedef struct EQUIP_INFO
	{
		EQUIP_INFO()
		{
			vecValid_InvenArray.resize(_uint(INVEN_ARRAY_TYPE::TYPE_END));
		}
		~EQUIP_INFO()
		{
			vecValid_InvenArray.clear();
		}

		INVEN_ARRAY_TYPE eType = INVEN_ARRAY_TYPE::TYPE_END; // <- ���� ���� �������� � Array�� �ִ� ���� �ǹ��Ѵ� 
		_uint iIndex = 0; // <- �������� ��ġ�� �ε��� 
		vector<_bool> vecValid_InvenArray; // �ش� �������Կ� ����� �� �ִ� �κ� Array�� ���
	}EQUIP;

	typedef struct SHOP_INFO
	{
		_int iIndex = -1;
		_int iCount = 0;
		_int iPrice_Buy = 0;
	}SHOP;


private:
	CItem_Manager(CGameInstance* pGameInstance);
	virtual ~CItem_Manager() = default;


public:
	// ������Ʈ
	void Update_Item(_float fDeltatime);

	// �÷��̾� ��� ����
	CPlayer::WEAPON_TYPE Get_Weapon_Model_Index(); // ���� ��� ���� ������ �� ��ȣ ����
	list<SPECIAL_ITEM>& Get_LastFrame_UsingItem_Info(); // �̹� �����ӿ� ��� ������ �ʿ��� �������� ����ߴ��� Ȯ��
	SPECIAL_ITEM Check_Item_Special_Enum(_int iIndex);
	void Set_Item_Funtion(_int iItem_Index);
	_bool Get_CanSwitch_Weapon(); // ���� ���� 2�� �� 1���� �������� �ִ� ��� false ��ȯ

	SPECIAL_ITEM Get_Now_Select_Item()
	{
		_int iIndex = -1;
		if (m_bIsPotion)
		{
			const ITEM* pItem = Get_Equip_Item_Info(EQUIP_SLOT(_int(EQUIP_SLOT::EQUIP_USING_TOP_0) + m_iPotion_Select));
			if (pItem != nullptr)
				iIndex = pItem->iItem_Index;
		}
		else 
		{
			const ITEM* pItem = Get_Equip_Item_Info(EQUIP_SLOT(_int(EQUIP_SLOT::EQUIP_USING_BOTTOM_0) + m_iTool_Select));
			if (pItem != nullptr)
				iIndex = pItem->iItem_Index;
		}

		return Check_Item_Special_Enum(iIndex);
	}

	// ����, ����
	_bool Is_ItemData_Change() { return m_bIsChange; }
	ITEM_RESULT AddNewItem_Inven(_uint iItemIndex, _uint iCount = 1); // <- ���Ӱ� �������� ����� �κ��� �ִ´� 
	ITEM_RESULT EquipItem_Inven(INVEN_ARRAY_TYPE eIndex, EQUIP_SLOT eSlot, _uint iIndex);
	ITEM_RESULT EquipWeapon_Inven(INVEN_ARRAY_TYPE eIndex, EQUIP_SLOT eSlot, _uint iIndex); // <- �κ��� �ִ� �������� ����Ѵ� 
	ITEM_RESULT UnEquipItem_Inven(EQUIP_SLOT eSlot); // <- �κ��� �ִ� �������� ����Ѵ� 
	ITEM_RESULT UnEquipWeapon_Inven(EQUIP_SLOT eSlot); // <- �κ��� �ִ� �������� ����Ѵ� 

	ITEM_RESULT Move_EquipItem(EQUIP_SLOT eBefore, EQUIP_SLOT eDest); // <-���� �������� ��� ��ġ ����

	ITEM_RESULT UseItem_Equip(EQUIP_SLOT eSlot, _uint iCount = 1); // <- ���� �������� ����Ѵ�
	ITEM_RESULT UseItem_Inven(INVEN_ARRAY_TYPE eIndex, _uint iIndex, _uint iCount = 1); // <- �κ��� �ִ� �������� ���� ���

	ITEM_RESULT Use_Potion_Slot(_uint iCount = 1) { return UseItem_Equip(EQUIP_SLOT(_int(EQUIP_SLOT::EQUIP_USING_TOP_0) + m_iPotion_Select), iCount); } // <- ���õ� �� ��Ʈ ������ ���
	ITEM_RESULT Use_Tool_Slot(_uint iCount = 1) { return UseItem_Equip(EQUIP_SLOT(_int(EQUIP_SLOT::EQUIP_USING_BOTTOM_0) + m_iTool_Select), iCount); }  // <- ���õ� �Ʒ� ��Ʈ ������ ���
	ITEM_RESULT Use_Bag_Slot(_uint iSelect, _uint iCount = 1) { return UseItem_Equip(EQUIP_SLOT(_int(EQUIP_SLOT::EQUIP_USING_BAG_0) + iSelect), iCount); }  // <- ���õ� ���� ���� ������ ���

	ITEM_RESULT Remove_Item_Inven(INVEN_ARRAY_TYPE eIndex, _uint iIndex); // <- �κ��� ������ ����

	

	ITEM_RESULT Get_Array_Info(INVEN_ARRAY_TYPE eIndex, _wstring* strName, _int* iRow)
	{
		if ((_int(eIndex) < 0) || (_int(eIndex) >= _int(INVEN_ARRAY_TYPE::TYPE_END)))
			return ITEM_RESULT::RESULT_INVALID;

		*strName = m_vecArray_Inven[_int(eIndex)]->strInven_Array_Name;
		*iRow = m_vecArray_Inven[_int(eIndex)]->Get_Array_Size() / 5;

		return ITEM_RESULT::RESULT_SUCCESS;
	}
	const ITEM* Get_Item_Origin_Spec(_uint iIndex)
	{
		if ((iIndex < 0) || (iIndex >= m_vecItem_BasicSpec.size()))
			return nullptr;

		return m_vecItem_BasicSpec[iIndex];
	}
	const ITEM* Get_Array_Item_Info(INVEN_ARRAY_TYPE eIndex, _uint iIndex) // <- �κ��� ������ ���� ȹ��
	{
		if ((_int(eIndex) < 0) || (_int(eIndex) >= _int(INVEN_ARRAY_TYPE::TYPE_END)))
			return nullptr;

		if ((iIndex < 0) || (iIndex >= m_vecArray_Inven[_int(eIndex)]->Get_Array_Size()))
			return nullptr;

		if (m_vecArray_Inven[_int(eIndex)]->vecItemInfo[iIndex]->eType_Index == ITEM_TYPE::ITEMTYPE_END)
			return nullptr;

		return m_vecArray_Inven[_int(eIndex)]->vecItemInfo[iIndex];
	}

	const ITEM* Get_Equip_Item_Info(EQUIP_SLOT eSlot) // ���� ���â�� ������ �������� ���� ȹ��
	{
		if ((_int(eSlot) < 0) || (_int(eSlot) >= _int(EQUIP_SLOT::EQUIP_END)))
			return nullptr;

		if (m_vecEquip_ItemInfo[_uint(eSlot)]->eType == INVEN_ARRAY_TYPE::TYPE_END)
			return nullptr;

		return m_vecArray_Inven[_uint(m_vecEquip_ItemInfo[_uint(eSlot)]->eType)]->vecItemInfo[_uint(m_vecEquip_ItemInfo[_uint(eSlot)]->iIndex)];
	}

	const EQUIP* Get_Equip_Slot_Info(EQUIP_SLOT eSlot)
	{
		if ((_int(eSlot) < 0) || (_int(eSlot) >= _int(EQUIP_SLOT::EQUIP_END)))
			return nullptr;

		return m_vecEquip_ItemInfo[_int(eSlot)];
	}






	
	// ���� ������ ����
	_int Change_Potion_Select(_bool bNext)
	{
		m_bIsChange = true;
		if (bNext)
			++m_iPotion_Select;
		else
			--m_iPotion_Select;

		if (m_iPotion_Select > 2)
			m_iPotion_Select = 0;
		else if (m_iPotion_Select < 0)
			m_iPotion_Select = 2;

		return m_iPotion_Select;
	}
	_int Change_Tool_Select(_bool bNext)
	{
		m_bIsChange = true;
		if (bNext)
			++m_iTool_Select;
		else
			--m_iTool_Select;

		if (m_iTool_Select > 2)
			m_iTool_Select = 0;
		else if (m_iTool_Select < 0)
			m_iTool_Select = 2;

		return m_iTool_Select;
	}
	_int Get_Potion_Select() { return m_iPotion_Select; }
	_int Get_Tool_Select() { return m_iTool_Select; }
	void Set_Select(_bool bIsPotion) { m_bIsPotion = bIsPotion; }
	

	// ���� ����
	_int Change_Weapon()
	{
		m_bIsChange = true;
		++m_iWeapon_Select;
		if (m_iWeapon_Select >= 2)
			m_iWeapon_Select = 0;

		return m_iWeapon_Select;
	}
	_int Get_Weapon() { return m_iWeapon_Select; }
	const ITEM* Get_Now_Equip_Weapon_Blade()
	{
		if (m_iWeapon_Select == 0)
			return Get_Equip_Item_Info(EQUIP_SLOT::EQUIP_WEAPON_BLADE_0);
		else
			return Get_Equip_Item_Info(EQUIP_SLOT::EQUIP_WEAPON_BLADE_1);
	}

	const ITEM* Get_Now_Equip_Weapon_Handle()
	{
		if (m_iWeapon_Select == 0)
			return Get_Equip_Item_Info(EQUIP_SLOT::EQUIP_WEAPON_HANDLE_0);
		else
			return Get_Equip_Item_Info(EQUIP_SLOT::EQUIP_WEAPON_HANDLE_1);
	}
	
	void Add_Durable_Weapon(_float fAdd)
	{
		ITEM* pNowWeapon = m_vecArray_Inven[_int(m_vecEquip_ItemInfo[_int(EQUIP_SLOT::EQUIP_WEAPON_BLADE_0) + m_iWeapon_Select * 2]->eType)]->vecItemInfo[m_vecEquip_ItemInfo[_int(EQUIP_SLOT::EQUIP_WEAPON_BLADE_0) + m_iWeapon_Select * 2]->iIndex];

		if (pNowWeapon != nullptr)
		{
			pNowWeapon->fDurable_Now += fAdd;
			pNowWeapon->fDurable_Now = min(pNowWeapon->fDurable_Now, pNowWeapon->fDurable_Max);
			pNowWeapon->fDurable_Now = max(pNowWeapon->fDurable_Now, 0.f);
		}
	}



	// ���� ���� 
	void Add_Potion_Gauge(_float fAdd)
	{
		m_bIsChange = true;
		m_fNow_Potion_Gauge += fAdd;
		if (m_fNow_Potion_Gauge >= m_fMax_Potion_Gauge)
		{
			m_fNow_Potion_Gauge = 0.f;
			m_iNow_Potion_Count = min(m_iNow_Potion_Count + 1, m_iMax_Potion_Count);

			for (_int i = 0; i < 5; ++i)
				if (m_vecArray_Inven[_int(INVEN_ARRAY_TYPE::TYPE_USING_BASIC)]->Get_Item_Info(i)->iItem_Index == _int(SPECIAL_ITEM::SP_PULSE_BATTERY))
					m_vecArray_Inven[_int(INVEN_ARRAY_TYPE::TYPE_USING_BASIC)]->Get_Item_Info(i)->iCount = m_iNow_Potion_Count;
		}
	}
	_bool Use_Potion();

	// �� ����
	void Add_Arm_Gauge(_float fAdd)
	{
		m_bIsChange = true;
		_int iIndex = m_vecEquip_ItemInfo[_int(EQUIP_SLOT::EQUIP_RESION_ARM)]->iIndex;
		ITEM* pItem = m_vecArray_Inven[_int(INVEN_ARRAY_TYPE::TYPE_REASON_ARM)]->Get_Item_Info(iIndex);
		if (pItem == nullptr)
			return;

		if (fAdd > 0)
			pItem->Arm_Gauge_Now = min(pItem->Arm_Gauge_Now + fAdd, pItem->Arm_Gauge_Max);
		else if ((fAdd < 0) && (abs(fAdd) <= pItem->Arm_Gauge_Now))
			pItem->Arm_Gauge_Now = max(pItem->Arm_Gauge_Now + fAdd, 0.f);
	}
	_float Get_Arm_Gauge_Ratio()
	{
		_int iIndex = m_vecEquip_ItemInfo[_int(EQUIP_SLOT::EQUIP_RESION_ARM)]->iIndex;
		ITEM* pItem = m_vecArray_Inven[_int(INVEN_ARRAY_TYPE::TYPE_REASON_ARM)]->Get_Item_Info(iIndex);
		if (pItem == nullptr)
			return 0.f;

		return pItem->Arm_Gauge_Now / pItem->Arm_Gauge_Max;
	}

	// New ǥ�� ���� ���� 
	_int Get_IsNew(INVEN_ARRAY_TYPE eType, _int iIndex) 
	{
		if(!IsValid_Inven(eType, iIndex))
			return -1;

		if (m_vecArray_Inven[_int(eType)]->Get_Item_Info(iIndex))
			return _int(m_vecArray_Inven[_int(eType)]->Get_Item_Info(iIndex)->bIsNew);
		else
			return -1;
	}
	void Set_IsNew(INVEN_ARRAY_TYPE eType, _int iIndex, _bool IsNew)
	{
		if (!IsValid_Inven(eType, iIndex))
			return;

		if (m_vecArray_Inven[_int(eType)]->Get_Item_Info(iIndex))
		{
			m_vecArray_Inven[_int(eType)]->Get_Item_Info(iIndex)->bIsNew = IsNew;
			m_vecArray_Inven[_int(eType)]->Get_Item_Info(iIndex)->bIsNewMark_Show = false;
		}
		else
			return;
	}
	void Set_IsNew_Show(INVEN_ARRAY_TYPE eType, _int iIndex)
	{
		if (!IsValid_Inven(eType, iIndex))
			return;

		if (m_vecArray_Inven[_int(eType)]->Get_Item_Info(iIndex))
			m_vecArray_Inven[_int(eType)]->Get_Item_Info(iIndex)->bIsNewMark_Show = true;
		else
			return;
	}
	void Set_Show_NewMark_Off()
	{
		for (auto& iter : m_vecArray_Inven)
			for (auto& iterItem : iter->vecItemInfo)
				if (iterItem->bIsNewMark_Show)
				{
					iterItem->bIsNew = false;
					iterItem->bIsNewMark_Show = false;
				}
	}

	// ItemAction ���� 
	void Set_ItemAction(EQUIP_SLOT eSlot, _Vec2 vPos, _Vec2 vSize, _bool bMainTainPage = false);
	void Set_ItemAction(INVEN_ARRAY_TYPE eType, _int iIndex, _Vec2 vPos, _Vec2 vSize, _bool bMainTainPage = false);

	void Reset_ItemAction();

	_bool IsReset_ItemAction();


	ITEM_RESULT Operate_ItemAction(ITEM_FUNC eFunc, _Vec2 vPos, _Vec2 vSize);

	ITEM_RESULT Operate_EquipAction(_Vec2 vPos, _Vec2 vSize);



	// Defence_Info 
	void Adjust_Spec();


	// ����, ������
	vector<SHOP*>& Get_ShopData() { return m_vecShop_Item; }
	vector<ITEM*>& Get_ChestData() { return m_vecChest_Item; }
	void Buy_ShopItem(_int iIndex);
	void Sell_ShopItem(INVEN_ARRAY_TYPE eType, _int iIndex);
	void ChestItem_To_Inven(_int iIndex);
	void InvenItem_To_Chest(INVEN_ARRAY_TYPE eType, _int iIndex);



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


	// �� + �ڷ� ��ü
	// �Ʒ� ������ ���� �ʿ��ϸ� �ۺ����� �̵�
	ITEM_RESULT Assemble_Blade_Handle(_int iBladeIndex, _int iHandleIndex);

	// ��ȿ ���� ���� 
	_bool IsValid_Inven(INVEN_ARRAY_TYPE eType, _int iIndex)
	{
		if ((_int(eType) < 0) || (_int(eType) >= _int(INVEN_ARRAY_TYPE::TYPE_END)))
			return false;

		if (m_vecArray_Inven[_int(eType)]->Get_Array_Size() <= _uint(iIndex))
			return false;

		return true;
	}

	_bool IsValid_Equip(EQUIP_SLOT eSlot)
	{
		if ((_int(eSlot) < 0) || (_int(eSlot) >= _int(EQUIP_SLOT::EQUIP_END)))
			return false;

		return true;
	}



private:
	CGameInstance* m_pGameInstance = { nullptr };

	vector<ITEM*> m_vecItem_BasicSpec; // ���� �� �����ϴ� ��� �������� �⺻ ����

	vector<_uint> m_vecItem_InvenSlotIndex; // ������ Ÿ�� ���� �� �� �ִ� �κ� ���� ���� 

	_uint m_iAdd_Handle = 1;
	_uint m_iAdd_Heroic = 2;
	_uint m_iAdd_Separate = 4;
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

	vector<SHOP*> m_vecShop_Item; // ���� ������ ���
	vector<ITEM*> m_vecChest_Item; // ������ ������ ���

	_uint m_iInven_Array_Col_Count = 5; // <- �κ� �� �ٿ� �� ���� ���� ���� ��

	// ���ϴ� ���ۿ�
	_uint m_iPotion_Select = 0;
	_uint m_iTool_Select = 0;
	_bool m_bIsPotion = false;

	// ���ϴ� ���ۿ�
	_uint m_iWeapon_Select = -1;

	// ���� ���� 
	_int m_iNow_Potion_Count = 3;
	_int m_iMax_Potion_Count = 5;
	_float m_fNow_Potion_Gauge = 0.f;
	_float m_fMax_Potion_Gauge = 1000.f;

	// ������ ���� 
	_bool m_bIsChange = false;
	_int m_iNow_Using = -1;

	list<SPECIAL_ITEM> m_LastFrame_UsingItem;

	// ������ �׼� ���� 
	_bool m_bItemAction_Active = false;
	EQUIP_SLOT m_eNow_ActionSlot = EQUIP_SLOT::EQUIP_END;
	INVEN_ARRAY_TYPE m_eNow_ActionArray = INVEN_ARRAY_TYPE::TYPE_END;
	_int m_iArray_Index = -1;
	_int m_iActionPopup_Page = 0;
	_bool m_bInstant_Weapon_Change = false;

	_bool m_bSpec_Setting = false;

public:
	static CItem_Manager* Create(CGameInstance* pGameInstance);
	virtual void Free() override;

};

END

