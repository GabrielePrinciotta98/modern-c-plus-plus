#pragma once

#include <array>

template <typename T, typename U, typename V, typename W> 
struct Quadruple {
public:
    Quadruple(T t, U u, V v, W w) : first{t}, second{u}, third{v}, fourth{w} {};

    T get_first(){ return first; }
    U get_second(){ return second; }
    V get_third(){ return third; }
    W get_fourth(){ return fourth; }

private:
    T first;
    U second;
    V third;
    W fourth;
};


template <> 
struct Quadruple <int, int, int, int>{
public:
    std::array<int, 4> members;

    Quadruple(int t, int u, int v, int w){
        members[0] = t;
        members[1] = u;
        members[2] = v;
        members[3] = w;
    }

    int get_first(){ return members[0]; }
    int get_second(){ return members[1]; }
    int get_third(){ return members[2]; }
    int get_fourth(){ return members[3]; }

private:
    
};

template <typename T> 
struct Quadruple <T, T, T, T>{
public:
    std::array<T, 4> members;

    Quadruple(T t, T u, T v, T w){
        members[0] = t;
        members[1] = u;
        members[2] = v;
        members[3] = w;
    }

    T get_first(){ return members[0]; }
    T get_second(){ return members[1]; }
    T get_third(){ return members[2]; }
    T get_fourth(){ return members[3]; }

private:
    
};