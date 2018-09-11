/* 
 * File:   main.cpp
 * Author: Eagleye
 *
 * Created on September 11, 2018, 2:02 AM
 */

#include <iostream>
#include <thread>
#include <future>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>

using namespace std;

int factorial(int n) {
    int result = 1;

    for (int i = n; i > 1; i--) {
        result *= i;
    }
    return result;
}

deque<packaged_task<int() >> task_q;
mutex mu;
condition_variable cond;

void thread_1() {
    packaged_task<int() > t;
    {
        unique_lock<mutex> locker(mu);
        cond.wait(locker, []() {return !task_q.empty();}); // if conditional variable is used then lock_guard cannot be used
        t = move(task_q.front());
        task_q.pop_front();
    }
    t();
}

int main(int argc, char** argv) {

    thread thr1(thread_1);

    packaged_task<int(int) > t(factorial); /* packaged_task cannot get arguments like thread
                                            * in order to pass the argument below code is used
                                            */
    t(5); // since packade_task returns void we write the code line below to get the result
    int result = t.get_future().get();

    packaged_task<int() > t1(bind(factorial, 6)); /* with std::bind, we can pass the argument
                     |                             * Here, since we already passed the argument,
                     |_____________________________* the indicated brackets will be with no argument
                                                   */
    //t1(); // t1 does get any parameters here since the parameter has already been passed by bind class

    future<int> fu = t1.get_future();
    {
        lock_guard<mutex> locker(mu);
        task_q.push_back(move(t1)); // t1 task was pushed to deque task_q
    }
    cond.notify_one();
    cout << fu.get() << '\n';

    // cout << t1.get_future().get() << '\n';



    // cout<<factorial(5)<<'\n';

    cout << result << '\n';

    return 0;
}
/* 3 ways to get the future:
 *  1- pormise::get_future()
 *  2- packaged_task::get_future()
 *  3- async() returns  a future
 */
