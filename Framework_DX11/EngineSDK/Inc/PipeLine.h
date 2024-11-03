#pragma once

#include "Base.h"

/* ��, ������ĸ� �����Ѵ�. */
/* �� �����Ӹ��� Update�Լ��� ȣ���ϸ鼭 ����־��� ��,��������� �������  ī�޶��� ��ġ �̸� ���صд�. */

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrices[eState], TransformMatrix);
	}

public:
	const _Matrix& Get_Transform(D3DTRANSFORMSTATE eState) const {
		return m_TransformMatrices[eState];
	}
	const _Matrix& Get_Transform_Inverse(D3DTRANSFORMSTATE eState) const {
		return m_TransformInverseMatrices[eState];
	}

	const _Vec4& Get_CamPosition_Vec4() const {
		return m_vCamPosition; }
	const _Vec3& Get_CamPosition_Vec3() const {
		return _Vec3(m_vCamPosition);
	}


public:
	HRESULT Initialize();
	void Update();

private:
	_Matrix			m_TransformMatrices[D3DTS_END];
	_Matrix			m_TransformInverseMatrices[D3DTS_END];
	_Vec4			m_vCamPosition = {};

public:
	static CPipeLine*	Create();
	virtual void Free() override;
};

END