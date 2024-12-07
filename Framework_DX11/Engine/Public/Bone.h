#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _char* Get_Name() const { return m_szName; }

	_Matrix				Get_CombinedTransformationMatrix() const { return m_CombinedTransformationMatrix; }
	void				Set_CombinedTransformationMatrix(_matrix CombinedMatrix) { m_CombinedTransformationMatrix, CombinedMatrix; }

	const _Matrix*		Get_CombinedTransformationMatrix_Ptr() const { return &m_CombinedTransformationMatrix; }

	_Matrix				Get_TransformationMatrix() { return XMLoadFloat4x4(&m_TransformationMatrix); }
	void				Set_TransformationMatrix(_Matrix TransformationMatrix) { m_TransformationMatrix = TransformationMatrix; }

	_int				Get_ParentBoneIndex() { return m_iParentBoneIndex; }

	_bool				Get_IsChildOf_Boundary() { return m_bIsChildOf_Boundary; }

public:
	virtual HRESULT		Initialize(HANDLE* pFile, _int iParentBoneIndex);
	HRESULT				Initialize_ToBinary(HANDLE* pFile, _bool bUseBoundary, _uint eType);
	void				Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

	void				Setting_ParentBoneName(class CModel* pModel);

	void				Update_Boundary(const vector<CBone*>& Bones, _int iCurBoneIndex, _int iBoundaryBoneIndex);

	HRESULT				Create_BinaryFile(HANDLE* pFile, _bool bUseBoundary, _uint eType);

	_bool				Get_isNeedTuning() { return m_isNeedTuning; }
	void				SetUp_isNeedTuning(_bool bState);

	void				Apply_Tuning() { m_TransformationMatrix = m_TuningMatrix; }

private:
	/* ���� �̸� */
	_char		m_szName[MAX_PATH] = {};
	_char		m_szParentName[MAX_PATH] = {};

	/* �θ� �������� ǥ���� �� ������ �������. */
	_Matrix		m_TransformationMatrix = {};
	_Matrix		m_PrevTransformationMatrix = {};

	/* �� ���� �������� ���� ��� ( �� ���� ��� * �θ��� ����������� ) */
	/* m_TransformationMatrix * Parent's m_CombinedTransformationMatrix */
	_Matrix		m_CombinedTransformationMatrix = {};


	//Ʃ�� ��Ʈ����
	_Matrix		m_TuningMatrix = {};

	_int		m_iParentBoneIndex = { -1 };

	//�ִϸ��̼� ����ü �и��� ���� �غ�
	_bool		m_bIsChildOf_Boundary = {false};
	_bool		m_isNeedTuning = { false };

public:
	static CBone* Create(HANDLE* pFile, _int iParentBoneIndex);
	static CBone* Create_To_Binary(HANDLE* pFile, _bool bUseBoundary, _uint eType);
	CBone* Clone();
	virtual void Free() override;
};

END