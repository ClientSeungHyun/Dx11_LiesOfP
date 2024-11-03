#include "stdafx.h"
#include "ThreadPool.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CThreadPool)

CThreadPool::CThreadPool()
    :m_pGameInstance{ CGameInstance::Get_Instance() }
{
    int a = 0;
}

HRESULT CThreadPool::Initialize()
{
    m_iNumThreads = 5;

    return S_OK;
}

void CThreadPool::Start_Work()
{
    for (size_t i = 0; i < m_iNumThreads; ++i)
    {
        m_WorkerThreads.emplace_back([this]() { this->WorkerThread(); });
    }
}

void CThreadPool::StopAll()
{
    m_isAllStop = true;
    m_ContionVariable.notify_all();

    for (auto& work : m_WorkerThreads) {
        work.join();
    }
  
    m_WorkerThreads.clear();
}

void CThreadPool::WorkerThread()
{
    while (true) {
        unique_lock<mutex> lock(m_Mutex);
        m_ContionVariable.wait(lock, [this]() { return !this->m_Functions.empty() || m_isAllStop; });
        if (m_isAllStop && this->m_Functions.empty())
        {
            return;
        }

        // �� ���� job �� ����.
        function<void()> Function = move(m_Functions.front());
        m_Functions.pop();
        lock.unlock();

        // �ش� job �� �����Ѵ� :)
        Function();
    }
}

void CThreadPool::Free()
{
    __super::Free();

    StopAll();

    lock_guard<mutex> lock(m_Mutex);
    while (!m_Functions.empty()) {
        m_Functions.pop();  // �Լ��� ���������� ����
    }

    Safe_Release(m_pGameInstance);
}
