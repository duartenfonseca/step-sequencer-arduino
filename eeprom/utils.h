/*
  utils.h - useful multipurpose functions

*/
#ifndef Utils_h
#define Utils_h
#include "Arduino.h"

// templated sizeof function for arrays
template<size_t N>
struct type_of_size {
  typedef char type[N];
};

template<typename T, size_t Size>
typename type_of_size<Size>::type& sizeof_array_helper(T (&)[Size]);
#define sizeof_array(pArray) sizeof(sizeof_array_helper(pArray))

namespace utils {

// print each bit of a byte
void printBin(const byte &aByte);

// prints the sequence of an instrument
void printSequence(uint16_t instrumentSequence);
}

#endif