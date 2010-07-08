// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_case.h"

#include <sstream>

std::string TestCase::MakeFailureMessage(const char* file,
                                         int line,
                                         const char* cmd) {
  std::ostringstream output;
  output << "Failure in " << file << "(" << line << "): " << cmd;
  return output.str();
}
