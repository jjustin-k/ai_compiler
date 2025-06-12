#include<iostream>
#include <sstream>

#ifndef DEBUG_HPP
#define DEBUG_HPP

extern int debug_level;

#endif

void print(std::ostringstream stream, int level);