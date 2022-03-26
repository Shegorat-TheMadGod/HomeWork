// Deque.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "List.h"

int main()
{
	List<int> myList;
	size_t listSize;
	int outputData;
	std::cout << "The list is empty for now. You are able to put data in the list. " << std::endl;
	std::cout << "Possible comands are push_back, push_front, pop_back, pop_front, delbyval, delallbyval, show, size, exit." << std::endl;
	std::cout << "The program run shall be break when you enter exit comand." << std::endl;
	std::string choice = "";
	while (true) {
		std::cin >> choice;
		if (choice == "push_back") {
			int inputData;
			std::cout << "Enter data. ";
			while (true) {
				std::cin >> inputData;
				if (!std::cin.fail()) {
					break;
				}
				else {
					std::cout << "You entered wrong data!" << std::endl;
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
			}
			myList.AddNodeToEnd(inputData);
		}
		else if (choice == "push_front") {
			int inputData;
			std::cout << "Enter data. ";
			while (true) {
				std::cin >> inputData;
				if (!std::cin.fail()) {
					break;
				}
				else {
					std::cout << "You entered wrong data!" << std::endl;
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
				}
			}
			myList.AddNodeToStart(inputData);
		}
		else if (choice == "pop_back") {
			outputData = myList.DeleteNodeFromEnd();
		}
		else if (choice == "pop_front") {
			outputData = myList.DeleteNodeFromStart();
		}
		else if (choice == "delbyval") {
			outputData = myList.DeleteByValue();
		}
		else if (choice == "delallbyval") {
			outputData = myList.DeleteAllByValue();
		}
		else if (choice == "show") {
			myList.ShowList();
		}
		else if (choice == "size") {
			listSize = myList.Size();
		}
		else if (choice == "exit") {
			std::cout << "The program operation is interrupted. Wish you luck!";
			break;
		}
		else {
			std::cout << "You entered wrong comand, please try again!" << std::endl;
		}
	}
	return 0;
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
