#pragma once
#include "Base.h"


/* ������Ʈ�� ������ �������� �����Ѵ�. */
/* �����Ͽ� �����Ѵ�.*/

BEGIN(Engine)

class Instance_Manager final : public CBase
{
private:
	Instance_Manager();
	virtual ~Instance_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_NonAnimModel(const _wstring& strPrototypeTag, class CModel* pModel);
	void Clear();

private:
	map<const _wstring, class CModel*>			m_pNonAnimModles;
private:
	class CModel* Find_NonAnimModel(const _wstring& strPrototypeTag);

public:
	static Instance_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END