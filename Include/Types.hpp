#pragma once

#include <stdint.h>

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

struct ASGlobalFunction {
    u64 Namespace;
    u64 FuncPtr;
private:
    char pad_0010[0x18];
public:
    int FuncPtrFlag;
    u64 Declaration;
    u64 NextGlobalFunction;
};
static_assert(sizeof(ASGlobalFunction) == 0x40);

struct ASObjectMethod {
    u64 Namespace;
    u64 ObjectName;
    u64 FuncPtr;
private:
    char pad_0018[0x18]; // Unknown data
public:
    int FuncPtrFlag;
    u64 Declaration;
    int CallingConvention;
    u64 NextObjectMethod;
};
static_assert(sizeof(ASObjectMethod) == 0x50);

struct ASGlobalProperty {
    u64 Namespace;
    u64 Declaration;
    u64 ValuePtr;
    u64 NextGlobalProperty;
};
static_assert(sizeof(ASGlobalProperty) == 0x20);
