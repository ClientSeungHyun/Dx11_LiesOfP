#pragma once

#include "Client_Defines.h"
#include "UIPage.h"


BEGIN(Engine)

END

BEGIN(Client)

class CUIPage_Ortho : public CUIPage
{
public:
	enum class PART_GROUP
	{
		GROUP_HP_FRAME,
		GROUP_HP_FILL,
		GROUP_HP_COUNT,
		GROUP_FOCUS,
		GROUP_SPECIAL_HIT,
		GROUP_END
	};

	typedef struct ORTHO_HOST
	{
		UI_ORTHO_OBJ_TYPE eType = UI_ORTHO_OBJ_TYPE::ORTHO_END;
		CGameObject* pHost = { nullptr };
		_bool bIsActive = true;

		_bool bDamegeCount = true;
		_float fAngleFor_Weakness_Alpha = 360.f;

	}OR_HOST;

	

	typedef struct ORTHO_RENDER_CTRL
	{
		_float fDistance_From_Cam = 0.f;
		_Vec2 fPosition = { 0.f,0.f };
		PART_GROUP eType = PART_GROUP::GROUP_END;
		_float fRatio = 0.f;
		_float fWeaknessRatio = 0.f;
		_wstring strText = {};
		_int iTexture = -1;
		_bool bIsWeakness = false;


	}OR_RENDER;

	struct Order_Ortho_UI_Render {
		bool operator()(const ORTHO_RENDER_CTRL* First, const ORTHO_RENDER_CTRL* Second)
		{
			if (First->fDistance_From_Cam < Second->fDistance_From_Cam)
				return true;
			else if ((First->fDistance_From_Cam == Second->fDistance_From_Cam)&&(_int(First->eType) > _int(Second->eType)))
				return true;

			return false; // ī�޶�� �Ÿ��� �� �� ���� ���� �׷�������
		}
	};

protected:
	CUIPage_Ortho(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Ortho(const CUIPage_Ortho& Prototype);
	virtual ~CUIPage_Ortho() = default;

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

	void Register_Pointer_Into_OrthoUIPage(UI_ORTHO_OBJ_TYPE eType, void* pObj);

	HRESULT Render_Ortho_UI();

	void Set_OnOff_OrthoUI(_bool bIsOn, void* pObj);
	

private:
	void Initialize_Ortho_Info(); // <- ���� UI ����� ���� �⺻ ������ �����Ѵ� 

	void CheckHost(_float fTimeDelta); // <- ��� ��ü�� ���鼭 �׷��� �ϴ� ���� �ִ� �� üũ 

	// ������ ���� ���� UI ���� ���� �Լ� ���� �� 
	void Make_Monster_HP_Bar(CGameObject* pHost, _float fTimeDelta, _float fDistance, UI_ORTHO_OBJ_TYPE eType, _float* WeakRatio);
	void Make_Monster_Focusing(CGameObject* pHost, _float fTimeDelta, _float fDistance, UI_ORTHO_OBJ_TYPE eType);
	void Make_Monster_SpecialHit(CGameObject* pHost, _float fTimeDelta, _float fDistance, UI_ORTHO_OBJ_TYPE eType);








	_bool Make_OrthoGraphy_Position(CGameObject* pHost, PART_GROUP eGroup, _Vec2* fPosition, UI_ORTHO_OBJ_TYPE eType); // <- ���� ��ǥ�� �̴´�, ���� UI Ÿ�� �� ����ġ�� �����Ѵ� 
	_float Check_Distance_From_Cam(CGameObject* pHost);

protected:
	list<OR_HOST*> m_Ortho_Host_list; // <- ���� UI �� �ʿ��� ��� ���
	priority_queue<OR_RENDER*, vector<OR_RENDER*>, Order_Ortho_UI_Render> m_queue_Ortho_Render_Ctrl; // <- �̹� �����ӿ� ���� ���� �ؾ� �ϴ� ����
	vector<vector<_Vec3>> m_vecOrtho_Adjust; // <- ���� ��ǥ ���� �� ��� ��ü Pos���� ���� �󸶳� ������ ��ġ�� �׸� ������ ���س��´� 

	_float m_fTimeDelta = 0.f;

public:
	static CUIPage_Ortho* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END