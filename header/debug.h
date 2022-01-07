#ifndef INLINE_DEBUG_H
#define INLINE_DEBUG_H

#include <iostream>

// TODO:: use target_compile_definitions() to generate MACRO or generator expression
#define _PRINT_IN_DEBUG_

inline
void debug_print(char const* msg) {
    #ifdef _PRINT_IN_DEBUG_
    std::cout << msg << "\n";
    #endif
}

inline
void debug_print(const char* msg, int err) {
    #ifdef _PRINT_IN_DEBUG_
    std::cout << msg << " " << err << "\n";
    #endif
}

#endif