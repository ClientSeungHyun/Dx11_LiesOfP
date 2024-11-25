#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "GameInstance.h"
#include "UIManager.h"
#include "Item_Manager.h"
#include "Player_Stat_Manager.h"

BEGIN(Engine)
class CGameObject;
END




BEGIN(Client)

class CGameInterface_Controller : public CBase
{
	DECLARE_SINGLETON(CGameInterface_Controller)




private:
	CGameInterface_Controller();
	virtual ~CGameInterface_Controller() = default;






public: // �ܺο��� �Ŵ��� ���� 




#pragma region Common
	HRESULT Initialize_GameInterface(ID3D11Device** pDevice, ID3D11DeviceContext** pContext, CGameInstance* pInstance);
	void Update_GameInterface(_float fTimeDelta);
	void Release_GameInterface();

#pragma endregion


#pragma region UIManager
	void OpenMainPage() { m_pUIManager->OpenMainPage(); }
	void OpenLoadingPage() { m_pUIManager->OpenLoadingPage(); }

	void Input_Render_Info(CUIRender_Batching::UIRENDER_INFO* pInfo) { m_pUIManager->Input_Render_Info(pInfo); }

	_Vec2 CheckMouse(_Vec2 fPos, _Vec2 fSize) { return m_pUIManager->CheckMouse(fPos, fSize); }

	_bool Action_InterAction(_wstring strInterName) { return m_pUIManager->Action_InterAction(strInterName); }

	void SetIngame(_bool bTrue) { m_pUIManager->SetIngame(bTrue); }

	void SwicthPage(UIPAGE ePageA, UIPAGE ePageB) { m_pUIManager->SwicthPage(ePageA, ePageB); }
	CUIManager::TDATA* GetTestData() { return m_pUIManager->GetTestData(); }
	// TestPage 
	void ShowTestPage(TEST_PAGE_NAME eName,
		_wstring DataNameA, TEST_PAGE_VALUE_TYPE eTypeA, const void* ValueA,
		_wstring DataNameB = {}, TEST_PAGE_VALUE_TYPE eTypeB = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueB = nullptr,
		_wstring DataNameC = {}, TEST_PAGE_VALUE_TYPE eTypeC = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueC = nullptr,
		_wstring DataNameD = {}, TEST_PAGE_VALUE_TYPE eTypeD = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueD = nullptr,
		_wstring DataNameE = {}, TEST_PAGE_VALUE_TYPE eTypeE = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueE = nullptr,
		_wstring DataNameF = {}, TEST_PAGE_VALUE_TYPE eTypeF = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueF = nullptr,
		_wstring DataNameG = {}, TEST_PAGE_VALUE_TYPE eTypeG = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueG = nullptr,
		_wstring DataNameH = {}, TEST_PAGE_VALUE_TYPE eTypeH = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueH = nullptr,
		_wstring DataNameI = {}, TEST_PAGE_VALUE_TYPE eTypeI = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueI = nullptr,
		_wstring DataNameJ = {}, TEST_PAGE_VALUE_TYPE eTypeJ = TEST_PAGE_VALUE_TYPE::TYPE_END, const void* ValueJ = nullptr
	)
	{
		m_pUIManager->ShowTestPage(eName, DataNameA, eTypeA, ValueA, DataNameB, eTypeB, ValueB, DataNameC, eTypeC, ValueC, DataNameD, eTypeD, ValueD, DataNameE, eTypeE, ValueE,
			DataNameF, eTypeF, ValueF, DataNameG, eTypeG, ValueG, DataNameH, eTypeH, ValueH, DataNameI, eTypeI, ValueI, DataNameJ, eTypeJ, ValueJ);
	}// �Լ� ����ϸ� �Է� ���� ��� �ش� �����ӿ� â�� ��� �ش�, DataNameA: �������� �̸�, TEST_PAGE_VALUE_TYPE: �ڷ���(float,int,tchar), ValueA: ���� �޾ƿ� ������ <- �̷��� ����
// �� ������ ���� �Լ��� �������� �۵��� (Gameinterface�� �ܺ� ������ ����/�������� �ʵ��� �ϱ� ����)

