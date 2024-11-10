#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "GameInstance.h"
#include "UIManager.h"


BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CGameInterface_Controller : public CBase
{
	DECLARE_SINGLETON(CGameInterface_Controller)

public:
	typedef struct TESTDATA
	{
		_float fHP_Now = 500.f; // ���� ü�°� 
		_float fMax_HP_Now = 1000.f; // ü�� �ִ�ġ�� ���簪
		_float fMax_HP_Limit = 1000.f; // ü�� �ִ�ġ�� �ִ� ������

		_float fST_Now = 500.f; // ���� ���¹̳��� 
		_float fMax_ST_Now = 1000.f; // ���¹̳� �ִ�ġ�� ���簪
		_float fMax_ST_Limit = 1000.f; // ���¹̳� �ִ�ġ�� �ִ� ������
		
		_float fSP_Now = 100.f; // ���� Ư���ɷ�ġ
		_float fSP_Single_Max = 100.f; // Ư���ɷ�ġ ������ �� ĭ�� �ִ�ġ 
		_int iSP_Gauge_Count = 5; // ���� Ư���ɷ�ġ ������ ĭ 
		_int iSP_Gauge_Count_Max = 5; // Ư���ɷ�ġ ������ �ִ�ġ 

	}TDATA;


private:
	CGameInterface_Controller();
	virtual ~CGameInterface_Controller() = default;






public: // �ܺο��� �Ŵ��� ���� 

#pragma region TestData
	TESTDATA* m_pTestData = { nullptr }; // UI �� ��Ÿ �׽�Ʈ�� ���� �ӽ� ������ ����ü 
	// GameInterface �̱������� �ƹ��� ���� �� ������ ������ 
	// ���� �����ʹ� �̷��� ����/���� �Ұ����ϵ��� ���� ����
#pragma endregion


#pragma region Common
	HRESULT Initialize_GameInterface(ID3D11Device** pDevice, ID3D11DeviceContext** pContext, CGameInstance* pInstance);
	void Update_GameInterface(_float fTimeDelta);
	void Release_GameInterface();
	TDATA* GetTestData() {return m_pTestData;} // �׽�Ʈ�� ������ ���� ��� ���� �־� �δ� �Լ�
	// ���� �ż��� ������ ���� �׽�Ʈ�� ������ ��� ���⿡ �ְ� ���� �ѹ��� �����ϱ� 
	// �� �Լ��� ���õ� ���� �׽�Ʈ�� �ӽ� ������ �ٷ�Ƿ� ���Ǽ��� �߽��� ������ ���� ���� ���� ���� ������ ������ �ȵ� 

#pragma endregion


#pragma region UIManager
	void OpenMainPage() { m_pUIManager->OpenMainPage(); }
	void OpenLoadingPage() { m_pUIManager->OpenLoadingPage(); }

	_bool GetPlayMode() { return m_pUIManager->GetPlayMode(); }
	void SetPlayMode(_bool bIsPlayMode) { m_pUIManager->SetPlayMode(bIsPlayMode); }

	void SwicthPage(UIPAGE eNextPage) { m_pUIManager->SwicthPage(eNextPage); }


#pragma endregion


#pragma region ItemManager



#pragma endregion




private:
	CGameInstance* m_pGameInstance = { nullptr };
	CUIManager* m_pUIManager = { nullptr };





private: // <- ����̽� 
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END
