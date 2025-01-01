#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)

END

BEGIN(Client)
class CUITutorial_Info;
class CUITutorial_Guide;
class CUITutorial_Timing;
class CUITutorial_Result;
class CUITutorial_Popup;

class CUIPage_Tutorial : public CUIPage
{
public:
	enum class PART_GROUP
	{
		GROUP_CENTER,
		GROUP_INFO_FRAME,
		GROUP_INFO_MISSION,
		GROUP_INFO_SCORE,
		GROUP_GUIDE,
		GROUP_TIMING,
		GROUP_RESULT,
		GROUP_POPUP,
		GROUP_END
	};

	typedef struct
	{
		_int iCapterIndex = -1;
		_wstring strTitle = {};
		_wstring strDescA = {};
		_wstring strDescB = {};

		_int iKey_Texture[6] = { -1, };
		_wstring strKey_Desc[3] = { {}, };
	}TUTO_CHAPTER; // Guide, Popup���� ���

	typedef struct
	{
		_int iCapterIndex = -1;
		_int iMissionIndex = -1;
		_wstring strTitle = {};

		_float fNow = 0;
		_float fGoal = -1;

		_bool bComplete = false;
	}TUTO_MISSION; // Info���� ���



protected:
	CUIPage_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Tutorial(const CUIPage_Tutorial& Prototype);
	virtual ~CUIPage_Tutorial() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OpenAction() override;
	virtual void CloseAction() override;

	virtual CHECK_MOUSE Check_Page_Action(_float fTimeDelta) override;

	void Close_Popup();
	void Open_Popup();
	
	_bool Get_TutorialPopup_Open() { return m_bPopupOpen; }

public:
	const vector<UPART*>& Get_UIPartInfo() { return m_vecPart; }
	virtual HRESULT Ready_UIPart_Group_Control() override;

protected:
	void Initialize_Tutorial();
	void Data_Setting();
	_int Check_KeyTexture_Index(_wstring strKeyName);

	_bool Set_Crtl_Guide(vector<UPART*>* Part, TUTO_CHAPTER& Data, _int iIndex);


	void Update_Tutorial();

	// �ܰ� �Ѿ�� 
	void Next_Chapter();
	


	// UI ��ü ���� 
	void Update_Tutorial_Info(_float fTimeDelta);
	void Update_Tutorial_Guide(_float fTimeDelta);
	void Update_Tutorial_Timing(_float fTimeDelta);
	void Update_Tutorial_Result(_float fTimeDelta);
	void Update_Tutorial_Popup(_float fTimeDelta);
	void Update_Tutorial_NowChapter(_float fTimeDelta);

	void ShowTiming(KEY eKey, _float fTime);

	void Test_Control(_float fTimeDelta);

	// ���� �ൿ Ȯ�� 
	void Check_Mission_Complete(_float fTimeDelta);

	void Check_Player_Move(_float fTimeDelta); // �̵��ϱ� �̼�
	void Check_Player_Dash(_float fTimeDelta); // ȸ�� �̼�
	void Check_Player_Lbutton_Attack(); // �Ϲ� ���� 
	void Check_Player_RButton_Attack(); // ������
	void Check_Player_Fable_Art(); // ���� ���̺� ���� ���
	void Check_Dummy_Weakness(); // �׷α� ����
	void Check_Dummy_Get_FatalAttack(); // ����Ż ���� ����
	void Check_Player_Switch_Weapon(); // ���� ����
	void Check_Player_Guard(); // ����, ����Ʈ ����
	void Check_Player_Resion_Arm_Normal(); // ���� �� ����
	void Check_Player_Resion_Arm_Skill(); // ���� �� ����, ���� �� ����

protected:
	vector<class CUITutorial_Info*> m_vecMission_Info = { nullptr };
	class CUITutorial_Guide* m_pGuide = { nullptr };
	class CUITutorial_Timing* m_pTiming = { nullptr };
	class CUITutorial_Result* m_pResult = { nullptr };
	class CUITutorial_Popup* m_pPopup = { nullptr };

	vector<TUTO_CHAPTER*> m_vecTutorial_ChapterData;
	vector<TUTO_MISSION*> m_vecTutorial_MissionData;

	_bool m_bPopupOpen = false; // <- �˾� ���� ����
	_bool m_bStart = false; // <- ���� �ȳ�
	_bool m_bResult = false; // <- ���� �ȳ� 

	_bool m_bNewChapter = true;

	_int m_iNowChapter = -1;
	_int m_iNowMission = 0;
	_int m_iNow_Index = 0;

	_float m_fKeyHeight = 30.f;

public:
	static CUIPage_Tutorial* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END