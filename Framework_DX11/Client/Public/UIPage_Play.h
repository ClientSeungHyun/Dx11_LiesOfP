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
		GROUP_SP1_FRAME,
		GROUP_SP2_FRAME,
		GROUP_SP3_FRAME,
		GROUP_SP4_FRAME,
		GROUP_SP0_FILL,
		GROUP_SP1_FILL,
		GROUP_SP2_FILL,
		GROUP_SP3_FILL,
		GROUP_SP4_FILL,

		// ���ϴ� ������
		GROUP_POTION_FRAME,
		GROUP_TOOL_FRAME,

		GROUP_POTION_ITEM, // <- ������ �ؽ���
		GROUP_POTION_TOOL_RKEY, // <- RŰ (��� ������ �� ǥ��)
		GROUP_POTION_FILL, // <- ���� ������ (��� ���� ������ 0�� �� ǥ��)
		GROUP_POTION_COUNT,
		GROUP_POTION_QUEUE_BACK,
		GROUP_POTION_QUEUE_ITEM,

		GROUP_TOOL_ITEM,
		GROUP_TOOL_COUNT,
		GROUP_TOOL_QUEUE_BACK,
		GROUP_TOOL_QUEUE_ITEM,

		GROUP_SELECT_CELL, // ���� ǥ��

		// ���ϴ� ��������
		GROUP_BAG_FRAMELINE,
		GROUP_BAG_BACK,
		GROUP_BAG_ITEM,
		GROUP_BAG_COUNT,
		GROUP_BAG_NUM,

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
		GROUP_WEAPON_CENTER, // ����â ��ü�� �߽�
		GROUP_WEAPON_DURABLE_FRAME,
		GROUP_WEAPON_DURABLE_FILL,
		GROUP_WEAPON_NORMAL_BACK, // �Ϲ� ���� (�� + �ڷ�) ǥ��
		GROUP_WEAPON_NORMAL_BACK_FX, // �Ϲ� ���� (�� + �ڷ�) ǥ�� (Ư��ȿ��)
		GROUP_WEAPON_NORMAL_BLADE,
		GROUP_WEAPON_NORMAL_HANDLE,
		GROUP_WEAPON_SPECIAL_BACK, // ����� ���� ǥ��
		GROUP_WEAPON_SPECIAL_TEX,
		GROUP_WEAPON_EQUIP_NUM, // 1,2�� ���� ǥ��


		// Ư�� ���� ������ 
		// ���̵�� 3ĭ¥���� ����Ѵ� 
		GROUP_WEAPON_GAUGE_LEFT_SYMBOL, // �� Ư������ ������
		GROUP_WEAPON_GAUGE_RIGHT_SYMBOL, // �ڷ� Ư������ ������

		GROUP_WEAPON_GAUGE_LEFT_SIDE_FRAME, // �� Ư�� ���� ������ ������ (���̵�) 
		GROUP_WEAPON_GAUGE_LEFT_SIDE_WHITE, // �� Ư�� ���� ������ ���� (���̵�)
		GROUP_WEAPON_GAUGE_LEFT_SIDE_FILL, // �� Ư�� ���� ������ Ȱ��ȭ ǥ�� (���̵�)

		GROUP_WEAPON_GAUGE_LEFT_CENTER_FRAME, // �� Ư�� ���� ������ ǥ�� (�߽�)
		GROUP_WEAPON_GAUGE_LEFT_CENTER_WHITE, // �� Ư�� ���� ������ ���� (�߽�)
		GROUP_WEAPON_GAUGE_LEFT_CENTER_FILL,  // �� Ư�� ���� ������ Ȱ��ȭ ǥ�� (�߽�)

		GROUP_WEAPON_GAUGE_RIGHT_SIDE_FRAME, // �ڷ� Ư�� ���� ������ ������ (���̵�) 
		GROUP_WEAPON_GAUGE_RIGHT_SIDE_WHITE, // �ڷ� Ư�� ���� ������ ���� (���̵�)
		GROUP_WEAPON_GAUGE_RIGHT_SIDE_FILL, // �ڷ� Ư�� ���� ������ Ȱ��ȭ ǥ�� (���̵�)

		GROUP_WEAPON_GAUGE_RIGHT_CENTER_FRAME, // �ڷ� Ư�� ���� ������ ǥ�� (�߽�)
		GROUP_WEAPON_GAUGE_RIGHT_CENTER_WHITE, // �ڷ� Ư�� ���� ������ ���� (�߽�)
		GROUP_WEAPON_GAUGE_RIGHT_CENTER_FILL, // �ڷ� Ư�� ���� ������ Ȱ��ȭ ǥ�� (�߽�)

		GROUP_WEAPON_GAUGE_LEFT_KEYSET_A, // �� Ư�� ���� Ű (F)
		GROUP_WEAPON_GAUGE_LEFT_KEYSET_B, // �� Ư�� ���� Ű (Shift+F)

		GROUP_WEAPON_GAUGE_RIGHT_KEYSET_A, // �ڷ� Ư�� ���� Ű (F)
		GROUP_WEAPON_GAUGE_RIGHT_KEYSET_B, // �ڷ� Ư�� ���� Ű (Shift+F)


		// ȹ�� ������ ����
		GROUP_DROP_STATIC,
		GROUP_DROP_ITEM,
		GROUP_DROP_TYPE,
		GROUP_DROP_NAME,

		// ��ȣ�ۿ� �ȳ� 
		GROUP_INTER_STATIC,
		GROUP_INTER_EKEY,
		GROUP_INTER_TEXT,

		// ����� 
		GROUP_DEBUFF_STATIC,
		GROUP_DEBUFF_ICON,
		GROUP_DEBUFF_NAME,
		GROUP_DEBUFF_FILL,

		// ���� ü�¹� 
		GROUP_BOSS_NAME,
		GROUP_BOSS_STATIC,
		GROUP_BOSS_HP_FILL,

		// ����
		GROUP_PLAYER_HP_SECOND,
		GROUP_PLAYER_STAMINA_SECOND,
		GROUP_BOSS_HP_SECOND,

		GROUP_END
	};

	typedef struct DROP_ITEM_INFO
	{
		_int iIndex = -1;
		_int iCount = 0;
		_float fLifeTime = 0.f;
	}DROP;

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

	virtual CHECK_MOUSE Check_Page_Action(_float fTimeDelta) override;

