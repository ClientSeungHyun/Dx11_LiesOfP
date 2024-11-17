#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

#include "UIPage_Defines.h"
#include "UIRender_Client.h"
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


		// ���� �÷��� ȭ�� �׽�Ʈ��
		// �Ʒ� ������ ���� ���Ǹ� ���� �÷��̾� Ŭ������ ���� �� 

		_int Next_Potion() {++iSelect_Potion; if (iSelect_Potion >= 3) iSelect_Potion = 0; return iSelect_Potion;}
		_int Next_Tool() { ++iSelect_Tool; if (iSelect_Tool >= 3) iSelect_Tool = 0; return iSelect_Tool; }

		_float Add_Arm_Max(_float fAdd) { fArm_Max += fAdd; fArm_Max = min(fArm_Max, fArm_Max_Limit); return fArm_Max;}
		_float Add_Arm(_float fAdd) { fArm_Now += fAdd; fArm_Now = min(fArm_Now, fArm_Max); return fArm_Now;}
		_float Add_Durable(_float fAdd) { fDurable_Now += fAdd; fDurable_Now = min(fDurable_Now, fDurable_Max); return fDurable_Now;}

		_int Add_Coin(_int iAdd) { iCoin += iAdd; if (iCoin < 0) iCoin = 0; return iCoin;}

		void Swich_Weapon() 
		{
			if (bIsFirstWeapon) bIsFirstWeapon = false;
			else bIsFirstWeapon = true;

			if (bIsFirstWeapon)
			{
				bCell_Frame[0] = bCell_Frame[2] = true;
				bCell_Frame[3] = bCell_Frame[5] = false;
			}
			else 
			{
				bCell_Frame[0] = bCell_Frame[2] = false;
				bCell_Frame[3] = bCell_Frame[5] = true;
			}

			for (_int i = 0; i < 6; ++i)
				bCell_Fill[i] = true;
		}

		void Use_Blade() { bCell_Fill[0] = bCell_Fill[1] = bCell_Fill[2] = false; }
		void Use_Handle() { bCell_Fill[3] = bCell_Fill[4] = bCell_Fill[5] = false; }

		_int iSelect_Potion = 0;
		_int iPotion[3] = { 11,22,33 };

		_int iSelect_Tool = 0;
		_int iTool[3] = { 44,55,66 };

		_bool bIsPotion = true;

		_float fArm_Max_Limit = 1000.f;
		_float fArm_Max = 1000.f;
		_float fArm_Now = 500.f;

		_int iCoin = 12345;

		_float fDurable_Max = 1000.f;
		_float fDurable_Now = 500.f;

		_bool bIsFirstWeapon = true;

		_bool bCell_Frame[6] = { true,true ,true ,false,true ,false };
		_bool bCell_Fill[6] = { true,true ,true ,true,true ,true };

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
	void Update_UIManager(_float fTimeDelta);
	void Update_UIControl(_float fTimeDelta);

private:
	void UIControl_Test(_float fTimeDelta);
	void UIControl_Main(_float fTimeDelta);
	void UIControl_Loading(_float fTimeDelta);
	void UIControl_Play(_float fTimeDelta);
	void UIControl_Inven(_float fTimeDelta);

public:
#pragma region Page_Main
	void OpenMainPage();
#pragma endregion

#pragma region Page_Loading
	void OpenLoadingPage();
#pragma endregion

#pragma region PlayMode
	// �÷��� ��� ����
	_bool GetPlayMode() { return m_bPlayMode; }
	void SetPlayMode(_bool bIsPlayMode)
	{
		m_vecPage[_int(UIPAGE::PAGE_LOADING)]->SetRender(false);
		m_bPlayMode = bIsPlayMode;
	}

	// ȭ�� ��ȯ 
	void SwicthPage(UIPAGE eNextPage);



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

	void InputTestPageInfo(vector<_wstring>* pName, vector<_wstring>* pValue, _wstring DataNameA, TEST_PAGE_VALUE_TYPE eTypeA, const void* ValueA);

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
	CUIPage_Test* m_pUIPage_Test = { nullptr };
	// ����
	CUIPage_Ortho* m_pUIPage_Ortho = { nullptr };



	CUIRender_Client* m_pUIRender_Client = { nullptr };

	// �÷��� ��� ���� 
	_bool m_bPlayMode = false;

	// test code
#ifdef _DEBUG
	_int m_iFonttest = 0; // -1 : ����, 0 : off, 1 : �ѱ�
#endif

#pragma region TestData
	TESTDATA* m_pTestData = { nullptr }; // UI �� ��Ÿ �׽�Ʈ�� ���� �ӽ� ������ ����ü 
	// GameInterface �̱������� �ƹ��� ���� �� ������ ������ 
	// ���� �����ʹ� �̷��� ����/���� �Ұ����ϵ��� ���� ����

	vector<_float2> m_vecTestPage_Pos;
	vector<_float2> m_vecTestPage_Size;
	vector<_float2> m_vecTestPage_ClickPos;

	vector<_bool> m_vecTestPageOpen;
	vector<_bool> m_vecTestPageMove;




#pragma endregion

public:
	static CUIManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END