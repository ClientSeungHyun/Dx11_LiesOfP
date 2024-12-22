#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)

END

BEGIN(Client)

class CUIPage_Achievment : public CUIPage
{
public:
	enum class PART_GROUP
	{
		GROUP_FRAME,
		GROUP_TITLE,
		GROUP_STATISTIC_FRAME,
		GROUP_STATISTIC_LINE,
		



		GROUP_END
	};

	// �ʿ��� �� 

	/*
	1. ����� + Ÿ��Ʋ ���� 
	2. ��ũ�� ���� 
	3. ����� ���� 
	 3-1. ����� ���� ����
	4. ������ ���� 
	 4-1. ������ ���� ���� 
	*/








protected:
	CUIPage_Achievment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Achievment(const CUIPage_Achievment& Prototype);
	virtual ~CUIPage_Achievment() = default;

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

protected:








public:
	static CUIPage_Achievment* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END