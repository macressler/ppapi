// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_url_loader.h"

#include "ppapi/c/ppb_url_loader.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/tests/test_instance.h"

REGISTER_TEST_CASE(URLLoader);

bool TestURLLoader::Init() {
  url_loader_interface_ = reinterpret_cast<PPB_URLLoader const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_URLLOADER_INTERFACE));
  return !!url_loader_interface_;
}

void TestURLLoader::RunTest() {
  // TODO(darin): Add tests!
}
