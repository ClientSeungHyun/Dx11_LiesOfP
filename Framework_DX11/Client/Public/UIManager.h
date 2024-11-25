#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

#include "UIPage_Defines.h"
#include "UIRender_Batching.h"
#include "Interface_Enums.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIManager : public CUIObject
{
public:
	typedef struct TESTDATA
	{
		// ���� UI �׽�Ʈ�� 
		_float fHP_Now = 500.f;
		_float fHP_Max = 1000.f;

		_float fHP_Damege = 100.f;

		_bool bFocus = true;
		_bool bSpecial_Attack = true;
	}TDATA;





private:
	CUIManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIManager(const CUIManager& Prototype);
	virtual ~CUIManager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Input_Render_Info(CUIRender_Batching::UIRENDER_INFO* pInfo) { m_pUIRender_Batching->Input_Render_Info(pInfo); }

	void Set_Scroll_Y_Offset(SCROLL_AREA eArea, _float fOffset) { m_pUIRender_Batching->Set_Scroll_Y_Offset(eArea, fOffset); }

	void Update_UIManager(_float fTimeDelta);
	void Update_UIControl(_float fTimeDelta);

	void Update_TestPage(_float fTimeDelta);

	_Vec2 CheckMouse(_Vec2 fPos, _Vec2 fSize);

	_bool Action_InterAction(_wstring strInterName) { return m_pUIPage_Play->Action_InterAction(strInterName); }

	void Set_Scroll_Area(SCROLL_AREA eArea, _Vec2 vPos, _Vec2 vSize) { m_pUIRender_Batching->Set_Scroll_Area(eArea, vPos, vSize); }
	void Select_Scroll_Area(SCROLL_AREA eArea) { m_pUIRender_Batching->Select_Scroll_Area(eArea); }

	void SetIngame(_bool bTrue) 
	{ 
		m_bIsIngame = bTrue;
		if (bTrue)
		{
			for (_int i =0; i < m_vecPage.size(); ++i)
			{
				if (i <= _int(UIPAGE::PAGE_LOADING))
				{
					m_vecPage[i]->SetUpdate(false);
					//m_vecPage[i]->SetRender(false);
				}
				else
				{
					m_vecPage[i]->SetUpdate(true);
					//m_vecPage[i]->SetRender(true);
				}
			}
		}
		else
		{
			for (_int i = 0; i < m_vecPage.size(); ++i)
			{
				if (i <= _int(UIPAGE::PAGE_LOADING))
				{
					m_vecPage[i]->SetUpdate(true);
					//m_vecPage[i]->SetRender(true);
				}
				else
				{
					m_vecPage[i]->SetUpdate(false);
					//m_vecPage[i]->SetRender(false);
				}
			}
		}
	
	
	} // �Ŵ����� ���� ���� / ������ ���� �˸�

	// ������ ȹ�� 
	void Input_Drop_Item_Info(_int iIndex, _int iCount) { m_pUIPage_Play->Input_Drop_Item_Info(iIndex, iCount); }


private:
	void UIControl_Test(_float fTimeDelta);

	void UIControl_Common(_float fTimeDelta);
	void UIControl_Main(_float fTimeDelta);
	void UIControl_Loading(_float fTimeDelta);
	void UIControl_Play(_float fTimeDelta);
	void UIControl_Menu(_float fTimeDelta);
	void UIControl_Inven(_float fTimeDelta);
	void UIControl_Equip(_float fTimeDelta);
	void UIControl_Stat(_float fTimeDelta);
	void UIControl_LevelUp(_float fTimeDelta);
	void UIControl_Skill(_float fTimeDelta);

public:
#pragma region Page_Main
	void OpenMainPage();
#pragma endregion

#pragma region Page_Loading
	void OpenLoadingPage();
#pragma endregion

#pragma region PlayMode
	// ȭ�� ��ȯ 
	void SwicthPage(UIPAGE ePageA, UIPAGE ePageB);



#pragma endregion


#pragma region Page_Ortho
	void Register_Pointer_Into_OrthoUIPage(UI_ORTHO_OBJ_TYPE eType, void* pObj) { m_pUIPage_Ortho->Register_Pointer_Into_OrthoUIPage(eType, pObj); }


#pragma endregion

	TDATA* GetTestData() { return m_pTestData; } // �׽�Ʈ�� ������ ���� ��� ���� �־� �δ� �Լ�
	// ���� �ż��� ������ ���� �׽�Ʈ�� ������ ��� ���⿡ �ְ� ���� �ѹ��� �����ϱ� 
	// �� �Լ��� ���õ� ���� �׽�Ʈ�� �ӽ� ������ �ٷ�Ƿ� ���Ǽ��� �߽��� ������ ���� ���� ���� ���� ������ ������ �ȵ� 

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
	); // �Լ� ����ϸ� �Է� ���� ��� �ش� �����ӿ� â�� ��� �ش�, DataNameA: �������� �̸�, TEST_PAGE_VALUE_TYPE: �ڷ���(float,int,tchar), ValueA: ���� �޾ƿ� ������ <- �̷��� ����
	// �� ������ ���� �Լ��� �������� �۵��� (Gameinterface�� �ܺ� ������ ����/�������� �ʵ��� �ϱ� ����)




