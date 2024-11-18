#include "..\Public\Channel.h"
#include "Bone.h"
#include "Model.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(HANDLE* pFile, const CModel* pModel)
{
	_ulong dwByte = 0;

	ReadFile(*pFile, &m_szName, MAX_PATH, &dwByte, nullptr);

	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);

	if (m_iBoneIndex == -1)
	{
		int a = 0;
	}

	/* ���� ū Ű �������� ã�� */
	/* Assimp������ Ű �������� ���� �����Ӱ� ū ���̰� ������ ���ο� Ű �������� �������� ���� (�߰��� ��� �ִ� ���� ����) */
	/* ���� ���� ū Ű ������ �������� �����ؾ� ��. */
	ReadFile(*pFile, &m_iNumKeyFrames, sizeof(_uint), &dwByte, nullptr);

	_float3			vScale = {};
	_float4			vRotation = {};
	_float3			vTranslation = {};

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME			KeyFrame{};

		ReadFile(*pFile, &KeyFrame, sizeof(KEYFRAME), &dwByte, nullptr);
		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex, _double CurrentTrackPosition)
{
	if (0.0 == CurrentTrackPosition)
		*pCurrentKeyFrameIndex  = 0;

	KEYFRAME	LastKeyFrame = m_KeyFrames.back();

	_vector		vScale, vRotation, vTranslation;

	/* ������ �ʿ���� ��� */
	if (CurrentTrackPosition >= LastKeyFrame.TrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}

	/* ������ �ʿ��� ��κ� ��� */
	else
	{
		if (CurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex].TrackPosition)
		{
			while (CurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].TrackPosition)
				++(*pCurrentKeyFrameIndex);
		}
		else//�ڷ� ����Ǿ��� ��쿡�� Ű�������� ����
		{
			while (CurrentTrackPosition <= m_KeyFrames[*pCurrentKeyFrameIndex - 1].TrackPosition)
				--(*pCurrentKeyFrameIndex);
		}


		_vector		vSourScale = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale);
		_vector		vDestScale = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale);

		_vector		vSourRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation);
		_vector		vDestRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation);

		_vector		vSourTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation), 1.f);
		_vector		vDestTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation), 1.f);

		/* �װ� ���� �����Ѱ�. */
		_double		Ratio = (CurrentTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].TrackPosition) / (m_KeyFrames[*pCurrentKeyFrameIndex + 1].TrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].TrackPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, (_float)Ratio);
	}

	/* �� ������ ������� = ȸ�������� �翬�� �θ����.(�ڽĻ����� ���¸� ���鶧 ������������ �����ؾ��Ѵ�.) */

	_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

KEYFRAME CChannel::Find_KeyFrameIndex(_uint* pCurrentKeyFrameIndex, _double CurrentTrackPosition)
{
	if (0.0 == CurrentTrackPosition)
	{
		*pCurrentKeyFrameIndex = 0;
	}

	KEYFRAME	LastKeyFrame = m_KeyFrames.back();

	_vector		vScale, vRotation, vTranslation;

	/* ������ �ʿ���� ��� */
	if (CurrentTrackPosition >= LastKeyFrame.TrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
		return m_KeyFrames.back();
	}

	/* ������ �ʿ��� ��κ� ��� */
	else
	{
		while (CurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].TrackPosition)
			++(*pCurrentKeyFrameIndex);
	}

	return m_KeyFrames[*pCurrentKeyFrameIndex];
}

KEYFRAME CChannel::Get_KeyFrame(_uint iFrame)
{
	if (m_KeyFrames.size() > iFrame)
	{
		return m_KeyFrames[iFrame];
	}
	else
	{
		return m_KeyFrames[0];
	}
}

HRESULT CChannel::Create_BinaryFile(HANDLE* pFile)
{
	_ulong dwByte = 0;
	//�̸� ����
	WriteFile(*pFile, &m_szName, MAX_PATH, &dwByte, nullptr);

	//Ű������ ���� ����
	WriteFile(*pFile, &m_iNumKeyFrames, sizeof(_uint), &dwByte, nullptr);

	for (auto& KeyFrame : m_KeyFrames)
	{
		WriteFile(*pFile, &KeyFrame, sizeof(KEYFRAME), &dwByte, nullptr);
	}

	return S_OK;
}

CChannel* CChannel::Create(HANDLE* pFile, const CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pFile, pModel)))
	{
		MSG_BOX(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
}
