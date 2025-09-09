#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;

#define NUM_THREADS 10

// shared data
int shared_x = 0;

// mutex for synchronization
mutex mtx;   //changed name

void thr_func(int tid) {
    // lock mutex before accessing shared_x
    lock_guard<mutex> guard(mtx);

    this_thread::sleep_for(chrono::seconds(1));
    cout << "hello from thr_func, thread id: " << tid << endl;

    shared_x++;
    cout << "x = " << shared_x << endl;

    // lock_guard automatically unlocks when function exits
}

int main() {
    vector<thread> threads;

    // create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(thr_func, i);
    }

    // join all threads
    for (auto &t : threads) {
        t.join();
    }

    cout << "Final value of x = " << shared_x << endl;
    return 0;
}
