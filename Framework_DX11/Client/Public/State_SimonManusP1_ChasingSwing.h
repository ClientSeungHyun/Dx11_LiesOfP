#pragma once
#include "State.h"

BEGIN(Engine)
class CFsm;
END

BEGIN(Client)

class CState_SimonManusP1_ChasingSwing : public CState
{
private:
    typedef enum {
        AN_CHASINGSWING_START = 13, AN_CHASINGSWING_LOOP = 12, AN_CHASINGSWING_END = 11
    }ANIMNUM;

public:
    CState_SimonManusP1_ChasingSwing(class CFsm* pFsm, class CMonster* pMonster);
    virtual ~CState_SimonManusP1_ChasingSwing() = default;

public:
    virtual HRESULT Initialize(_uint iStateNum, void* pArg) override;
    virtual HRESULT Start_State(void* pArg = nullptr) override;
    virtual void    Update(_float fTimeDelta) override;
    virtual void    End_State() override;

private:
    class CMonster* m_pMonster = { nullptr };

    _float              m_fCurrentTime = { 0.f };
    _float              m_fChaseDuration = { 1.5f };

    _bool               m_bSwing = { false };

    _bool               m_bStampSound = { false };
    _bool               m_bSwingSound = { false };

    _uint               m_iRouteTrack = {};

private:
    _bool               End_Check();
    void                Collider_Check(_double CurTrackPos);
    void                Effect_Check(_double CurTrackPos);
    void                Control_Sound(_double CurTrackPos);

public:
    static CState_SimonManusP1_ChasingSwing* Create(class CFsm* pFsm, class CMonster* pMonster, _uint iStateNum, void* pArg = nullptr);
    virtual void Free() override;

};

END