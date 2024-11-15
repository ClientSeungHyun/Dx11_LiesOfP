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
		GROUP_FOCUS,
		GROUP_SPECIAL_HIT,
		GROUP_END
	};

	typedef struct ORTHO_HOST
	{
		UI_ORTHO_TYPE eType = UI_ORTHO_TYPE::ORTHO_END;
		const CGameObject* pHost = { nullptr };
	}OR_HOST;

	typedef struct ORTHO_RENDER_CTRL
	{
		_float fDistance_From_Cam = 0.f;
		UPART* pPartInfo = { nullptr };

		bool operator<(const ORTHO_RENDER_CTRL& other) const 
		{
			return fDistance_From_Cam < other.fDistance_From_Cam; // ī�޶�� �Ÿ��� �� �� ���� ���� �׷�������
		}
	}OR_RENDER;

	/*
	0. ��ü�� ������ �� �����͸� ���� ���� 
	1. ���� ���� ����� ��ȸ�ϸ鼭 ���� ���� ���� 
	 1) ���� ���� ���� 
	 2) ���� ������ ���� UI ��ü ���� 
	 3) ���� ���� ���� �� ��ġ ��� 
	 4) ���� ������ ���� ��� ���Ͽ� OR_RENDER�� ��� 
	 5) OR_RENDER vector�� 20���� �̸� ����� ��, ���� �� �����ӿ� �׷��� �ϴ� �� �̰��� �Ѿ�� 10�� �� ����� 
	2. OR_RENDER�� ��ϵ� ������ ���� �������� Stable_sort 
	 1) Render ���η� ���� (true�� �տ� ������)
	 2) ī�޶�� �Ÿ��� ����� ���� ���߿� �׷�������
	3. UIRender_client���� ���� �����Ͽ� �׸�
	4. �׸� �� OR_RENDER�� render�� off ó��

	
	*/


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

	priority_queue<OR_RENDER*>* Get_Ortho_Render_Ctrl() { return &m_queue_Ortho_Render_Ctrl; } // <- Render_Client���� ���� �̿� �� pop�� �����ؾ� �� (�� �׷��� ���� ���� �� �����)

	void Register_Pointer_Into_OrthoUIPage(UI_ORTHO_TYPE eType, const void* pObj); 

private:
	void Initialize_Ortho_Info(); // <- ���� UI ����� ���� �⺻ ������ �����Ѵ� 
	void Make_OrthoGraphy_Position(const CGameObject* pHost, PART_GROUP eGroup, _float2* fPosition); // <- ���� ��ǥ�� �̴´�, ���� UI Ÿ�� �� ����ġ�� �����Ѵ� 

protected:
	list<OR_HOST*> m_Ortho_Host_list; // <- ���� UI �� �ʿ��� ��� ���
	priority_queue<OR_RENDER*> m_queue_Ortho_Render_Ctrl; // <- �̹� �����ӿ� ���� ���� �ؾ� �ϴ� ����
	vector<UG_CTRL*> m_vecOrtho_Group_Ctrl; // <- ���� UI ������ ���� ��Ʈ�� ����ü ����
	vector<_float3> m_vecOrtho_Adjust; // <- ���� ��ǥ ���� �� ��� ��ü Pos���� ���� �󸶳� ������ ��ġ�� �׸� ������ ���س��´� 

public:
	static CUIPage_Ortho* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END