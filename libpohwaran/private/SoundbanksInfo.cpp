//
// Created by yretenai on 2020-08-16.
//

#include "SoundbanksInfo.h"

#include <tinyxml2.h>
#ifndef _WIN32
#include <algorithm>
#endif

using namespace tinyxml2;

pohwaran::SoundbanksInfo::StreamedSoundbankFile read_entry(XMLElement* element) {
    pohwaran::SoundbanksInfo::StreamedSoundbankFile entry;
    entry.id = element->UnsignedAttribute("Id");
    const char* ptr = element->Attribute("Language");
    if (ptr != nullptr) {
        entry.language = ptr;
    }
    XMLElement* child = element->FirstChildElement("ShortName");
    if (child != nullptr) {
        ptr = child->GetText();
        if (ptr != nullptr) {
            std::string str = std::string(ptr);
#ifndef _WIN32
            if(str.find('\\')) {
                std::replace(str.begin(), str.end(), '\\', '/');
            }
#endif
            entry.short_name = str;
        }
    }
    child = element->FirstChildElement("Path");
    if (child != nullptr) {
        ptr = child->GetText();
        if (ptr != nullptr) {
            std::string str = std::string(ptr);
#ifndef _WIN32
            if(str.find('\\')) {
                std::replace(str.begin(), str.end(), '\\', '/');
            }
#endif
            entry.path = str;
        }
    }
    return entry;
}

pohwaran::SoundbanksInfo::SoundbanksInfo(std::filesystem::path soundbanks_path) {
    XMLDocument doc;

    if (doc.LoadFile(soundbanks_path.string().c_str()) != XML_SUCCESS) {
        return;
    }

    XMLElement* root = doc.RootElement();
    const char* xml_platform = root->Attribute("Platform");
    if (xml_platform != nullptr) {
        platform = xml_platform;
    }
    schema_version = root->UnsignedAttribute("SchemaVersion");
    version = root->UnsignedAttribute("SoundbankVersion");

    XMLElement* xml_streamed_files = root->FirstChildElement("StreamedFiles");
    if (xml_streamed_files != nullptr) {
        XMLElement* streamed_file = xml_streamed_files->FirstChildElement("File");
        while (streamed_file != nullptr) {
            StreamedSoundbankFile file = read_entry(streamed_file);
            streamed_files[file.id] = file;
            streamed_file = streamed_file->NextSiblingElement("File");
        }
    }

    XMLElement* xml_soundbanks = root->FirstChildElement("SoundBanks");
    if (xml_soundbanks != nullptr) {
        XMLElement* soundbank = xml_soundbanks->FirstChildElement("SoundBank");
        while (soundbank != nullptr) {
            SoundbankInfo info;
            info.soundbank_file = read_entry(soundbank);
            XMLElement* referenced_files = soundbank->FirstChildElement("ReferencedStreamedFiles");
            if (referenced_files != nullptr) {
                XMLElement* referenced_file = referenced_files->FirstChildElement("File");
                while (referenced_file != nullptr) {
                    uint32_t id = referenced_file->UnsignedAttribute("Id");
                    info.files.push_back(id);
                    referenced_file = referenced_file->NextSiblingElement("File");
                }
            }
            XMLElement* included_files = soundbank->FirstChildElement("IncludedMemoryFiles");
            if (included_files != nullptr) {
                XMLElement* included_file = included_files->FirstChildElement("File");
                while (included_file != nullptr) {
                    StreamedSoundbankFile file = read_entry(included_file);
                    info.included_memory_files[file.id] = file;
                    included_file = included_file->NextSiblingElement("File");
                }
            }
            /*
            XMLElement* excluded_files = soundbank->FirstChildElement("ExcludedMemoryFiles");
            if(excluded_files != nullptr) {
                XMLElement* excluded_file = excluded_files->FirstChildElement("File");
                while(excluded_file != nullptr) {
                    StreamedSoundbankFile file = read_entry(excluded_file);
                    LOG("Found excluded file " << file.path);
                    info.excluded_memory_files[file.id] = file;
                    excluded_file = excluded_file->NextSiblingElement("File");
                }
            }
             */
            soundbanks.push_back(info);
            soundbank = soundbank->NextSiblingElement("SoundBank");
        }
    }
}
