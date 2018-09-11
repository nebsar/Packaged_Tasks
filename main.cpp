/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Eagleye
 *
 * Created on September 11, 2018, 2:02 AM
 */
#include <iostream>
#include <future>
#include <functional>

using namespace std;

int factorial(const int& n) {
    int result = 1;

    for (int i = n; i > 1; i--) {
        result *= i;
    }
    return result;
}

int main(int argc, char** argv) {

    packaged_task<int(int) > t(factorial); /* packaged_task cannot get arguments like thread
                                            * in order to pass the argument below code is used
                                            */
    packaged_task<int() > t1(bind(factorial, 6)); /* with std::bind, we can pass the argument
                     |                             * Here, since we already passed the argument,
                     |_____________________________* the indicated brackets will be with no argument
                                                   */
    t1(); // t1 does get any parameters here since the parameter has already been passed by bind class
    
    cout<<t1.get_future().get()<<'\n';

    t(5); // since packade_task returns void we write the code line below to get the result

    int result = t.get_future().get();

    // cout<<factorial(5)<<'\n';

    cout << result << '\n';

    return 0;
}

