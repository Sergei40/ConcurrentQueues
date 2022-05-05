
#ifndef _CONCURRENT_BLOCKING_QUEUE_H_
# define _CONCURRENT_BLOCKING_QUEUE_H_
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream> 
#include <cstring> 

/*
 * Потокобезопасная блокирующая очередь с фиксированной емкостью.
 * Операции push блокируются, когда очередь заполнена.
*/
template <typename T>
class ConcurrentBlockingQueue
{
public:
    ConcurrentBlockingQueue(const size_t capacity = 10) : maxSize(capacity) {}

	T pop()
	{
        std::unique_lock<std::mutex> mlock(mutex);
        while (queue.empty())
		{
            std::stringstream message;
            message << "Can't pop : queue is empty !\n";
            std::cout << message.str();
            queueEmpty.wait(mlock);
		}
        auto item = queue.front();
        queue.pop();
		mlock.unlock();
        queueFull.notify_one();
		return item;
	}

	void pop(T& item)
	{
		{
            std::unique_lock<std::mutex> mlock(mutex);
            while (queue.empty())
			{
                std::stringstream message;
                message << "Can't pop : queue is empty !\n";
                std::cout << message.str();
                queueEmpty.wait(mlock);
			}
            item = queue.front();
            queue.pop();
		}
        queueFull.notify_one();
	}

	void push(const T& item)
	{
		{
            std::unique_lock<std::mutex> mlock(mutex);
            while (queue.size() >= maxSize) {
                std::stringstream message;
                message << "Can't push : queue is full !\n";
                std::cout << message.str();
                queueFull.wait(mlock);
			}
            queue.push(item);
		}
        queueEmpty.notify_one();
	}

	void push(T&& item)
	{
		{
            std::unique_lock<std::mutex> mlock(mutex);
            while (queue.size() >= maxSize) {
                std::stringstream message;
                message << "Can't push : queue is full !\n";
                std::cout << message.str();
                queueFull.wait(mlock);
			}
            queue.push(std::move(item));
		}
        queueEmpty.notify_one();
	}

	inline size_t size() noexcept
	{
        std::unique_lock<std::mutex> mlock(mutex);
        return queue.size();
	}

	inline bool empty() noexcept
	{
        std::unique_lock<std::mutex> mlock(mutex);
        return queue.empty();
	}

	inline bool full() noexcept
	{
        std::unique_lock<std::mutex> mlock(mutex);
        return queue.size() >= maxSize;
	}

private:
    std::queue<T> queue;
    const size_t maxSize;
    std::mutex mutex;
    std::condition_variable queueFull;  // блокируется, когда очередь заполнена
    std::condition_variable queueEmpty; // блокируется, когда очередь пуста
};


#endif /* !_CONCURRENT_BLOCKING_QUEUE_H_ */

