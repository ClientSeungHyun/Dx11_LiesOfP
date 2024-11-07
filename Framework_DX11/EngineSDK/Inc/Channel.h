#pragma once

#include "Base.h"

BEGIN(Engine)

/* �ִԿ��ó��� ������ ���� ����ϴ� �� �� �ϳ� */
/* �ð��� ���� ���� ������ ������ �ִ´�. �ʱ� ������ ��� �ð��� ���� ���°����� �̸� �����صд�. */
/* ���� ��Ÿ�Ӱ��� �� ä�ο��� ���� �ִϸ��̼��� ��� ��ġ�� �������ָ� �����ġ�� �´� �������(m_TransformatoinMatrix )�� �����. */

/* KEYFRAME : Ư�� �����ġ���� �� ���� ���ؾ��� ����(���)�� ����(������)�Ͽ� ǥ���� ����ü.*/

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	vector<KEYFRAME>&	Get_KeyFrames() { return m_KeyFrames; }
	void				Set_KeyFrames(vector<KEYFRAME>& KeyFrames) { m_KeyFrames = KeyFrames; }

	_uint				Get_BoneIndex() { return m_iBoneIndex; }

public:
	HRESULT Initialize(HANDLE* pFile, const class CModel* pModel);
	void Update_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex, _double CurrentTrackPosition);

	KEYFRAME		Find_KeyFrameIndex(_uint* pCurrentKeyFrameIndex, _double CurrentTrackPosition);
	KEYFRAME		Get_KeyFrame(_uint iFrame);

	HRESULT						Create_BinaryFile(HANDLE* pFile);

private:
	_char				m_szName[MAX_PATH] = {};

	_uint				m_iBoneIndex = { 0 };
	_uint				m_iNumKeyFrames = {};
	vector<KEYFRAME>	m_KeyFrames;

public:
	static CChannel* Create(HANDLE* pFile, const class CModel* pModel);
	virtual void Free() override;
};

END