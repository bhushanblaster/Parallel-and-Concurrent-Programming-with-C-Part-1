#include<iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include<array>

char WEEKDAYS[7][10] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
int today = 0;
std::mutex marker;

void calendar_reader(const int id) {
	for (int i = 0; i<7; i++) {
		marker.lock();
		printf("Reader-%d sees today is %s\n", id, WEEKDAYS[today]);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		marker.unlock();
	}
}

void calendar_writer(const int id) {
	for (int i = 0; i<7; i++) {
		marker.lock();
		today = (today + 1) % 7;
		printf("Writer-%d updated date to %s\n", id, WEEKDAYS[today]);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		marker.unlock();
	}
}

int main() {
	// create ten reader threads ...but only two writer threads
	std::array<std::thread, 10> readers;
	for (unsigned int i = 0; i<readers.size(); i++) {
		readers[i] = std::thread(calendar_reader, i);
	}
	std::array<std::thread, 2> writers;
	for (unsigned int i = 0; i<writers.size(); i++) {
		writers[i] = std::thread(calendar_writer, i);
	}

	// wait for readers and writers to finish
	for (unsigned int i = 0; i<readers.size(); i++) {
		readers[i].join();
	}
	for (unsigned int i = 0; i<writers.size(); i++) {
		writers[i].join();
	}
}

/*
As this is normal lock Reader and writer aquires lock afer every 100 milliseconds.
When the lock is acquired by one thread, all other threads are waiting it to be released to acquire it.

Output

Reader-0 sees today is Sunday
Reader-0 sees today is Sunday
Reader-0 sees today is Sunday
Reader-0 sees today is Sunday
Reader-0 sees today is Sunday
Reader-0 sees today is Sunday
Reader-0 sees today is Sunday
Reader-7 sees today is Sunday
Reader-7 sees today is Sunday
Reader-7 sees today is Sunday
Reader-7 sees today is Sunday
Reader-7 sees today is Sunday
Reader-7 sees today is Sunday
Reader-7 sees today is Sunday
Reader-3 sees today is Sunday
Reader-3 sees today is Sunday
Reader-3 sees today is Sunday
Reader-3 sees today is Sunday
Reader-3 sees today is Sunday
Reader-3 sees today is Sunday
Reader-3 sees today is Sunday
Writer-1 updated date to Monday
Writer-1 updated date to Tuesday
Writer-1 updated date to Wednesday
Writer-1 updated date to Thursday
Writer-1 updated date to Friday
Writer-1 updated date to Saturday
Writer-1 updated date to Sunday
Reader-9 sees today is Sunday
Reader-9 sees today is Sunday
Reader-9 sees today is Sunday
Reader-9 sees today is Sunday
Reader-9 sees today is Sunday
Reader-9 sees today is Sunday
Reader-9 sees today is Sunday
Reader-8 sees today is Sunday
Reader-8 sees today is Sunday
Reader-8 sees today is Sunday
Reader-8 sees today is Sunday
Reader-8 sees today is Sunday
Reader-8 sees today is Sunday
Reader-8 sees today is Sunday
Writer-0 updated date to Monday
Writer-0 updated date to Tuesday
Writer-0 updated date to Wednesday
Writer-0 updated date to Thursday
Writer-0 updated date to Friday
Writer-0 updated date to Saturday
Writer-0 updated date to Sunday
Reader-2 sees today is Sunday
Reader-2 sees today is Sunday
Reader-2 sees today is Sunday
Reader-2 sees today is Sunday
Reader-2 sees today is Sunday
Reader-2 sees today is Sunday
Reader-2 sees today is Sunday
Reader-6 sees today is Sunday
Reader-6 sees today is Sunday
Reader-6 sees today is Sunday
Reader-6 sees today is Sunday
Reader-6 sees today is Sunday
Reader-6 sees today is Sunday
Reader-6 sees today is Sunday
Reader-1 sees today is Sunday
Reader-1 sees today is Sunday
Reader-1 sees today is Sunday
Reader-1 sees today is Sunday
Reader-1 sees today is Sunday
Reader-1 sees today is Sunday
Reader-1 sees today is Sunday
Reader-4 sees today is Sunday
Reader-4 sees today is Sunday
Reader-4 sees today is Sunday
Reader-4 sees today is Sunday
Reader-4 sees today is Sunday
Reader-4 sees today is Sunday
Reader-4 sees today is Sunday
Reader-5 sees today is Sunday
Reader-5 sees today is Sunday
Reader-5 sees today is Sunday
Reader-5 sees today is Sunday
Reader-5 sees today is Sunday
Reader-5 sees today is Sunday
Reader-5 sees today is Sunday
Press any key to continue . . .
*/
