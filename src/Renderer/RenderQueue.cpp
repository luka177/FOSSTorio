#include <Renderer/RenderQueue.h>
#include <atomic>
#include <mutex>

const std::vector<RenderObject>& RenderQueue::getFrontList() const
{
    int front = m_frontIndex.load(std::memory_order_acquire);
    return m_lists[front];
}

std::vector<RenderObject>& RenderQueue::getBackList()
{
    int front = m_frontIndex.load(std::memory_order_relaxed);
    int back = 1 - front;
    return m_lists[back];
}

void RenderQueue::swapBuffers()
{
    int currentFront = m_frontIndex.load(std::memory_order_relaxed);
    int newFront = 1 - currentFront;
    m_frontIndex.store(newFront, std::memory_order_release);
}