public:
	const vector<UPART*>& Get_UIPartInfo() { return m_vecPart; }
	virtual HRESULT Ready_UIPart_Group_Control() override;

	HRESULT Render_PlayInfo(class CUIRender_Client* pRender_Client);

	_bool Action_InterAction(_wstring strInterName);

	void Input_Drop_Item_Info(_int iIndex, _int iCount)
	{
		DROP_ITEM_INFO* pNew = new DROP_ITEM_INFO;
		pNew->iIndex = iIndex;
		pNew->iCount = iCount;
		pNew->fLifeTime = m_fDrop_Item_ShowTime;
		m_DropItem_Index_list.push_back(pNew);
		if (m_DropItem_Index_list.size() > m_iDrop_Item_Render_Limit)
			while (m_DropItem_Index_list.size() > m_iDrop_Item_Render_Limit)
			{
				Safe_Delete(m_DropItem_Index_list.front());
				m_DropItem_Index_list.pop_front();
			}
	}

	// ���� ü�¹� 
	void Activate_Boss_Hp_Bar(_bool bActive) { m_bIs_BossHp_Activate = bActive; }
	void Set_Boss_Hp_Bar_Info(_wstring strName, _float HpNow, _float HpMax) { m_strBossName = strName, m_fBoss_Hp_Now = HpNow; m_fBoss_Hp_Max = HpMax; }

	// �»�� �������� ���� �Ŵ����� ������ �����Ͽ� ���� 
	// �ܺ� -> ���� �Ŵ��� -> UI �Ŵ���(Page_Play)

	// ���ϴ� ����/���� 

public: 



private:
	void Action_Potion_Tool(_float fTimeDelta);
	void Action_Arm(_float fTimeDelta);
	void Action_Weapon(_float fTimeDelta);

	void LU_Gauge_Update(_float fTimeDelta);
	void LD_Potion_Tool_Update(_float fTimeDelta);
	void LD_Bag_Update(_float fTimeDelta);
	void LD_Arm_Update(_float fTimeDelta);
	void RU_Coin_Update(_float fTimeDelta);
	void RD_Weapon_Update(_float fTimeDelta);

	void Boss_Hp_Update(_float fTimeDelta);

	void PlayInfo_Update(_float fTimeDelta);
	void Add_Render_Info_DropInfo(_float fTimeDelta);
	void Add_Render_Info_BuffInfo(_float fTimeDelta);

	void Switch_Weapon_UI_Action(_float fTimeDelta);
	void Switch_Bag_UI_Action(_float fTimeDelta, _bool bIsOpen);
	void Using_Fable_UI_Action(_float fTimeDelta);

private:
	// ���� ����
	_bool m_bIsBagOpen = false;
	_float m_fBag_Open_Waiting_Now = 0.f;
	_float m_fBag_Open_Waiting_Limit = 0.5f;

	// ���ͷ���
	_bool m_bCan_InterAction = true;

	// ���
	list<DROP_ITEM_INFO*> m_DropItem_Index_list;
	_int m_iDrop_Item_Render_Limit = 3;
	_float m_fEmerge_Effect_Time = 1.f;
	_float m_fDrop_Item_ShowTime = 3.f;

	// ���� ���� 
	_int m_iWeapon_Equip_0_Symbol = 0;
	_Vec2 m_vSwitch_Time = { 0.f,0.5f };
	_float m_fNormal_Weapon_Fx_Alpha_Origin = 0.f;

	_Vec2 m_vFable_Art_ActionTime = { 0.f,0.5f };

	// ���� ü�¹� 
	_bool m_bIs_BossHp_Activate = false;
	_wstring m_strBossName = {};
	_float m_fBoss_Hp_Now = 0.f;
	_float m_fBoss_Hp_Max = 0.f;


private: // �׽�Ʈ ����
	_bool m_bWeapon_Top = true;

public:
	static CUIPage_Play* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END