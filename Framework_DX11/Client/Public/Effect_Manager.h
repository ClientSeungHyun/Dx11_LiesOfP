#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Particle_Effect.h"
#include "Texture_Effect.h"
#include "Mesh_Effect.h"
#include "Trail_Effect_OP.h"
#include "Trail_Effect_TP.h"
#include "Trail_Effect_MP.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class dafs;
END

BEGIN(Client)
class CEffect_Manager final : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)
public:
	enum EFFECT
	{
		EFFECT_POWER_HIT,
		EFFECT_END
	};

private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strEffectPath, const _wstring strTexturePath, const _wstring strModelPath);

public:
	class CEffect_Container* Clone_Effect(const _wstring& strECTag, const _Matrix* pParentMatrix, const _Matrix* pSocketMatrix, _Vec3 vPos = {}, _Vec3 vDir = { 0.f, 0.f, 1.f }, _Vec3 vScale = { 1.f, 1.f, 1.f }, _Vec3 vRotate = { 0.f, 0.f, 0.f });

	HRESULT Add_Effect_ToLayer(_uint iLevelID, const _wstring& strECTag, _Vec3 vPos = {}, _Vec3 vDir = {}, _Vec3 vScale = { 1.f, 1.f, 1.f });
	HRESULT Add_Effect_ToLayer(_uint iLevelID, const _wstring& strECTag, const _Matrix* pParentMatrix, const _Matrix* pSocketMatrix, _Vec3 vPos = {}, _Vec3 vDir = {}, _Vec3 vScale = { 1.f, 1.f, 1.f });

	HRESULT Add_Effect_ToLayer_Rot(_uint iLevelID, const _wstring& strECTag, _Vec3 vPos = {}, _Vec3 vRot = {}, _Vec3 vScale = { 1.f, 1.f, 1.f });

	CParticle_Effect* Clone_ParticleEffect(const _wstring& strEffectTag);
	CTexture_Effect* Clone_TextureEffect(const _wstring& strEffectTag);
	CMesh_Effect* Clone_MeshEffect(const _wstring& strEffectTag);
	CTrail_Effect_OP* Clone_TrailOP_Effect(const _wstring strEffectTag);
	CTrail_Effect_TP* Clone_TrailTP_Effect(const _wstring strEffectTag);
	CTrail_Effect_MP* Clone_TrailMP_Effect(const _wstring strEffectTag);

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	map<const _wstring, CParticle_Effect::PARTICLE_EFFECT_DESC> m_PEDescs;
	map<const _wstring, CTexture_Effect::TEXTURE_EFFECT_DESC> m_TEDescs;
	map<const _wstring, CMesh_Effect::MESH_EFFECT_DESC> m_MEDescs;
	map<const _wstring, CTrail_Effect_OP::TRAIL_OP_DESC> m_TOPDescs;
	map<const _wstring, CTrail_Effect_TP::TRAIL_TP_DESC> m_TTPDescs;
	map<const _wstring, CTrail_Effect_MP::TRAIL_MP_DESC> m_TMPDescs;

	map<const _wstring, vector<_wstring>> m_EffectContainers;

	map<const _wstring, vector<class CEffect_Container*>>	m_EffectPooling;

private:
	HRESULT Load_Effects(const _wstring& strEffectPath, const _wstring& strTexturePath, const _wstring& strModelPath);
	HRESULT Load_EffectContainers(const _wstring& strEffectPath);

	HRESULT Load_Textures(const _wstring strTexturePath);
	HRESULT Load_Models(const _wstring& strEffectPath);
	HRESULT Load_Shaders();
	HRESULT Load_Objects();

	HRESULT Load_Particle_Effect(const _wstring& strResultPath, const _wstring& strTexturePath);
	HRESULT Load_Texture_Effect(const _wstring& strResultPath, const _wstring& strTexturePath);
	HRESULT Load_Mesh_Effect(const _wstring& strResultPath, const _wstring& strTexturePath, const _wstring& strModelPath);
	HRESULT Load_TrailOP_Effect(const _wstring& strResultPath, const _wstring& strTexturePath);
	HRESULT Load_TrailTP_Effect(const _wstring& strResultPath, const _wstring& strTexturePath);
	HRESULT Load_TrailMP_Effect(const _wstring& strResultPath, const _wstring& strTexturePath);

	_wstring Get_FileName(const _wstring& strFileTag);
	_wstring Get_FileExtentin(const _wstring& strFileTag);
	_wstring Remove_FileExtentin(const _wstring& strFileTag);

	// 풀링할 오브젝트 만들어서 맵에 추가하고 반환하는 함수.
	class CEffect_Container* Find_PoolingEffect(const _wstring& strECTag, void* pArg);
	class CEffect_Container* Clone_Effect_From_Prototype(const _wstring& strECTag, void* pArg);

	HRESULT Effect_Pooling(const _wstring& strECTag, void* pArg, size_t iSize);

	_wstring Erase_Text(const _wstring& strText, const _wstring& strRemove);
	HRESULT Load_Texture(const _wstring& strTexturePath, const _wstring& strPrototypeTag);
	HRESULT Load_Moedl(const _wstring& strModelPath, const _wstring& strPrototypeTag);

public:
	virtual void Free() override;
};
END


