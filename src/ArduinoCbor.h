#ifndef ARDUINO_CBOR_H_
#define ARDUINO_CBOR_H_

#define USE_CBOR_CONTEXT

#include "cn-cbor/cn-cbor.h"

#ifndef CBOR_INT_T
#define CBOR_INT_T long
#endif

#ifndef CBOR_BOOLEAN_T
#define CBOR_BOOLEAN_T bool
#endif

// null type is only tested, not represented
#ifndef CBOR_NULL_T
#define CBOR_NULL_T int // value is not used, but in constructor for CborVariant
#endif

// define a float type with isFloat(), asFloat()
#ifndef CBOR_FLOAT_T
#define CBOR_FLOAT_T double // double on Arduino is 32 bits
#endif

// Limit float to 32 bits for Arduino
#ifndef FLOAT_T_32
#define FLOAT_T_32
#endif

class CborArray;
class CborBuffer;
class CborObject;
class CborVariant;

#include "CborArray.h"
#include "CborBuffer.h"
#include "CborObject.h"
#include "CborVariant.h"

#endif // ARDUINO_CBOR_H_
