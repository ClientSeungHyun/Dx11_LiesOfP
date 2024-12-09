#pragma once

#include "Client_Defines.h"
#include "Pawn.h"
#include "PlayerCamera.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
END

BEGIN(Client)

class CPlayer final : public CPawn
{
public:
	enum WEAPON_TYPE { WEP_RAPIER, WEP_SCISSOR, WEP_FLAME, WEP_END };

	enum EFFECT_TYPE 
	{
		EFFECT_RAPIER_TRAIL_FIRST, EFFECT_RAPIER_TRAIL_SECOND,
		EFFECT_GRIND, EFFECT_HEAL,

		EFFECT_END
	};

	enum PLAYER_STATE
	{
		HIT, PARRY, HEAL, CHANGEWEP, GRINDER, GETUP,

		OH_IDLE, OH_WALK, OH_RUN, OH_SPRINT, OH_GUARD, OH_GUARDHIT, OH_JUMP, OH_DASH,

		TH_IDLE, TH_WALK, TH_RUN, TH_SPRINT, TH_GUARD, TH_GUARDHIT, TH_JUMP, TH_DASH,

		RAPIER_LATTACK0, RAPIER_LATTACK1, RAPIER_RATTACK0, RAPIER_CHARGE, RAPIER_FABALE, RAPIER_PARRYATTACK, RAPIER_FATAL,

		FLAME_LATTACK0, FLAME_LATTACK1, FLAME_RATTACK0, FLAME_RATTACK1, FLAME_CHARGE0, FLAME_CHARGE1, FLAME_FABLE, FLAME_PARRYATTACK, FLAME_FATAL,

		SCISSOR_LATTACK0, SCISSOR_LATTACK1, SCISSOR_RATTACK0, SCISSOR_RATTACK1, SCISSOR_CHARGE0, SCISSOR_CHARGE1, SCISSOR_FATAL,
		SCISSOR_FABAL0, SCISSOR_FABAL1, SCISSOR_FABAL2, SCISSOR_BUFF,


		LADDER = 100, LIFT, CHEST, ITEMGET, STARGAZER, TELEPORT,

		CUTSCENE_IDLE = 200,
		SOPHIA_DOOR_OPEN, SOPHIA_WALK, SOPHIA_HAND, SOPHIA_HANDEND,
		RAXASIA_DOOR_OPEN,

		STATE_END
	};

