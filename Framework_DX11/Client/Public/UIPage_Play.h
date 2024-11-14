#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)

END

BEGIN(Client)

class CUIPage_Play : public CUIPage
{
public:
	enum class PART_GROUP
	{
		// �»�� ������ 
		GROUP_HP_STATIC,
		GROUP_HP_FRAME,
		GROUP_HP_FILL,
		GROUP_ST_STATIC,
		GROUP_ST_FRAME,
		GROUP_ST_FILL,
		GROUP_SP0_FRAME,
		GROUP_SP0_FILL,
		GROUP_SP1_FRAME,
		GROUP_SP1_FILL,
		GROUP_SP2_FRAME,
		GROUP_SP2_FILL,
		GROUP_SP3_FRAME,
		GROUP_SP3_FILL,
		GROUP_SP4_FRAME,
		GROUP_SP4_FILL,

		// ���ϴ� ������
		GROUP_POTION_ITEM, // <- ������ �ؽ���
		GROUP_POTION_RKEY, // <- RŰ (��� ������ �� ǥ��)
		GROUP_POTION_FILL, // <- ���� ������ (��� ���� ������ 0�� �� ǥ��)
		GROUP_POTION_QUEUE, // <- ���� ��⿭

		GROUP_TOOL_ITEM, // <- ������ �ؽ���
		GROUP_TOOL_RKEY, // <- RŰ (��� ������ �� ǥ��)
		GROUP_TOOL_QUEUE, // <- �� ��⿭

		GROUP_SELECT_CELL, // ���� ǥ��

		// ���ϴ� ��
		GROUP_ARM_BACK,
		GROUP_ARM_TEX,
		GROUP_ARM_CTRL,
		GROUP_ARM_NAME,
		GROUP_ARM_GAUGE_FRAME,
		GROUP_ARM_GAUGE_FILL,

		// ���� ���� 
		GROUP_COIN_BACK,
		GROUP_COIN_COUNT,

		// ���ϴ� ����
		GROUP_WEAPON_DURABLE_FRAME,
		GROUP_WEAPON_DURABLE_FILL,
		GROUP_WEAPON_BACK,
		GROUP_WEAPON_NORMAL_BACK, // �Ϲ� ���� (�� + �ڷ�) ǥ��
		GROUP_WEAPON_NORMAL_BLADE,
		GROUP_WEAPON_NORMAL_HANDLE,
		GROUP_WEAPON_SPECIAL_BACK, // ����� ���� ǥ��
		GROUP_WEAPON_SPECIAL_TEX,
		GROUP_WEAPON_EQUIP_NUM, // 1,2�� ���� ǥ��

		GROUP_WEAPON_SPECIAL_FRAME_0,  // �� + �ڷ��� Ư���ൿ ǥ�� 
		GROUP_WEAPON_SPECIAL_FILL_0,  // 0~2 : �� ���, 3~5 : �ڷ� ��� 
		GROUP_WEAPON_SPECIAL_FRAME_1,  // Ư���ൿĭ�� 1���� �ʿ��� ��� 1,4���� ����
		GROUP_WEAPON_SPECIAL_FILL_1,
		GROUP_WEAPON_SPECIAL_FRAME_2,
		GROUP_WEAPON_SPECIAL_FILL_2,
		GROUP_WEAPON_SPECIAL_FRAME_3,
		GROUP_WEAPON_SPECIAL_FILL_3,
		GROUP_WEAPON_SPECIAL_FRAME_4,
		GROUP_WEAPON_SPECIAL_FILL_4,
		GROUP_WEAPON_SPECIAL_FRAME_5,
		GROUP_WEAPON_SPECIAL_FILL_5,

		GROUP_END
	};

protected:
	CUIPage_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Play(const CUIPage_Play& Prototype);
	virtual ~CUIPage_Play() = default;

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

	

	// �»�� �������� ���� �Ŵ����� ������ �����Ͽ� ���� 
	// �ܺ� -> ���� �Ŵ��� -> UI �Ŵ���(Page_Play)

	// ���ϴ� ����/���� 
	void Move_SelectCtrl(_bool bIsUp);
	void Swich_ToolItem();

private:
	void LU_Gauge_Update(_float fTimeDelta);
	void LD_Potion_Tool_Update(_float fTimeDelta);
	void LD_Arm_Update(_float fTimeDelta);
	void RU_Coin_Update(_float fTimeDelta);
	void RD_Weapon_Update(_float fTimeDelta);
	
private:
	

private:
	

	// �Ʒ� �ִ� ������ Page�� �ִ� UG_CTRL ����Ʈ�� ������ ���� �����Ͽ� ����Ѵ� 
	// ���� �Ʒ� ���� �̿ܿ� ���� �߰� ���縦 �����ϸ� �ȵǸ� ������ �ÿ��� ���� Delete ó������ �ʴ´� 

	// ü�¹� ����
	UG_CTRL* m_HP_Gauge_Frame = { nullptr };
	UG_CTRL* m_HP_Gauge_Fill = { nullptr };

	// ���¹̳� ����
	UG_CTRL* m_Stamina_Gauge_Frame = { nullptr };
	UG_CTRL* m_Stamina_Gauge_Fill = { nullptr };

	// Ư�� ��ų �� ����
	vector<UG_CTRL*> m_vecSpecial_Gauge_Frame;
	vector<UG_CTRL*> m_vecSpecial_Gauge_Fill;

	// ���ϴ� ����/���� ����
	UG_CTRL* m_Potion_Tool_Tex = { nullptr };
	UG_CTRL* m_Potion_Tool_RKey = { nullptr };
	UG_CTRL* m_Potion_Queue = { nullptr };
	UG_CTRL* m_Potion_Gauge = { nullptr };
	UG_CTRL* m_Tool_Queue = { nullptr };
	UG_CTRL* m_Portion_Tool_Select = { nullptr };

	// ���ϴ� �� 
	UG_CTRL* m_Arm_Tex = { nullptr };
	UG_CTRL* m_Arm_Name = { nullptr };
	UG_CTRL* m_Arm_Frame = { nullptr };
	UG_CTRL* m_Arm_Fill = { nullptr };

	// ���� ���� 
	UG_CTRL* m_Coin_Count = { nullptr };

	// ���ϴ� ���� 
	UG_CTRL* m_Weapon_Durable_Frame = { nullptr };
	UG_CTRL* m_Weapon_Durable_Fill = { nullptr };

	UG_CTRL* m_Weapon_Normal_Back = { nullptr };
	UG_CTRL* m_Weapon_Normal_Tex = { nullptr };
	UG_CTRL* m_Weapon_Special_Back = { nullptr };
	UG_CTRL* m_Weapon_Special_Tex = { nullptr };

	UG_CTRL* m_Weapon_Equip_Num = { nullptr };

	vector<UG_CTRL*> m_vecWeapon_Special_Frame;
	vector<UG_CTRL*> m_vecWeapon_Special_Fill;

	


public:
	static CUIPage_Play* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END