private:
	HRESULT Load_UIDataFile();
	HRESULT Make_UIPage(_int iIndex);
	HRESULT Load_UIDataFile_Part(HANDLE handle, DWORD* dword, _int iIndex);

	void Setting_TestPage();

	void InputTestPageInfo(TEST_PAGE_NAME eName, _wstring DataNameA, TEST_PAGE_VALUE_TYPE eTypeA, const void* ValueA, _int iIndex);

	void Open_Close_Page(UIPAGE ePage); // ���� �ְų� ������ ���̸� �ݰ�, ���� �ְų� ������ ���̸� ���� 
	void OpenPage(UIPAGE ePage); // ���Ȱų� ������ ���̸� �����ϰ� �ƴϸ� ���� 
	void ClosePage(UIPAGE ePage); // �����ų� ������ ���̸� �����ϰ� �ƴϸ� �ݴ´� 

	

private:
	vector<CUIPage*> m_vecPage; // <- ������ ������

	// ������ �� ������
	// ĳ������ ���� �ʱ� ���� �߰�
	// AddRef���� �ʰ� ���

	// ����
	CUIPage_Main* m_pUIPage_Main = { nullptr };
	// �ε�
	CUIPage_Loading* m_pUIPage_Loading = { nullptr };
	// �÷���
	CUIPage_Play* m_pUIPage_Play = { nullptr };
	// �޴�
	CUIPage_Menu* m_pUIPage_Menu = { nullptr };
	// �κ�
	CUIPage_Inven* m_pUIPage_Inven = { nullptr };
	// ���
	CUIPage_Equip* m_pUIPage_Equip = { nullptr };
	// ����
	CUIPage_Stat* m_pUIPage_Stat = { nullptr };
	// ������
	CUIPage_LevelUp* m_pUIPage_LevelUp = { nullptr };
	// ��ųƮ��
	CUIPage_Skill* m_pUIPage_Skill = { nullptr };
	// �׽�Ʈ
	CUIPage_ToolTip* m_pUIPage_ToolTip = { nullptr };
	// ����
	CUIPage_Ortho* m_pUIPage_Ortho = { nullptr };


	CUIRender_Batching* m_pUIRender_Batching = { nullptr };

	_bool m_bIsIngame = false;


	// �÷��� ��� ���� 
	//_bool m_bPlayMode = false;

	// test code
#ifdef _DEBUG
	_int m_iFonttest = 0; // -1 : ����, 0 : off, 1 : �ѱ�
#endif

#pragma region TestData
	TESTDATA* m_pTestData = { nullptr }; // UI �� ��Ÿ �׽�Ʈ�� ���� �ӽ� ������ ����ü 
	// GameInterface �̱������� �ƹ��� ���� �� ������ ������ 
	// ���� �����ʹ� �̷��� ����/���� �Ұ����ϵ��� ���� ����

	vector<_Vec2> m_vecTestPage_Pos;
	vector<_Vec2> m_vecTestPage_Size;
	vector<_Vec2> m_vecTestPage_ClickPos;

	vector<_bool> m_vecTestPageOpen;
	vector<_bool> m_vecTestPageMove;

	vector<vector<vector<_wstring>>> m_vecTestPageInfo;



#pragma endregion

public:
	static CUIManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END