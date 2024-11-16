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
	}OR_HOST;

	

	typedef struct ORTHO_RENDER_CTRL
	{
		_float fDistance_From_Cam = 0.f;
		_float2 fPosition = { 0.f,0.f };
		PART_GROUP eType = PART_GROUP::GROUP_END;
		_float fRatio = 0.f;
		_wstring strText = {};
		_int iTexture = -1;

		bool operator<(const ORTHO_RENDER_CTRL& other) const 
		{
			if (fDistance_From_Cam < other.fDistance_From_Cam)
				return true;
			else if (_int(eType) > _int(other.eType))
				return true;

			return false; // ī�޶�� �Ÿ��� �� �� ���� ���� �׷�������
		}
	}OR_RENDER;

	struct Order_Ortho_UI_Render {
		bool operator()(const ORTHO_RENDER_CTRL* First, const ORTHO_RENDER_CTRL* Second)
		{
			if (First->fDistance_From_Cam < Second->fDistance_From_Cam)
				return true;
			else if (_int(First->eType) > _int(First->eType))
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

public:
	const vector<UPART*>& Get_UIPartInfo() { return m_vecPart; }
	virtual HRESULT Ready_UIPart_Group_Control() override;

	void Register_Pointer_Into_OrthoUIPage(UI_ORTHO_OBJ_TYPE eType, void* pObj);

	HRESULT Render_Ortho_UI(class CUIRender_Client* pRender_Client);

private:
	void Initialize_Ortho_Info(); // <- ���� UI ����� ���� �⺻ ������ �����Ѵ� 

	void CheckHost(_float fTimeDelta); // <- ��� ��ü�� ���鼭 �׷��� �ϴ� ���� �ִ� �� üũ 

	// ������ ���� ���� UI ���� ���� �Լ� ���� �� 
	void Make_Monster_HP_Bar(CGameObject* pHost, _float fTimeDelta, _float fDistance);
	void Make_Monster_Focusing(CGameObject* pHost, _float fTimeDelta, _float fDistance);
	void Make_Monster_SpecialHit(CGameObject* pHost, _float fTimeDelta, _float fDistance);








	_bool Make_OrthoGraphy_Position(CGameObject* pHost, PART_GROUP eGroup, _float2* fPosition); // <- ���� ��ǥ�� �̴´�, ���� UI Ÿ�� �� ����ġ�� �����Ѵ� 
	_float Check_Distance_From_Cam(CGameObject* pHost);

protected:
	list<OR_HOST*> m_Ortho_Host_list; // <- ���� UI �� �ʿ��� ��� ���
	priority_queue<OR_RENDER*, vector<OR_RENDER*>, Order_Ortho_UI_Render> m_queue_Ortho_Render_Ctrl; // <- �̹� �����ӿ� ���� ���� �ؾ� �ϴ� ����
	vector<UG_CTRL*> m_vecOrtho_Group_Ctrl; // <- ���� UI ������ ���� ��Ʈ�� ����ü ����
	vector<_float3> m_vecOrtho_Adjust; // <- ���� ��ǥ ���� �� ��� ��ü Pos���� ���� �󸶳� ������ ��ġ�� �׸� ������ ���س��´� 

	_float m_fTimeDelta = 0.f;

public:
	static CUIPage_Ortho* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END