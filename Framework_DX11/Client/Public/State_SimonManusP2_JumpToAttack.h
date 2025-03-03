#pragma once
#include "State.h"

BEGIN(Engine)
class CFsm;
END

BEGIN(Client)

class CState_SimonManusP2_JumpToAttack : public CState
{
private:
    typedef enum {
        AN_JUMPTO_BACK = 33, AN_JUMPTO_LEFT = 35, AN_JUMPTO_RIGHT = 37
        , AN_MAGICTO_LEFT = 36, AN_MAGICTO_RIGHT = 38
        , AN_CANCLEATTACK = 34, AN_SWING = 32, AN_WAVE = 45
    }ANIMNUM;

public:
    CState_SimonManusP2_JumpToAttack(class CFsm* pFsm, class CMonster* pMonster);
    virtual ~CState_SimonManusP2_JumpToAttack() = default;

public:
    virtual HRESULT Initialize(_uint iStateNum, void* pArg) override;
    virtual HRESULT Start_State(void* pArg = nullptr) override;
    virtual void    Update(_float fTimeDelta) override;
    virtual void    End_State() override;

private:
    class CMonster* m_pMonster = { nullptr };

    _uint               m_iCurStartAnim = {};
    _uint               m_iCurLastAnim = {};

    _bool               m_bSwing = { false };

    _bool               m_bJump = { false };
    _bool               m_bMagic = { false };
    _bool               m_bMagicSound = { false };

    _uint               m_iRouteTrack = {};

private:
    _bool               End_Check();
    void                Collider_Check(_double CurTrackPos);
    void                Effect_Check(_double CurTrackPos);
    void                Control_Sound(_double CurTrackPos);

public:
    static CState_SimonManusP2_JumpToAttack* Create(class CFsm* pFsm, class CMonster* pMonster, _uint iStateNum, void* pArg = nullptr);
    virtual void Free() override;

};

END