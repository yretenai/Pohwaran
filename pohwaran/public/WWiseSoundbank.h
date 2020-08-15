//
// Created by yretenai on 2020-08-15.
//

#ifndef LIBPOHWARAN_WWISESOUNDBANK_H
#define LIBPOHWARAN_WWISESOUNDBANK_H

#include "export.h"
#include <cstdint>
#include <map>
#include <memory>
#include <standard_dragon/Array.h>

namespace pohwaran {
    POHWARAN_EXPORT class WWiseSoundbank {
      public:
        explicit WWiseSoundbank(dragon::Array<char> buffer);
        ~WWiseSoundbank() = default;
        std::map<uint32_t, std::shared_ptr<dragon::Array<char>>> Streams;
    };
} // namespace pohwaran

#endif // LIBPOHWARAN_WWISESOUNDBANK_H
