// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BASE_MEMORY_ALIGNED_MEMORY_H_
#define THIRD_PARTY_BASE_MEMORY_ALIGNED_MEMORY_H_

#include <stddef.h>
#include <stdint.h>

#include <type_traits>

#include "build/build_config.h"
#include "base/base_export.h"
#include "base/bits.h"

#if defined(COMPILER_MSVC)
#include <malloc.h>
#else
#include <stdlib.h>
#endif

// A runtime sized aligned allocation can be created:
//
//   float* my_array = static_cast<float*>(AlignedAlloc(size, alignment));
//
//   // ... later, to release the memory:
//   AlignedFree(my_array);
//
// Or using unique_ptr:
//
//   std::unique_ptr<float, AlignedFreeDeleter> my_array(
//       static_cast<float*>(AlignedAlloc(size, alignment)));

namespace pdfium {
namespace base {

// This can be replaced with std::aligned_alloc when we have C++17.
// Caveat: std::aligned_alloc requires the size parameter be an integral
// multiple of alignment.
BASE_EXPORT void* AlignedAlloc(size_t size, size_t alignment);

inline void AlignedFree(void* ptr) {
#if defined(COMPILER_MSVC)
  _aligned_free(ptr);
#else
  free(ptr);
#endif
}

// Deleter for use with unique_ptr. E.g., use as
//   std::unique_ptr<Foo, base::AlignedFreeDeleter> foo;
struct AlignedFreeDeleter {
  inline void operator()(void* ptr) const {
    AlignedFree(ptr);
  }
};

#ifdef __has_builtin
#define SUPPORTS_BUILTIN_IS_ALIGNED (__has_builtin(__builtin_is_aligned))
#else
#define SUPPORTS_BUILTIN_IS_ALIGNED 0
#endif

inline bool IsAligned(uintptr_t val, size_t alignment) {
  // If the compiler supports builtin alignment checks prefer them.
#if SUPPORTS_BUILTIN_IS_ALIGNED
  return __builtin_is_aligned(val, alignment);
#else
  DCHECK(bits::IsPowerOfTwo(alignment));
  return (val & (alignment - 1)) == 0;
#endif
}

#undef SUPPORTS_BUILTIN_IS_ALIGNED

inline bool IsAligned(void* val, size_t alignment) {
  return IsAligned(reinterpret_cast<uintptr_t>(val), alignment);
}

}  // namespace base
}  // namespace pdfium

#endif  // THIRD_PARTY_BASE_MEMORY_ALIGNED_MEMORY_H_
