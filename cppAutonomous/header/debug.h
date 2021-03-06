#ifndef INLINE_DEBUG_H
#define INLINE_DEBUG_H

#include <iostream>

inline void debug_print(char const* msg) {
#ifdef _PRINT_IN_DEBUG_
  std::cout << msg << ".\n";
#endif
}

template <typename T>
inline void debug_print(const char* msg, T err) {
#ifdef _PRINT_IN_DEBUG_
  std::cout << msg << " " << err << ".\n";
#endif
}

#endif