#include <functional>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <thread>
#include "ConcurrentQueue.h"
#include "ConcurrentBlockingQueue.h"

using MyQueue = ConcurrentBlockingQueue<int>;

unsigned int uniqueItemId = 0;

void produce(MyQueue& q, unsigned int producer, unsigned int iter) {
    for (unsigned int i = 0; i < iter; ++i) {
        q.push(++uniqueItemId);

        std::stringstream message; // оператор << - не потокобезопасный
        message << "Producer " << producer << " ==> item " << uniqueItemId << ", size = " << q.size() << std::endl;
        std::cout << message.str();
    }
}

void consume(MyQueue& q, unsigned int consumer, unsigned int iter) {
    for (unsigned int i = 0; i< iter; ++i) {
        auto item = q.pop();

        std::stringstream message;
        message << "Consumer " << consumer << " <== id " << item << ", size = " << q.size() << std::endl;
        std::cout << message.str();
    }
}

int main()
{
    MyQueue myQueue(5);
    const int iterProducer = 10;
    const int iterConsumer = 10;

    // producer threads
    std::thread producer1(std::bind(&produce, std::ref(myQueue), 1, iterProducer));
    std::thread producer2(std::bind(&produce, std::ref(myQueue), 2, iterProducer));
    std::thread producer3(std::bind(&produce, std::ref(myQueue), 3, iterProducer));

    // consumer threads
    std::thread consumer1(std::bind(&consume, std::ref(myQueue), 1, iterConsumer));
    std::thread consumer2(std::bind(&consume, std::ref(myQueue), 2, iterConsumer));
    std::thread consumer3(std::bind(&consume, std::ref(myQueue), 3, iterConsumer));

    producer1.join();
    producer2.join();
    producer3.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();

}


