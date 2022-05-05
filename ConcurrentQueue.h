#ifndef _CONCURRENTQUEUE_H_
# define _CONCURRENTQUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

/*
* Thread safe unbounded concurrent queue
* It's a FIFO: items are popped out in the order they've been pushed in.
* The queue length is limited by available memory only.
* For a bounded queue, use ConcurrentBlockingQueue.
*/
template <typename T>
class ConcurrentQueue
{
public:

	T pop()
	{
        std::unique_lock<std::mutex> mlock(mutex);
        while (queue.empty())
		{
            queueEmpty.wait(mlock);	// block
		}
        auto item = queue.front();
        queue.pop();
		return item;
	}

	void pop(T& item)
	{
        std::unique_lock<std::mutex> mlock(mutex);
        while (queue.empty())
		{
            queueEmpty.wait(mlock);	// block
		}
        item = queue.front();
        queue.pop();
	}

	void push(const T& item)
	{
		{
            std::unique_lock<std::mutex> mlock(mutex);
            queue.push(item);
		}
        queueEmpty.notify_one();
	}

	void push(T&& item)
	{
		{
            std::unique_lock<std::mutex> mlock(mutex);
            queue.push(std::move(item));
		}
        queueEmpty.notify_one();
	}

	inline size_t size() 
	{
        std::unique_lock<std::mutex> mlock(mutex);
        return queue.size();
	}

	inline bool empty() 
	{
        std::unique_lock<std::mutex> mlock(mutex);
        return queue.empty();
	}

private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable queueEmpty;	// blocks when the queue is empty
};


#endif /* !_CONCURRENTQUEUE_H_ */

