//
// Created by yretenai on 2020-08-08.
//

#ifndef POHWARAN_EXPORT
#define POHWARAN_EXPORT

#ifdef _WIN32
#include "export_win32.h"
#else
#include "export_clang.h"
#endif

#endif //POHWARAN_EXPORT
