#pragma once

#include "VIBuffer.h"
#include "Model.h"

/* �� = �޽� + �޽� + �޽� .. */
/* �޽÷� �����ϴ� ���� : ������ ��ü�� �����ϰ� ������ֱ� ���ؼ�.*/
/* �޽� = �������� + �ε��� */

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;	

public:
	_char*			Get_Name() { return m_szName; }

	_uint			Get_MaterialIndex() const { return m_iMaterialIndex; }


	VTXMESH*		Get_Vertices() { return m_pVertices; }
	VTXANIMMESH*	Get_AnimVertices() { return m_pAnimVertices; }
	_uint*			Get_Indices() { return m_pIndices; }

	void Culling(CGameInstance* pGameInstance, _Matrix worldMatrix);

public:
	virtual HRESULT Initialize_Prototype(HANDLE* pFile, const CModel* pModel, _fmatrix PreTransformMatrix, const CModel::DISSOLVE_PARTICLE_DESC& ParticleDesc, _uint iMeshIndex);
	virtual HRESULT Initialize_Prototype_To_Binary(HANDLE* pFile, const CModel* pModel, _fmatrix PreTransformMatrix, const CModel::DISSOLVE_PARTICLE_DESC& ParticleDesc, _uint iMeshIndex);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_BoneMatrices(const CModel* pModel, class CShader * pShader, const _char * pConstantName);
	_matrix		CalcMatrix_forVtxAnim(vector<class CBone*>&	Bones	,VTXANIMMESH VtxStruct);
	HRESULT		Create_BinaryFile(HANDLE* pFile);
	_Vec3 Get_MinPos_Vertex() { return m_vMinPos; }
	_Vec3 Get_MaxPos_Vertex() { return m_vMaxPos; }
private:
	_char				m_szName[MAX_PATH] = {};
	_uint				m_iMaterialIndex = { 0 };
	_uint				m_iNumBones = { 0 };
	_uint				m_iNumFaces = {};

	/* uint : �𵨿� �ε��س��� ��ü �� ���� �ε����� �̾߱��Ѵ�. */
	vector<_int>		m_BoneIndices;
	vector<string>		m_BoneNaems;

	_float4x4			m_BoneMatrices[g_iMaxMeshBones] = {};
	vector<_float4x4>	m_OffsetMatrices;

	VTXMESH*			m_pVertices = { nullptr };
	VTXANIMMESH*		m_pAnimVertices = { nullptr };
	_uint*				m_pIndices = { nullptr };

	_int*				m_pWeightsCnts = { nullptr };

	//��� ����ȭ (�޽�����)
	_Vec3				m_vMinPos = { FLT_MAX ,FLT_MAX ,FLT_MAX };	//��ü�� �ּ� ��ǥ , �ִ��� ũ�� �ʱ�ȭ
	_Vec3				m_vMaxPos = { -FLT_MAX ,-FLT_MAX ,-FLT_MAX };	//��ü�� �ִ� ��ǥ, �ִ��� �۰� �ʱ�ȭ

	class COctree*		m_pOctree = { nullptr };
private:
	HRESULT	Ready_VertexBuffer_NonAnim(HANDLE* pFile, _fmatrix PreTransformMatrix);
	HRESULT	Ready_VertexBuffer_Anim(HANDLE* pFile, const CModel* pModel);

	HRESULT	Ready_VertexBuffer_To_Binary(HANDLE* pFile);
	
private:
	void CalculateBoundingBox_Mesh(const _Vec3& vVertexPos);
	_Vec3 Get_RandomFacePos(_Vec3 vFirst, _Vec3 vSecond, _Vec3 vThird);
	_Vec2 Get_CalculateUV(const _Vec3& A, const _Vec3& B, const _Vec3& C, const _Vec2& UV_A, const _Vec2& UV_B, const _Vec2& UV_C, const _Vec3& D);
	_float Get_TriangleArea(const _Vec3& A, const _Vec3& B, const _Vec3& C);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HANDLE* pFile, const CModel* pModel, _fmatrix PreTransformMatrix, const CModel::DISSOLVE_PARTICLE_DESC& ParticleDesc, _uint iMeshIndex);
	static CMesh* Create_To_Binary(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HANDLE* pFile, const CModel* pModel, _fmatrix PreTransformMatrix, const CModel::DISSOLVE_PARTICLE_DESC& ParticleDesc, _uint iMeshIndex);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END