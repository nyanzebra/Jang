#pragma once

#ifndef FTL_TYPES
#define FTL_TYPES

constexpr unsigned long long int operator"" _bytes(unsigned long long int size) { return size; }
constexpr unsigned long long int operator"" _kilobytes(unsigned long long int size) { return size * 1024_bytes; }
constexpr unsigned long long int operator"" _megabytes(unsigned long long int size) { return size * 1024_kilobytes; }
constexpr unsigned long long int operator"" _gigabytes(unsigned long long int size) { return size * 1024_megabytes; }
constexpr unsigned long long int operator"" _terabytes(unsigned long long int size) { return size * 1024_gigabytes; }
constexpr unsigned long long int operator"" _petabytes(unsigned long long int size) { return size * 1024_terabytes; }

#ifndef __RPCNDR_H_VERSION__
typedef unsigned char byte;
#endif

typedef void* void_pointer;
typedef const void* const_void_pointer;
typedef std::size_t size_type;

#endif
