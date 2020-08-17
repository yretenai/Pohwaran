//
// Created by yretenai on 2020-08-04.
//

#include <Pohwaran.h>
#include <cstdio>
#include <standard_dragon/WemSoundbank.hpp>

#ifndef _WIN32
#define sprintf_s(b, s, f, ...) sprintf(b, f, __VA_ARGS__)
#endif

using namespace pohwaran;

int main(int argc, char** argv) {
    if (argc < 3) {
        eprintf("Usage: %s path_to_data output_path \n", argv[0]);
        return -1;
    }

    std::filesystem::path root = std::filesystem::path(argv[1]);
    std::filesystem::path info = root / "Sound" / "GeneratedSoundBanks" / "SoundbanksInfo.xml";
    std::filesystem::path out(argv[2]);

    if (!std::filesystem::exists(info)) {
#ifndef _WIN32
        eprintf("%s doesn't exist!", info.c_str());
#else
        eprintf("%ls doesn't exist!", info.c_str());
#endif
        return -2;
    }

    SoundbanksInfo bnk_info(info);
    std::vector<std::filesystem::path> paths;
    for (SoundbanksInfo::SoundbankInfo soundbank_info : bnk_info.soundbanks) {
        std::filesystem::path bnk_path = root / "Sound" / "GeneratedSoundBanks" / soundbank_info.soundbank_file.path;
        if (!std::filesystem::exists(bnk_path)) {
            ELOG("Cannot find bnk path " << bnk_path);
            continue;
        }
        for (std::pair<uint32_t, SoundbanksInfo::StreamedSoundbankFile> file : soundbank_info.included_memory_files) {
            std::filesystem::path parent = file.second.path.parent_path();
            if (std::find(paths.begin(), paths.end(), parent) == paths.end()) {
                paths.push_back(parent);
            }
        }
        char* id_buffer[15];
        for (uint32_t id : soundbank_info.files) {
            if (bnk_info.streamed_files.find(id) == bnk_info.streamed_files.end()) {
                continue;
            }
            SoundbanksInfo::StreamedSoundbankFile file = bnk_info.streamed_files[id];
            sprintf_s(reinterpret_cast<char* const>(id_buffer), 17, "%d.wem", id);
            std::filesystem::path wem_path = root / "Sound" / "GeneratedSoundBanks" / std::string(reinterpret_cast<const char* const>(id_buffer));
            if (!std::filesystem::exists(wem_path)) {
                wem_path = root / "Sound" / "GeneratedSoundBanks" / file.language / std::string(reinterpret_cast<const char* const>(id_buffer));
                if (!std::filesystem::exists(wem_path)) {
                    continue;
                }
            }
            std::filesystem::path parent = file.path.parent_path();
            if (std::find(paths.begin(), paths.end(), parent) == paths.end()) {
                paths.push_back(parent);
            }
        }
    }

    LOG("Building directory tree...");
    for (std::filesystem::path path : paths) {
        std::filesystem::path combined = out / path;
        if (std::filesystem::exists(combined)) {
            continue;
        }
        LOG("Creating directory " << combined);
        std::filesystem::create_directories(combined);
    }

    LOG("Processing soundbanks and copying wem files...");
    for (SoundbanksInfo::SoundbankInfo soundbank_info : bnk_info.soundbanks) {
        std::filesystem::path bnk_path = root / "Sound" / "GeneratedSoundBanks" / soundbank_info.soundbank_file.path;
        if (!std::filesystem::exists(bnk_path)) {
            continue;
        }
        LOG("Processing bnk and wem files for " << soundbank_info.soundbank_file.short_name << " with language "
                                                << soundbank_info.soundbank_file.language);
        dragon::WemSoundbank bnk = dragon::WemSoundbank(dragon::read_file(bnk_path));
        for (std::pair<uint32_t, SoundbanksInfo::StreamedSoundbankFile> file : soundbank_info.included_memory_files) {
            if (!bnk.has_stream(file.first)) {
                continue; // ?
            }
            if (std::filesystem::exists(out / file.second.path)) {
                continue;
            }
            LOG("Extracting " << (out / file.second.path));
            dragon::Array<char> data = bnk.get_stream(file.first);
            dragon::write_file(out / file.second.path, &data);
        }
        char* id_buffer[15];
        for (uint32_t id : soundbank_info.files) {
            if (bnk_info.streamed_files.find(id) == bnk_info.streamed_files.end()) {
                continue;
            }
            SoundbanksInfo::StreamedSoundbankFile file = bnk_info.streamed_files[id];
            sprintf_s(reinterpret_cast<char* const>(id_buffer), 17, "%d.wem", id);
            std::filesystem::path wem_path = root / "Sound" / "GeneratedSoundBanks" / std::string(reinterpret_cast<const char* const>(id_buffer));
            if (!std::filesystem::exists(wem_path)) {
                wem_path = root / "Sound" / "GeneratedSoundBanks" / file.language / std::string(reinterpret_cast<const char* const>(id_buffer));
                if (!std::filesystem::exists(wem_path)) {
                    continue;
                }
            }
            if (std::filesystem::exists(out / file.path)) {
                continue;
            }
            LOG("Copying " << (out / file.path));
            std::filesystem::copy_file(wem_path, out / file.path);
        }
    }
    LOG("Cleaning up");
}
