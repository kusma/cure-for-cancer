#ifndef MISC_H
#define MISC_H

#include "linkedlist.h"

template<typename T>
inline void xchg( T* i1, T* i2 ){
    T i3 = *i1;
    *i1 = *i2;
    *i2 = i3;
}

class File;

void SetWindowsBackground();

#endif //MISC_H