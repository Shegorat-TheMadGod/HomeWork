// ProducerConsumer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

class Stock {
private:
	std::vector<std::vector<int>> m_stock;
	const unsigned int m_vectorsCount = 5; //count of vectors in the stock
	const unsigned int m_elementsCount = 100; //count of elements in each vector
	const unsigned int m_batchesCount = 15; //count of produced m_elementCount until m_exit is true
	bool m_exit=false;
	bool m_stockIsFull = false;
	std::mutex m_mutex;
public:
	friend class Producer;
	friend class Consumer;
};
class Producer {
public:
	void Produce(Stock& stock) {
		std::vector<int> batch;
		unsigned int counter = 0;
		while (!stock.m_exit) {
			if (stock.m_stock.size()<stock.m_vectorsCount) {
				if (batch.size() < stock.m_elementsCount) {
					batch.push_back(rand());
				}
				else {
					stock.m_stock.push_back(batch);
					batch.clear();
					counter++;
				}
			}
			else {
				if (!stock.m_stockIsFull) {
					std::lock_guard<std::mutex> lock(stock.m_mutex);
					std::cout << "The stock is full"<<std::endl;
					stock.m_stockIsFull = true;
				}
			}
			if (counter >= stock.m_batchesCount) {
				stock.m_exit = true;
				break;
			}
		}
	}
};
class Consumer {
public:
	void Consume(Stock& stock) {
		std::vector<int> batch;
		while (!stock.m_exit) {
			while (stock.m_stock.size()>0){
				if (stock.m_stock.size() > 0) {
					batch = stock.m_stock.back();
					stock.m_stock.pop_back();
					stock.m_stockIsFull = false;
					for (size_t i = 0; i < stock.m_elementsCount;i++) {
						std::lock_guard<std::mutex> lock(stock.m_mutex);
						std::cout << batch[i]<<std::endl;
					}
				}
			}
		}
	}
};
int main()
{
	Stock stock;
	Producer producer;
	Consumer consumer;
	std::thread producerThread(&Producer::Produce, &producer, std::ref(stock));
	std::thread consumerThread(&Consumer::Consume, &consumer, std::ref(stock));
	producerThread.join();
	consumerThread.join();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
