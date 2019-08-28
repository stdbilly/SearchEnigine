#pragma once
#include <iostream>
using std::cout;
using std::endl;

class Nocopyble {
protected:
    Nocopyble() {}
    ~Nocopyble() {}
    Nocopyble(const Nocopyble&) = delete ;
    Nocopyble& operator=(const Nocopyble&) = delete ;
};

