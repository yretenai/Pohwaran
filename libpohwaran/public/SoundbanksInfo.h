//
// Created by yretenai on 2020-08-16.
//

#ifndef LIBPOHWARAN_SOUNDBANKSINFO_H
#define LIBPOHWARAN_SOUNDBANKSINFO_H

#include "PohwaranExport.h"
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace pohwaran {
    class POHWARAN_EXPORT SoundbanksInfo {
      public:
        typedef struct STREAMED_SOUNDBANK_FILE {
            uint32_t id;
            std::string language;
            std::filesystem::path short_name;
            std::filesystem::path path;
        } StreamedSoundbankFile;
        typedef struct SOUNDBANK_INFO {
            StreamedSoundbankFile soundbank_file;
            std::vector<uint32_t> files;
            std::map<uint32_t, StreamedSoundbankFile> included_memory_files;
            // std::map<uint32_t, StreamedSoundbankFile> excluded_memory_files;
        } SoundbankInfo;

        std::map<uint32_t, StreamedSoundbankFile> streamed_files;
        std::vector<SoundbankInfo> soundbanks;
        std::string platform;
        uint32_t schema_version;
        uint32_t version;

        SoundbanksInfo(std::filesystem::path soundbanks_path);
        ~SoundbanksInfo() = default;
    };
} // namespace pohwaran

#endif // LIBPOHWARAN_SOUNDBANKSINFO_H
