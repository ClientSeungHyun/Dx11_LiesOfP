#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)

END

BEGIN(Client)

class CUIPage_Talking : public CUIPage
{
public:
	enum class PART_GROUP
	{
		TALKING_Back_Fx,

		TALKING_Name_Text,
		TALKING_Line,
		TALKING_Script_0,
		TALKING_Script_1,
		TALKING_Script_2,

		TALKING_Select_Clock_Center,
		TALKING_Select_Clock_Fx,
		TALKING_Select_Clock_Frame,
		TALKING_Select_Clock_Progress,
		TALKING_Select_Clock_Gear_Back,
		TALKING_Select_Clock_Gear_0,
		TALKING_Select_Clock_Gear_1,
		TALKING_Select_Clock_Gear_2,
		TALKING_Select_Clock_Indicator,

		TALKING_Select_Left_Back,
		TALKING_Select_Left_L,
		TALKING_Select_Left_M,
		TALKING_Select_Left_R,
		TALKING_Select_Left_Text,

		TALKING_Select_Right_Back,
		TALKING_Select_Right_L,
		TALKING_Select_Right_M,
		TALKING_Select_Right_R,
		TALKING_Select_Right_Text,



		GROUP_END
	};

	

	typedef struct SCRIPT_INFO
	{
		_wstring strSpeaker = {}; // <- ���ϴ� NPC �̸�
		_wstring strScript_A = {};
		_wstring strScript_B = {};
		_wstring strScript_C = {};

		_float fAlpha = 0.f; // <- ���� ȿ�� ����
	}SCRIPT;

	typedef struct NPC_INFO
	{
		NPC_INFO() {}
		~NPC_INFO() 
		{
			for (auto& iter : vecScript)
				Safe_Delete(iter);

			vecScript.clear();
		}

		SCRIPT* Get_NowScript()
		{
			return vecScript[iScript_Num];
		}

		_bool Add_ScriptNum()
		{
			if (iScript_Num < vecScript.size() - 1)
			{
				++iScript_Num;
				return true;
			}
				
			return false;
		}

		_bool Set_ScriptNum(_int iIndex)
		{
			if ((iIndex < 0) || (iIndex >= vecScript.size()))
				return false;

			iScript_Num = iIndex;

			return true;
		}

		_wstring strName = {};
		
		_int iScript_Num = 0;

		_bool bIsActive = false;

		vector<SCRIPT*> vecScript;
	}NPC;



protected:
	CUIPage_Talking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Talking(const CUIPage_Talking& Prototype);
	virtual ~CUIPage_Talking() = default;

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

public:
	const vector<UPART*>& Get_UIPartInfo() { return m_vecPart; }
	virtual HRESULT Ready_UIPart_Group_Control() override;

	void Show_Script(NPC_SCRIPT eNPC, _int iScriptNum = -1);
	void Next_Script();
	void OFF_Script();

	void Show_Select_Script(_wstring strLeft, _wstring strRight, _float fTime);
	_bool IsLeft_LastSelect_Result() { return m_bLastSelect_Result_IsLeft; }

protected:
	void Update_Script();
	void Update_Select();

protected:
	NPC_SCRIPT m_eNowNpc = NPC_SCRIPT::SCR_END;
	_bool m_bLastSelect_Result_IsLeft = false;
	vector<NPC*> m_vecNpc_ScriptInfo;






public:
	static CUIPage_Talking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END