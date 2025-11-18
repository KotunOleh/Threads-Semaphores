#include <iostream>
#include <vector>
#include <thread>
#include <latch>
#include <syncstream>
#include <chrono>
#include <format>

using namespace std;

latch latch_a(2); 
latch latch_b(2); 
latch latch_c(1); 
latch latch_d(2); 
latch latch_e(2);
latch latch_f(2);
latch latch_g(2); 
latch latch_h(2); 

void f(char id, int k) {
    this_thread::sleep_for(chrono::milliseconds(100)); 
    
    osyncstream(cout) << "From set " << id << " done action " << k << ".\n";
}


void thr1() {
    for(int i=1; i<=5; ++i) f('b', i);
    latch_b.count_down();
    latch_c.wait();

    for(int i=1; i<=5; ++i) f('g', i);
    latch_g.count_down();

    latch_a.wait();
    latch_e.wait();
    latch_g.wait();

    for(int i=1; i<=2; ++i) f('i', i);
}

void thr2() {
    for(int i=1; i<=5; ++i) f('c', i);
    latch_c.count_down();

    for(int i=6; i<=8; ++i) f('g', i);
    latch_g.count_down();

    for(int i=1; i<=2; ++i) f('h', i);

    latch_h.count_down(); 

    latch_a.wait();
    latch_e.wait();
    latch_g.wait();

    for(int i=3; i<=4; ++i) f('i', i);
}

void thr3() {
    for(int i=1; i<=5; ++i) f('a', i);
    latch_a.count_down();

    latch_b.wait();

    for(int i=1; i<=5; ++i) f('f', i);
    latch_f.count_down();

    latch_a.wait();
    latch_e.wait();
    latch_g.wait();

    for(int i=5; i<=6; ++i) f('i', i);
}

void thr4() {
    for(int i=1; i<=5; ++i) f('d', i);
    latch_d.count_down();

    latch_b.wait();

    for(int i=6; i<=7; ++i) f('f', i);
    latch_f.count_down();

    for(int i=1; i<=3; ++i) f('e', i);
    latch_e.count_down();

    latch_d.wait();
    latch_f.wait();
    latch_h.wait();

    for(int i=1; i<=3; ++i) f('j', i);
}

void thr5() {
    f('b', 6); 
    latch_b.count_down();
    f('a', 6); 
    latch_a.count_down();
    f('d', 6); 
    latch_d.count_down();
    
    latch_b.wait();
    f('e', 4); 
    latch_e.count_down();

    latch_c.wait();

    for(int i=3; i<=6; ++i) f('h', i);
    latch_h.count_down();

    latch_d.wait();
    latch_f.wait();
    latch_h.wait();

    for(int i=4; i<=6; ++i) f('j', i);
}

int main() {
    cout << "Started claculations." << endl;
    jthread t1(thr1);
    jthread t2(thr2);
    jthread t3(thr3);
    jthread t4(thr4);
    jthread t5(thr5);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    cout << "Finished calculations" << endl;
    return 0;
}