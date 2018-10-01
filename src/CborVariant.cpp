#include "CborVariant.h"

#ifdef FLOAT_T_32
#include <IEEE754tools.h>
#endif

CborVariant::CborVariant(CborBuffer& buffer, cn_cbor* raw) : buffer(buffer) {
  this->raw = raw;
}

CborVariant::CborVariant(CborBuffer& buffer, const char* value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_string_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, CBOR_INT_T value) : buffer(buffer) {
  cn_cbor_errback err;

  raw = cn_cbor_int_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, CBOR_FLOAT_T value) : buffer(buffer) {
  cn_cbor_errback err;

  // There is not a double create in cn_cbor...
  // raw = cn_cbor_idouble_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, CBOR_BOOLEAN_T value) : buffer(buffer) {
  cn_cbor_errback err;

  // There is not a boolean create in cn_cbor...
  // raw = cn_cbor_boolean_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, CBOR_NULL_T value) : buffer(buffer) {
  cn_cbor_errback err;

  // There is not a null create in cn_cbor...
  // raw = cn_cbor_null_create(value, &buffer.context, &err);
}

CborVariant::CborVariant(CborBuffer& buffer, CborObject& value) : buffer(buffer) {
  this->raw = value.raw;
}

CborVariant::CborVariant(CborBuffer& buffer, CborArray& value) : buffer(buffer) {
  this->raw = value.raw;
}

int CborVariant::length() {
  if (raw == 0) {
    return -1;
  }

  return raw->length;
}

bool CborVariant::isValid() {
  return raw != 0;
}

bool CborVariant::isString() {
  return isValid() && raw->type == CN_CBOR_BYTES || raw->type == CN_CBOR_TEXT;
}

bool CborVariant::isInteger() {
  return isValid() && raw->type == CN_CBOR_UINT || raw->type == CN_CBOR_INT;
}

bool CborVariant::isFloat() {
  return isValid() && raw->type == CN_CBOR_DOUBLE;
}

bool CborVariant::isBoolean() {
  return isValid() && ( raw->type == CN_CBOR_TRUE || raw->type == CN_CBOR_FALSE );
}

bool CborVariant::isNull() {
  return isValid() && raw->type == CN_CBOR_NULL;
}

bool CborVariant::isObject() {
  return isValid() && raw->type == CN_CBOR_MAP;
}

bool CborVariant::isArray() {
  return isValid() && raw->type == CN_CBOR_ARRAY;
}

const char* CborVariant::asString() {
  if (!isValid()) {
    return 0;
  }

  if (raw->type != CN_CBOR_BYTES && raw->type != CN_CBOR_TEXT) {
    return 0;
  }

  if (raw->v.str[raw->length] != 0) {
    char* tmp = (char*)buffer.alloc(raw->length + 1);

    for (int i = 0; i < raw->length; i++) {
      tmp[i] = raw->v.str[i];
    }

    return tmp;
  }

  return raw->v.str;
}

CBOR_INT_T CborVariant::asInteger() {
  if (!isValid()) {
    return 0;
  }

  if (raw->type == CN_CBOR_UINT) {
    return raw->v.uint;
  }

  if (raw->type == CN_CBOR_INT) {
    return raw->v.sint;
  }

  return 0;
}

CBOR_FLOAT_T CborVariant::asFloat() {
  if (!isValid()) {
    return 0;
  }
  if (raw->type == CN_CBOR_DOUBLE ) {
    #ifdef FLOAT_T_32
    // Use the IEEE754 routine to round the 64 bit double down to 32 bits
    // returns NAN if the 64 bit float is too big !fixme handle the NAN case
    union _DBLCONV dbl;
    if ( 8 == raw->length ) {
      dbl.d = raw->v.dbl;
      raw->v.dbl = doublePacked2Float(dbl.b);
    }
    #endif
    return raw->v.dbl;
  }

  return 0;
}

CBOR_BOOLEAN_T CborVariant::asBoolean() {
  if (!isValid()) {
    return 0; // in C/C++ isValid() should be tested before using the boolean
  }

  if (raw->type == CN_CBOR_TRUE) {
    return 1;
  }

  if (raw->type == CN_CBOR_FALSE) {
    return 0;
  }

  return 0;
}


CborObject CborVariant::asObject() {
  if (isObject()) {
    return CborObject(buffer, raw);
  }

  return CborObject(buffer);
}

CborArray CborVariant::asArray() {
  if (isArray()) {
    return CborArray(buffer, raw);
  }

  return CborArray(buffer);
}

size_t CborVariant::encode(uint8_t* data, size_t size) {
  return cn_cbor_encoder_write(data, 0, size, raw);
}
