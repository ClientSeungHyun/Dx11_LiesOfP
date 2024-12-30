#pragma once
#include "Base.h"
#include "iostream"

class CMyProfilerCallback : public physx::PxProfilerCallback
{
public:
    // zoneStart: �������ϸ� ���� ���� �� ȣ��
    void* zoneStart(const char* eventName, bool detached, uint64_t contextId) override
    {
        // �̺�Ʈ �̸��� ���ؽ�Ʈ ID�� �α׿� ���
        std::cout << "Zone Started: " << eventName << " (ContextId: " << contextId << ")" << std::endl;
        // ���� �����͸� ������ �� �ִ� ����ü�� ���� ��ȯ (���⼭�� ���÷� NULL ��ȯ)

        return nullptr;
    }

    // zoneEnd: �������ϸ� ���� ���� �� ȣ��
    void zoneEnd(void* profilerData, const char* eventName, bool detached, uint64_t contextId) override
    {
        // �������ϸ� ������ ���Ḧ �α׿� ���
        std::cout << "Zone Ended: " << eventName << " (ContextId: " << contextId << ")" << std::endl;
        // profilerData�� zoneStart���� ��ȯ�� �����ͷ�, �̸� ���� ���� ������ ������ �� �ֽ��ϴ�.
    }
};