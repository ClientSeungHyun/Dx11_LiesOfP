#pragma once
#include "State.h"

BEGIN(Engine)
class CFsm;
END

BEGIN(Client)

class CState_Raxasia_CutScene_Phase2 : public CState
{
public:
    CState_Raxasia_CutScene_Phase2(class CFsm* pFsm, class CMonster* pMonster);
    virtual ~CState_Raxasia_CutScene_Phase2() = default;

public:
    virtual HRESULT Initialize(_uint iStateNum, void* pArg) override;
    virtual HRESULT Start_State(void* pArg = nullptr) override;
    virtual void    Update(_float fTimeDelta) override;
    virtual void    End_State() override;

private:
    class CMonster* m_pMonster = { nullptr };

    _uint               m_iAnimation_Phase2 = {};

    _bool               m_isStartCutScene = {};
    _bool               m_isChangePhase2Model = {};
    _bool               m_isOnGroundWeapon = {};
    _bool               m_isChangeWeaponPos = {};
    _bool               m_isReConnetWeaponBone = {};
    _bool               m_isPlayWeaponChangeAnim = {};
    _bool               m_isConneectLeftHandShield = {};
    _bool               m_isChangeRealWeapon = {};
    _bool               m_isCrashGroundWeaon = {};

    _int                m_iFrme_ArmorGround[9] = {};
    string            m_strArmorBoneName[9] = {};
    _bool               m_isGroundArmor[9] = { false };
    _float              m_fArmorOffset[9] = {};

    _float              m_fDelay = {};
    _float              m_fAnimationStopTimer = {};
    _float              m_fAnimationStopTimer2 = {};

    _Vec3               m_vRootMoveStack{};

    class CRaxasia_Helmet_CutScene* m_pCutSceneHelmet = { nullptr };
    class CRaxasia_Sword_CutScene*  m_pCutSceneWeapon = { nullptr };
    class CWeapon*                  m_pShieldWeapon = { nullptr };

private:
    void                End_Check();
    void                Stop_Play_Animation(_int iFrame, _float fTimeDelta);

private:
    _bool               m_isShowDialog[2] = {false, };

    void                Control_Dialog(_int iFrame);

private:
    void                Control_Effect(_int iFrame);

public:
    static CState_Raxasia_CutScene_Phase2* Create(class CFsm* pFsm, class CMonster* pMonster, _uint iStateNum, void* pArg = nullptr);
    virtual void Free() override;

};

END