	// 24-11-27 �輺��
	// �÷��̾� ���� ����ü 
#pragma region PLAYER_STAT
	typedef struct PLAYER_STAT_INFO
	{
		void Reset_Zero()
		{
			iPlayer_Level = 0;

			iPoint_HP = 0;
			iPoint_Stamina = 0;
			iPoint_Attack = 0;
			iPoint_Defence = 0;
			iPoint_Heal = 0;

			iStat_Attack = 0;
			iStat_Defence = 0;

			vGauge_Hp = { 0.f,0.f,0.f,0.f };
			vGauge_Stamina = { 0.f,0.f,0.f,0.f };
			vGauge_Region = { 0.f,0.f,0.f,0.f };

			fRegion_Interval = 0.f;

			iErgo = 0;
			iErgo_LevelUp = 0;
			iPoint_Per_Level = 0;

			fDebuff_Fire = { 0.f,0.f };
			fDebuff_Electric = { 0.f,0.f };
			fDebuff_Acid = { 0.f,0.f };
		};

		// ���� ���� 
		_int iPlayer_Level = 1;

		// ���� ����Ʈ (���� �� �� ��� ����Ʈ)
		_int iPoint_HP = 1;
		_int iPoint_Stamina = 1;
		_int iPoint_Attack = 1;
		_int iPoint_Defence = 1;
		_int iPoint_Heal = 1;

		// �ɷ�ġ 
		_float iStat_Attack = 0.f;
		_float iStat_Defence = 0.f;


		// ������ ��ġ 
		// x : ���� ��ġ
		// y : ���� ��ġ(HP�� ���帮���� �� ǥ��)
		// z : ���� �ִ�ġ (���� ��ġ�� �þ �� �ִ� �ִ�ġ)
		// w : �ִ�ġ �Ѱ� (�������� �ִ�ġ�� �ø� �� �þ �� �ִ� �Ѱ�ġ) 
		_Vec4 vGauge_Hp = { 500.f,500.f,500.f,1000.f };
		_Vec4 vGauge_Stamina = { 500.f,500.f,500.f,1000.f };
		_Vec4 vGauge_Region = { 600.f,600.f,600.f,1000.f };

		// ���� 1ĭ�� ��ġ 
		// UI ���� 1ĭ�� ǥ�õǴ� �� 
		// ���� �������� _int(���� �ִ�ġ) / _int(1ĭ ��ġ)�� �� ��ŭ�� ��Ÿ�� -> ���� �ִ�ġ 700, 1ĭ ��ġ 200�̸� 3ĭ�� ��� 
		_float fRegion_Interval = 200.f;
		
		// ������ 
		_int iErgo = 1000;

		// ���� ���� �ʿ��� ������ 
		_int iErgo_LevelUp = 100;

		// ���� 1 ��� ��, ��� ������ ���� ����Ʈ 
		_int iPoint_Per_Level = 5;

		// �����̻� 
		// x : ���� ��ġ 
		// y : �ִ�ġ 
		// ���� ��ġ�� 0���� ũ�� �������� ��Ÿ���� 
		_Vec2 fDebuff_Fire = { 0.f, 100.f };
		_Vec2 fDebuff_Electric = { 0.f, 100.f };
		_Vec2 fDebuff_Acid = { 0.f, 100.f };
	}STAT_INFO;
#pragma endregion

#pragma region PLAYER_ABILITY
	typedef struct PLAYER_ABILITY_INFO
	{
		// �ƹķ� 
		_bool bDebuff_Fire_Ignore = false; // ��մ� ������ �ƹķ� : ȭ�� �����̻� �鿪 
		_bool bDebuff_Electric_Ignore = false; // �ұ��� �ƹķ� : ���� �����̻� �鿪 
		_bool bDebuff_Acid_Ignore = false; // ���׶��� �ƹķ� : �꼺 �����̻� �鿪 

		_float fIncrease_Stamina = 0.f; // ������ �ƹķ� : �ִ� ���¹̳� ���� 
		_float fIncrease_Hp = 0.f;  // ������ �ƹķ� : �ִ� ü�� ���� 
		_float fHeal = 0.f; // �������� �ƹķ� : ���������� HP ���� 

		// �ƹķ� || �������(������)
		//_float fIncrease_Defence = 0.f; // ö���� �ƹķ� || ������ ���� : ���� ���� ���� 

		// ��� ���� (������ ����)
		_float fResist_Fire = 0.f;
		_float fResist_Electric = 0.f;
		_float fResist_Acid = 0.f;
	}ABILITY_INFO;
#pragma endregion


public:
	CPlayerCamera*			Get_Camera() { return m_pPlayerCamera; }
	void					Set_Camera(class CPlayerCamera* pCamera) { m_pPlayerCamera = pCamera; }

	_bool					Get_IsJump() { return m_isJump; }
	void					Set_IsJump(_bool isJump) { m_isJump = isJump; }

	_bool					Get_IsGuard() { return m_isGuard; }
	void					Set_IsGuard(_bool isGuard, _bool isReset = true) {
		m_isGuard = isGuard;
		if (m_isGuard && isReset)
			m_fGuardTime = 0.f;
	}

	_bool					Get_IsParry() { return m_isParry; }
	void					Set_IsParry(_bool isParry) { m_isParry = isParry; }

	_bool					Get_IsLockOn() { return m_isLockOn; }
	void					Set_IsLockOn(_bool isLockOn) { m_isLockOn = isLockOn; }

	_bool					Get_IsInvicible() { return m_isInvicible; }
	void					Set_IsInvicible(_bool isInvicible) { m_isInvicible = isInvicible; }

	_bool					Get_IsLadderEnd() { return m_isLadderEnd; }

	_bool					Get_IsTeleport() { return m_isTeleport; }
	void					Set_IsTeleport(_bool isTeleport) { m_isTeleport = isTeleport; }

	WEAPON_TYPE				Get_WeaponType() { return m_eWeaponType; }
	void					Set_WeaponType(WEAPON_TYPE eType) { m_eWeaponType = eType; }
	class CWeapon*			Get_CurrentWeapon() { return m_pWeapon[m_eWeaponType]; }

	void					Reset_Root() { m_vCurRootMove = m_vRootMoveStack = _vector{0,0,0,0}; }

	CPawn*					Get_TargetMonster() { return m_pTargetMonster; }

	void					Set_DissloveRatio(_float fRatio) { m_fDissloveRatio = fRatio; }

