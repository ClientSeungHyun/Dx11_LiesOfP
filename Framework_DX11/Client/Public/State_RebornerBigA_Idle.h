#pragma once
#include "State.h"

BEGIN(Engine)
class CFsm;
END

BEGIN(Client)

class CState_RebornerBigA_Idle : public CState
{
private:
    typedef enum {
        AN_IDLE = 51, AN_TURN_LEFT = 63, AN_TURN_RIGHT = 64
        , AN_RUN = 61, AN_WALK = 65
    }ANIMNUM;

public:
    CState_RebornerBigA_Idle(class CFsm* pFsm, class CMonster* pMonster);
    virtual ~CState_RebornerBigA_Idle() = default;

public:
    virtual HRESULT Initialize(_uint iStateNum, void* pArg) override;
    virtual HRESULT Start_State(void* pArg = nullptr) override;
    virtual void    Update(_float fTimeDelta) override;
    virtual void    End_State() override;

private:
    class CMonster* m_pMonster = { nullptr };

    _bool*              m_pFirstMeetCheck = { nullptr };
    _bool*              m_pDiscover = { nullptr };

    _float              m_fRecognitionDist = { 15.f };
    _float              m_fDiscoverDist = { 4.f };

    _float              m_fIdleTime = { 0.f };
    _float              m_fIdleEndDuration = { 0.6f };

    _bool               m_bRunning = { false };
    _bool               m_bWalk = { false };

    _float              m_fRunSpeed = { 3.5f };
    _float              m_fWalkSpeed = { 1.f };
    _float              m_fRunningWeights = { 3.f };

    _float              m_fNeedDist_ForAttack = { 3.5f };


    _uint               m_iAtkCnt{};

private:
    void                Calc_Act_Attack(_float fTimeDelta, _float fDist);

public:
    static CState_RebornerBigA_Idle* Create(class CFsm* pFsm, class CMonster* pMonster, _uint iStateNum, void* pArg = nullptr);
    virtual void Free() override;

};

END