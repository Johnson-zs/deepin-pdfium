// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/test/scoped_locale.h"

#include <locale.h>

#include "testing/gtest/include/gtest/gtest.h"

namespace pdfium {
namespace base {

ScopedLocale::ScopedLocale(const std::string& locale) {
  prev_locale_ = setlocale(LC_ALL, nullptr);
  EXPECT_TRUE(setlocale(LC_ALL, locale.c_str()) != nullptr)
      << "Failed to set locale: " << locale;
}

ScopedLocale::~ScopedLocale() {
  EXPECT_STREQ(prev_locale_.c_str(), setlocale(LC_ALL, prev_locale_.c_str()));
}

}  // namespace base
}  // namespace pdfium
