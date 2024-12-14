#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)

END

BEGIN(Client)

class CUIPage_Inven : public CUIPage
{
public:
	enum class PART_GROUP
	{
		GROUP_BACK,
		GROUP_TOP_STATIC,
		GROUP_TOP_MOUSE,
		GROUP_TOP_TEXT,

		GROUP_TOP_HIGHLIGHT,

		GROUP_WEIGHT_TEXT,
		GROUP_WEIGHT_STAT,
		GROUP_WEIGHT_RATIO,

		GROUP_COIN_COUNT,

		GROUP_ITEMINFO_FRAME,
		GROUP_ITEMINFO_SLIDE,
		GROUP_ITEMINFO_SLIDE_BAR,

		// ������� �ݺ��۾� �ʿ�
		GROUP_ARRAY_FRAME,
		GROUP_ARRAY_TEXT,

		GROUP_CELL_0,
		GROUP_CELL_1,
		GROUP_CELL_2,
		GROUP_CELL_3,
		GROUP_CELL_4,
		// �������

		GROUP_BOX_STATIC,
		GROUP_BOX_COUNT_0,
		GROUP_BOX_COUNT_1,

		GROUP_END
	};

	enum class INVEN_UI_TAP
	{
		TAP_NORMAL_ITEM,
		TAP_INGRADIANT,
		TAP_FUNTION,
		TAP_WEAPON,
		TAP_WEAPON_PART,
		TAP_ARM,
		TAP_DEFENCE,
		TAP_AMULET,
		TAP_COSTUME,
		TAP_GESTURE,
		TAP_COLLECTION,
		TAP_END
	};

	typedef struct ARRAY_RENDER_INFO // � �κ� �迭�� ��� �׸� ���� ���� 
	{
		_float fAdjust_Y = 0.f; // <- ���� ǥ�õ� ��ŭ �������� ��
		_wstring strName = TEXT("none");
		_int iTexture[5] = { -1,-1,-1,-1,-1 };
		_int iTexture_Handle[5] = { -1,-1,-1,-1,-1 };
		_int iTexture_Symbol[5] = { -1,-1,-1,-1,-1 };
	}RENDER;


protected:
	CUIPage_Inven(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Inven(const CUIPage_Inven& Prototype);
	virtual ~CUIPage_Inven() = default;

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

	HRESULT Render_Inven_Array(class CUIRender_Client* pRender_Client); // <- Array ������ Part�� ������Ʈ �� ���� ��û�Ѵ� 

public:
	const vector<UPART*>& Get_UIPartInfo() { return m_vecPart; }
	virtual HRESULT Ready_UIPart_Group_Control() override;

private:
	// action
	void Action_Inven_Page(_float fTimeDelta); // ����� �ϴ� ������ ���� + ��ũ�� Ȯ��
	void Action_Focus(_float fTimeDelta); // �� ���� ���콺�� ���� �� ����, + ���� ������ �۵� ��û�� ����

	// update
	void Update_Top_Part(_float fTimeDelta); // ��� �ǹ�ư ���̶���Ʈ ���� �� ����, ���� ǥ�� ���� 
	void Update_Array_Position(_float fTimeDelta); // ������ ���� ���뿡 ���� Array���� ��ġ �� ���¸� ����
	void Update_Array_Position_Weapon(_float fTimeDelta); // ������ ���� ���뿡 ���� Array���� ��ġ �� ���¸� ����
	void Update_Array_Position_Weapon_Heroic(_float fTimeDelta, _float fAdjust, _int iRowCount, _float fStart); // ������ ���� ���뿡 ���� Array���� ��ġ �� ���¸� ����


	void Update_BoxInfo(_float fTimeDelta); // �ϴ��� ���� ���� ����

	// Part ���� ��
	void Update_Tap_Button(_float fTimeDelta); // ��� �ǹ�ư �̹���, �ؽ�Ʈ ����

	// �����̵� ���� 
	// 
	// 1. �� ������ �� �ִ� �ִ� �Ÿ� 
	// 2. �� ������ �Ÿ�
	// 
	// 3. ������ ����� �⺻ ũ�� 
	// 4. �������� �׷��� �� ���� (Y)
	// 5. ����� ������ �� �ִ� �Ÿ� (4 - 3)
	// 
	// 6. ������ ���� �� ���� 
	// 7. ���콺 ���� ��ġ ��� 
	// 8. 
	// 
	// 
	// 

	// �����̵� ���� 







	void Change_Data_Y_Size(_float fSize);
	
	void Action_Slide(_float fSize);
	void Update_Slide(_float fSize);


	_float m_fData_Adjust_Y_Origin = 0.f;
	
	// Focus ���� 

	void Reset_Focus() { m_vFocus_Pos = { -1.f,-1.f }; m_vFocus_Size = { -1.f,-1.f }; }

	_Vec2 m_vFocus_Pos = { -1.f,-1.f };
	_Vec2 m_vFocus_Size = { -1.f,-1.f };

	// ItemActionInfo 
	_bool m_bReset_ItemAction_Info = false;
	
	// Sound ���� 
	_int m_iNowCell = -1;

private:
	INVEN_UI_TAP m_eNow_Tap = INVEN_UI_TAP::TAP_NORMAL_ITEM;

	_float m_fGap = 2.5f; // Array ��ġ ��� �� �� ��� ������ ���� 

	vector<INVEN_ARRAY_TYPE> m_vecSelected_Array; // ���õ� Tap�� ���� �׷��� �ϴ� Inven_array ���

	queue<RENDER*> m_queueRender; // <- �̹� �����ӿ� �׷��� �ϴ� ����

	_bool m_IsTab_Change = false;

	SCROLL_INFO* m_pScroll = { nullptr };

public:
	static CUIPage_Inven* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END