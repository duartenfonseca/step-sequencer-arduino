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

// Print binary representation of a uint16
void printUint16(const uint16_t &aUint16);

// prints the sequence of an instrument as uint16
void printSequence(const uint16_t &instrumentSequence);

// prints the sequence of an instrument as bool array
void printSequence(bool instrumentSequence[]);

template <typename Func>
// helper function to process each bit in a uint16_t
void  processBits(const uint16_t &aUint16, Func func) {
  for (int8_t aBit = 15; aBit >= 0; aBit--) {
    func(bitRead(aUint16, aBit), 15 - aBit); // Pass the bit value and position
  }
}
}

#endif