//
// Created by yretenai on 2020-08-15.
//

#ifndef POHWARAN_SOUNDBANKINFO_H
#define POHWARAN_SOUNDBANKINFO_H

#include "export.h"
#include <cstdint>
#include <map>
#include <string>

namespace pohwaran {
    POHWARAN_EXPORT class SoundbankInfo {
      public:
        explicit SoundbankInfo(std::string path);
        ~SoundbankInfo() = default;
        std::map<uint32_t, std::string> IdMap;
    };
} // namespace pohwaran

#endif // POHWARAN_SOUNDBANKINFO_H