	// 24-11-27 �輺��
	// �÷��̾� ���� ����ü ���� �Լ� 
	// �ܺο����� ���۷����� ���� 
	// ��ġ ������ �ݵ�� �÷��̾���� ���� 
#pragma region PLAYER_STAT
	const STAT_INFO&		Get_Player_Stat() const { return *m_tPlayer_Stat; }
	// 24-12-05 �輺��
	// UI �Ŵ��� ������ �Լ�
	STAT_INFO*				Get_Player_Stat_Adjust() { return m_tPlayer_Stat_Adjust; }
	ABILITY_INFO*			Get_Player_Ability() { return m_tPlayer_Ability; }

#pragma endregion

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

public:
	virtual void OnCollisionEnter(CGameObject* pOther) override;
	virtual void OnCollisionStay(CGameObject* pOther) override;
	virtual void OnCollisionExit(CGameObject* pOther) override;

public:
	void			Move_Dir(_Vec4 vDir, _float fTimeDelta, _bool isTurn = true);
	_Vec4			Calculate_Direction_Straight();
	_Vec4			Calculate_Direction_Right();

	void			Change_Weapon();
	_uint			Change_WeaponType();
	void			Appear_Weapon();
	void			Disappear_Weapon();
	void			Set_WeaponStrength(ATTACK_STRENGTH eStrength);
	
	void			Active_CurrentWeaponCollider(_float fDamageRatio = 1.f, _uint iHandIndex = 1);
	void			DeActive_CurretnWeaponCollider(_uint iHandIndex = 1);

	void			Seperate_Scissor();
	void			Combine_Scissor();

	void			Change_CameraMode(CPlayerCamera::CAMERA_MODE eMode);

	void			LockOnOff();
	CPawn*			Find_TargetMonster();

	void			Play_CurrentWeaponSound(const _uint iType, const TCHAR* pSoundKey, _uint iHandIndex = 1);

	void			Active_Effect(const EFFECT_TYPE& eType, _bool isLoop = true);
	void			DeActive_Effect(const EFFECT_TYPE& eType);

	virtual _bool	Calc_DamageGain(_float fAtkDmg, _Vec3 vHitPos = { 0.f,0.f,0.f }, _uint iHitType = HIT_END, _uint iAttackStrength = ATK_END, CGameObject* pAttacker = nullptr) override;
	_bool			Decrease_Stamina(_float fAmount);
	_bool			Check_Region_Fable01();
	_bool			Check_Region_Fable02();
	void			Decrease_Region(_uint iRegionCount = 1);
	void			Recovery_Region(_float fAmount = 10.f);

	/* Effect */
private:
	vector<class CEffect_Container*> m_Effects;
	class CEffect_Manager* m_pEffect_Manager = { nullptr };

	CTexture* m_pDissloveTexture = { nullptr };

private:
	CPlayerCamera*		m_pPlayerCamera = { nullptr };

	_bool				m_isJump = { false };
	_bool				m_isGuard = { false };
	_bool				m_isParry = { false };
	_bool				m_isLockOn = { false };
	_bool				m_isInvicible = { false };
	_bool				m_isRecoveryStamina = { false };
	_bool				m_isLadderEnd = { false };
	_bool				m_isTeleport = { false };
	_bool				m_isCollisionMonster = { false };

	_float				m_fGuardTime = {};
	_float				m_fRecoveryStaminaTime = { 0.f };
	_float				m_fDissloveRatio = {};

	CPawn*				m_pTargetMonster = { nullptr };

	class CWeapon*		m_pWeapon[WEP_END] = { nullptr, };
	WEAPON_TYPE			m_eWeaponType = { WEP_RAPIER };

private:
	_vector		m_vRootMoveStack = {};
	_Vec3		m_vCurRootMove = {};
	_double		m_PrevTrackPos = {};
	_bool		m_bEndAnim = { false };
	_bool		m_bResetRootMove = { true };

	// 24-11-27 �輺��
	// �÷��̾� ���� ����ü ���� 
#pragma region PLAYER_STAT
	STAT_INFO*			m_tPlayer_Stat = { nullptr };
	STAT_INFO*			m_tPlayer_Stat_Adjust = { nullptr };	// 
	ABILITY_INFO*		m_tPlayer_Ability = { nullptr };
#pragma endregion

private:
	_float				m_fStaminaRecoveryTime = {};

private:
	void			Damaged(_float fAtkDmg, _Vec3 vHitPos);

	void			Update_Stat(_float fTimeDelta);
	void			CollisionStay_IntercObj(CGameObject* pGameObject);

	void			Choice_GuardSound(_uint iAttackStrength = ATK_WEAK, _uint iHitType = HIT_END, _bool isPerfect = false);

private:
	HRESULT Ready_Components();
	HRESULT Ready_Weapon();
	HRESULT Ready_FSM();
	HRESULT Ready_Effect();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CPawn* Clone(void* pArg);
	virtual void Free() override;

};


END