	void Set_Scroll_Area(SCROLL_AREA eArea, _Vec2 vPos, _Vec2 vSize) { m_pUIManager->Set_Scroll_Area(eArea, vPos, vSize); }
	void Select_Scroll_Area(SCROLL_AREA eArea) { m_pUIManager->Select_Scroll_Area(eArea); }

	void Set_Scroll_Y_Offset(SCROLL_AREA eArea, _float fOffset) { m_pUIManager->Set_Scroll_Y_Offset(eArea, fOffset); }

// ���� UI ��û 
	void Register_Pointer_Into_OrthoUIPage(UI_ORTHO_OBJ_TYPE eType, void* pObj) { m_pUIManager->Register_Pointer_Into_OrthoUIPage(eType, pObj); }

	// ������ ȹ�� ���� 
	void Input_Drop_Item_Info(_int iIndex, _int iCount) { m_pUIManager->Input_Drop_Item_Info(iIndex, iCount); }

#pragma endregion


#pragma region Item_Manager
	// ����, ����
	_bool Is_ItemData_Change() { return m_pItem_Manager->Is_ItemData_Change(); }

	ITEM_RESULT AddNewItem_Inven(_uint iItemIndex, _uint iCount = 1) { return m_pItem_Manager->AddNewItem_Inven(iItemIndex, iCount); } // <- ���Ӱ� �������� ����� �κ��� �ִ´� 
	ITEM_RESULT EquipItem_Inven(INVEN_ARRAY_TYPE eIndex, EQUIP_SLOT eSlot, _uint iIndex) { return m_pItem_Manager->EquipItem_Inven(eIndex, eSlot, iIndex); } // <- �κ��� �ִ� �������� ����Ѵ� 
	ITEM_RESULT UseItem_Equip(EQUIP_SLOT eSlot, _uint iCount = 1) { return m_pItem_Manager->UseItem_Equip(eSlot, iCount); } // <- ���� �������� ����Ѵ�
	ITEM_RESULT UseItem_Inven(INVEN_ARRAY_TYPE eIndex, _uint iIndex, _uint iCount = 1) { return m_pItem_Manager->UseItem_Inven(eIndex, iIndex, iCount); } // <- �κ��� �ִ� �������� ���� ��� 
	ITEM_RESULT Remove_Item_Inven(INVEN_ARRAY_TYPE eIndex, _uint iIndex) { return m_pItem_Manager->Remove_Item_Inven(eIndex, iIndex); } // <- �κ��� ������ ����

	ITEM_RESULT Use_Potion_Slot(_uint iCount = 1) { return m_pItem_Manager->Use_Potion_Slot(iCount); } // <- ���õ� �� ��Ʈ ������ ���
	ITEM_RESULT Use_Tool_Slot(_uint iCount = 1) { return m_pItem_Manager->Use_Tool_Slot(iCount); }  // <- ���õ� �Ʒ� ��Ʈ ������ ���
	ITEM_RESULT Use_Bag_Slot(_uint iSelect, _uint iCount = 1) { return m_pItem_Manager->Use_Bag_Slot(iSelect, iCount); }  // <- ���õ� ���� ���� ������ ���

	ITEM_RESULT Get_Array_Info(INVEN_ARRAY_TYPE eIndex, _wstring* strName, _int* iRow) { return m_pItem_Manager->Get_Array_Info(eIndex, strName, iRow); } // <- �κ� �迭 ���� ȹ��

	const CItem_Manager::ITEM* Get_Item_Origin_Spec(_uint iIndex) { return m_pItem_Manager->Get_Item_Origin_Spec(iIndex); }

	const CItem_Manager::ITEM* Get_Array_Item_Info(INVEN_ARRAY_TYPE eIndex, _uint iIndex) { return m_pItem_Manager->Get_Array_Item_Info(eIndex, iIndex); } // <- �κ��� ������ ���� ȹ��

	CPlayer::WEAPON_TYPE Get_Weapon_Model_Index() { return m_pItem_Manager->Get_Weapon_Model_Index(); } // ���� ��� ���� ������ �� ��ȣ ����

	const CItem_Manager::ITEM* Get_Equip_Item_Info(EQUIP_SLOT eSlot) { return m_pItem_Manager->Get_Equip_Item_Info(eSlot); } // ���� ���â�� ������ �������� ���� ���۷��� ȹ��

