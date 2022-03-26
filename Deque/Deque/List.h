#pragma once

template<typename T>
struct Node {
public:
	T data;
	Node<T>* next = nullptr;
	Node<T>* prev = nullptr;
};

template<typename T>
class List {
public:
	void AddNodeToEnd(T data) {
		Node<T>* myNode = new Node<T>;
		Node<T>* tmpNode = nullptr;
		myNode->data = data;
		if (m_head == nullptr) {
			m_head = myNode;
		}
		else {
			myNode->prev = m_last;
			tmpNode = m_last;
			tmpNode->next = myNode;
		}
		m_last = myNode;
		m_size++;
		//	std::cout << "You added a new element to list end." << std::endl;
	}
	void AddNodeToStart(T data) {
		Node<T>* myNode = new Node<T>;
		Node<T>* tmpNode = nullptr;
		myNode->data = data;
		if (m_head == nullptr) {
			m_head = myNode;
		}
		else {
			tmpNode = m_head;
			tmpNode->prev = myNode;
			myNode->next = m_head;
		}
		if (m_size < 1) {
			m_last = myNode;
		}
		m_head = myNode;
		m_size++;
		//	std::cout << "You added a new element to list start." << std::endl;
	}
	T DeleteNodeFromEnd() {
		Node<T>* tmpNode = m_last;
		if (m_size > 1) {
			--m_size;
			m_last = tmpNode->prev;
			tmpNode->prev->next = nullptr;
			T tmpData = tmpNode->data;
			delete tmpNode;
			//		std::cout << "The deleted element of the list contained following data: " << tmpData << std::endl;
			return tmpData;
		}
		else if (m_size == 1) {
			--m_size;
			T tmpData = tmpNode->data;
			m_head = nullptr;
			delete tmpNode;
			//		std::cout << "The deleted element of the list contained following data: " << tmpData << std::endl;
			return tmpData;
		}
		else {
			std::cout << "The list is empty!" << std::endl;
			return 0;
		}
	}
	T DeleteNodeFromStart() {
		Node<T>* tmpNode = m_head;
		if (m_size > 1) {
			--m_size;
			m_head = tmpNode->next;
			tmpNode->next->prev= nullptr;
			T tmpData = tmpNode->data;
			delete tmpNode;
			//		std::cout << "The deleted element of the list contained following data: " << tmpData << std::endl;
			return tmpData;
		}
		else if (m_size == 1) {
			--m_size;
			T tmpData = tmpNode->data;
			m_head = nullptr;
			delete tmpNode;
			//		std::cout << "The deleted element of the list contained following data: " << tmpData << std::endl;
			return tmpData;
		}
		else {
			std::cout << "The list is empty!" << std::endl;
			return 0;
		}
	}
	T DeleteByValue() {
		if (m_size != 0) {
			ShowList();
			std::cout << "Enter value to delete: ";
			T value;
			std::cin >> value;
			Node<T>* curNode = m_head;
			for (; curNode && curNode->data != value; curNode = curNode->next) {}
			if (curNode && curNode->data == value) {
				if (m_size == 1) {
					--m_size;
					m_head = nullptr;
					m_last = nullptr;
				}
				else if (curNode == m_last) {
					--m_size;
					curNode->prev->next = nullptr;
					m_last = curNode->prev;
				}
				else if (curNode == m_head) {
					--m_size;
					m_head = curNode->next;
					curNode->next->prev = nullptr;
				}
				else {
					--m_size;
					curNode->prev->next = curNode->next;
					curNode->next->prev = curNode->prev;
				}
				delete curNode;
				//			std::cout << "The deleted element of the list contained following data: " << value << std::endl;
				return value;
			}
			else {
				std::cout << "You entered wrong data!" << std::endl;
				return 0;
			}
		}
		else {
			std::cout << "The list is empty!" << std::endl;
			return 0;
		}
	}
	T DeleteAllByValue() {
		if (m_size != 0) {
			ShowList();
			std::cout << "Enter value to delete: ";
			T value;
			size_t tmpm_size = m_size;
			std::cin >> value;
			Node<T>* curNode = m_head;
			for (; curNode;) {
				Node<T>* tmpNode = curNode->next;
				if (curNode->data == value) {
					if (m_size == 1) {
						m_head = nullptr;
						m_last = nullptr;
					}
					else if (curNode == m_last) {
						curNode->prev->next = nullptr;
						m_last = curNode->prev;
					}
					else if (curNode == m_head) {
						m_head = curNode->next;
						curNode->next->prev = nullptr;
					}
					else {
						curNode->prev->next = curNode->next;
						curNode->next->prev = curNode->prev;
					}
					--m_size;
					delete curNode;
					curNode = tmpNode;
				}
				else {
					curNode = curNode->next;
				}
			}
			if (m_size != tmpm_size) {
				//			std::cout << "The deleted elements of the list contained following data: " << value << std::endl;
				return value;
			}
			else {
				std::cout << "You entered wrong value!" << std::endl;
				return 0;
			}
		}
		else {
			std::cout << "The list is empty!" << std::endl;
			return 0;
		}
	}
	void ShowList(){
	if (m_size > 0) {
		Node<T>* curNode = m_head;
		std::cout << "The list contain following data: ";
		for (curNode; curNode; curNode = curNode->next) {
			std::cout << curNode->data << ", ";
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "The list is empty!" << std::endl;
	}
}
	size_t Size() {
		std::cout << "The size of the list is " << m_size << std::endl;
		return m_size;
	}
	List(size_t size, T defaultValue) {
		m_size = size;
		for (size_t i = 0; i < size; i++) {
			AddNodeToEnd(defaultValue);
		}
	}
	List() {
		m_size = 0;
		Node<T>* m_head = nullptr;
		Node<T>* m_last = nullptr;
	}
	~List() {
		if (m_size > 0) {
			Node<T>* curNode = m_head;
			Node<T>* tmpNode = curNode;
			for (; curNode;) {
				tmpNode = curNode->next;
				delete curNode;
				curNode = tmpNode;
			}
		}
		m_size = 0;
		m_head = nullptr;
		m_last = nullptr;
	}

private:
	size_t m_size = 0;
	Node<T>* m_head = nullptr;
	Node<T>* m_last = nullptr;
};


