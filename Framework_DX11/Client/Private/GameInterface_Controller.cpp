#include "stdafx.h"
#include "..\Public\GameInterface_Controller.h"



IMPLEMENT_SINGLETON(CGameInterface_Controller)

CGameInterface_Controller::CGameInterface_Controller()
{
}

HRESULT CGameInterface_Controller::Initialize_GameInterface(ID3D11Device** pDevice, ID3D11DeviceContext** pContext, CGameInstance* pInstance)
{
	m_pGameInstance = pInstance;
	Safe_AddRef(m_pGameInstance);

	m_pDevice = *pDevice;
	m_pContext = *pContext;



	// UIManager
	m_pUIManager = CUIManager::Create(m_pDevice, m_pContext);

	// Item_Manager
	m_pItem_Manager = CItem_Manager::Create(m_pGameInstance);

	// Stat_Manager
	m_pPlayer_Stat_Manager = CPlayer_Stat_Manager::Create(m_pGameInstance);




	return S_OK;
}

void CGameInterface_Controller::Update_GameInterface(_float fTimeDelta)
{
	// �׽�Ʈ�� 
	if (KEY_HOLD(KEY::ALT))
	{
		if (KEY_TAP(KEY::F3))
		{
			_int iItem = _int(m_pGameInstance->Get_Random(0.f, 3.1f));
			if (iItem == 0)
				m_pItem_Manager->AddNewItem_Inven(255);
			if (iItem == 1)
				m_pItem_Manager->AddNewItem_Inven(247);
			if (iItem == 2)
				m_pItem_Manager->AddNewItem_Inven(277);
			if (iItem == 3)
				m_pItem_Manager->AddNewItem_Inven(279);
		}
	}

	// �Ŵ��� ������Ʈ 
	m_pItem_Manager->Update_Item(fTimeDelta); 
	m_pUIManager->Update_UIManager(fTimeDelta);

	// ������ ���� �˾� 
	const CUIPage::ITEMACTION_INFO* pNowAction = m_pUIManager->Get_ItemAction_Info(); // ���� UI���� ��û�� ������ ���� �˾� ���� ȹ��

	if (m_pUIManager->Get_ActiveFunc() != ITEM_FUNC::FUNC_END) // UI �Ŵ������� �˾��� ���� ���ۿ�û�� Ȯ�ε�
	{
		m_pItem_Manager->Operate_ItemAction(m_pUIManager->Get_ActiveFunc(), pNowAction->vAction_Pos, pNowAction->vAction_Size); // ������ �Ŵ����� ��û ������ ����

		if (m_pItem_Manager->IsReset_ItemAction()) // ������ �Ŵ����� Ȱ�� ���, �˾��� ����Ǿ��� �� Ȯ�� 
			m_pUIManager->Reset_ItemAction(); // �˾��� ����� ��� UI �Ŵ��������� ��û ������ ��� ���� 
	}
			
	if (pNowAction == nullptr) // UI �Ŵ����� ��û ������ ������ ��� 
		m_pItem_Manager->Reset_ItemAction(); // ������ �Ŵ����� �˾� ���� ���� 
	else if (pNowAction->eAction_Array_Type != INVEN_ARRAY_TYPE::TYPE_END) // UI �Ŵ����� ��û�� ���â���� �߻� 
		m_pItem_Manager->Set_ItemAction(pNowAction->eAction_Array_Type, pNowAction->iAction_Array_Index, pNowAction->vAction_Pos, pNowAction->vAction_Size, true);
	else if (pNowAction->eAction_Equip_Slot != EQUIP_SLOT::EQUIP_END) // UI �Ŵ����� ��û�� �κ����� �߻� 
		m_pItem_Manager->Set_ItemAction(pNowAction->eAction_Equip_Slot, pNowAction->vAction_Pos, pNowAction->vAction_Size, true);
	else
		m_pItem_Manager->Reset_ItemAction(); // ��û ���� ������ nullptr�� �ƴ����� ��������(���â, �κ� �� �� �ϳ����� ��)-> ���� ���� 

	// ��� ���� ���뿡 ���� ��� ���� ���� ���� 
}

void CGameInterface_Controller::Release_GameInterface()
{

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pUIManager);
	Safe_Release(m_pItem_Manager);
	Safe_Release(m_pPlayer_Stat_Manager);
	//m_pPlayer = nullptr;

	GET_GAMEINTERFACE->Destroy_Instance();
}




void CGameInterface_Controller::Free()
{
	__super::Free();




}