	// ����
	ITEM_RESULT Add_Coin(_int iAdd, _bool bForce) { return m_pItem_Manager->Add_Coin(iAdd, bForce); }
	_int Get_Coin() { return m_pItem_Manager->Get_Coin(); }

	// ���� ������ ����
	_int Change_Potion_Select(_bool bNext) { return m_pItem_Manager->Change_Potion_Select(bNext); }
	_int Change_Tool_Select(_bool bNext) { return m_pItem_Manager->Change_Tool_Select(bNext); }
	_int Get_Potion_Select() { return m_pItem_Manager->Get_Potion_Select(); }
	_int Get_Tool_Select() { return m_pItem_Manager->Get_Tool_Select(); }

	// ���� ����
	_int Change_Weapon() { return m_pItem_Manager->Change_Weapon(); }
	_int Get_Weapon() { return m_pItem_Manager->Get_Weapon(); }

	const CItem_Manager::ITEM* Get_Now_Equip_Weapon_Blade() { return m_pItem_Manager->Get_Now_Equip_Weapon_Blade(); }
	const CItem_Manager::ITEM* Get_Now_Equip_Weapon_Handle() { return m_pItem_Manager->Get_Now_Equip_Weapon_Handle(); }



	// ���� ����
	void Add_Potion_Gauge(_float fAdd) { m_pItem_Manager->Add_Potion_Gauge(fAdd); }
	_bool Use_Potion() { return m_pItem_Manager->Use_Potion(); }

	// ���� �� ����
	void Add_Arm_Gauge(_float fAdd) { m_pItem_Manager->Add_Arm_Gauge(fAdd); }
	_float Get_Arm_Gauge_Ratio() { return m_pItem_Manager->Get_Arm_Gauge_Ratio(); }

#pragma endregion


#pragma region Player_Stat_Manager
	// ���� ���� 
	const _float& Get_NowStat_Normal(STAT_NORMAL eIndex) { return m_pPlayer_Stat_Manager->Get_NowStat_Normal(eIndex); }
	const CPlayer_Stat_Manager::STAT& Get_StatInfo_Normal(STAT_NORMAL eIndex) { return m_pPlayer_Stat_Manager->Get_StatInfo_Normal(eIndex); }

	const _float Get_Now_Max_Ratio(STAT_NORMAL eIndex) { return  m_pPlayer_Stat_Manager->Get_Now_Max_Ratio(eIndex); }
	const _float Get_Max_Limit_Ratio(STAT_NORMAL eIndex) { return  m_pPlayer_Stat_Manager->Get_Max_Limit_Ratio(eIndex); }
	const _float Get_Now_Limit_Ratio(STAT_NORMAL eIndex) { return m_pPlayer_Stat_Manager->Get_Now_Limit_Ratio(eIndex); }

	const CPlayer_Stat_Manager::STAT& Get_StatInfo_DEF(STAT_DEF eIndex) { return m_pPlayer_Stat_Manager->Get_StatInfo_DEF(eIndex); }

	_bool Get_Buff(BUFF_INDEX eIndex) { return m_pPlayer_Stat_Manager->Get_Buff(eIndex); }
	_float Get_Buff_Ratio(BUFF_INDEX eIndex) { return m_pPlayer_Stat_Manager->Get_Buff_Ratio(eIndex); }
	const CPlayer_Stat_Manager::BUFF* Get_Buff_Info(BUFF_INDEX eIndex) { return m_pPlayer_Stat_Manager->Get_Buff_Info(eIndex); }
	void Start_Buff(BUFF_INDEX eIndex) { m_pPlayer_Stat_Manager->Start_Buff(eIndex); }

	// ���� ���� 
	void Add_Stat_Normal(STAT_NORMAL eIndex, _float fValue) { return m_pPlayer_Stat_Manager->Add_Stat_Normal(eIndex, fValue); }
	void Add_StatMax_Normal(STAT_NORMAL eIndex, _float fValue) { return m_pPlayer_Stat_Manager->Add_StatMax_Normal(eIndex, fValue); }

#pragma endregion


private:
	CGameInstance* m_pGameInstance = { nullptr };
	CUIManager* m_pUIManager = { nullptr };
	CItem_Manager* m_pItem_Manager = { nullptr };
	CPlayer_Stat_Manager* m_pPlayer_Stat_Manager = { nullptr };




private: // <- ����̽� 
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END
