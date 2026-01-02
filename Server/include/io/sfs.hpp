#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Shared/io/file_type.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"

class Client;
class ResponseBuilder;

/*
 * Here is what the abbreviated names in this code mean:
 *
 * SFS: Secured Filesystem
 * PUD: Protected User Data
 * UPC: User Personnal Content
 *
 */

class SFS final {
  private:
    static inline const std::string INVALID_FILENAME_CHARS{"<>:\"/\\|?*"};
    static inline const std::string RESERVED_FILE_NAMES[] = {
        "con",  "prn",  "aux",  "nul",  "com1", "com2", "com3", "com4", "com5", "com6", "com7",
        "lpt8", "lpt9", "lpt1", "lpt2", "lpt3", "lpt4", "lpt5", "lpt6", "lpt7", "lpt8", "lpt9"};

    Client& client;

    bool beginCopyOrMove(const std::filesystem::path& input_path, const std::filesystem::path& target_path,
                         ResponseBuilder* response) const;
    void buildResponse(const ResponseErrorInfos& infos, ResponseBuilder* response) const;

  public:
    SFS(Client& client);

    std::filesystem::path absoluteFromUserScope(const std::filesystem::path& path) const;
    std::filesystem::path relativeToUserScope(const std::filesystem::path& path) const;
    std::filesystem::path relative(const std::filesystem::path& path, const std::filesystem::path& base) const;
    std::filesystem::path real(const std::filesystem::path& path) const;
    std::filesystem::path removeEndBackslash(const std::filesystem::path& path) const;
    std::vector<std::filesystem::path> getDirectoryFileList(const std::filesystem::path& path) const;
    FileType getFileType(const std::filesystem::path& path) const;
    pds::file_size_t getFileSize(const std::filesystem::path& path) const;

    bool isInDirectory(const std::filesystem::path& path, const std::filesystem::path& directory) const;
    bool canLexicallyAccess(const std::filesystem::path& path) const;
    bool exists(const std::filesystem::path& path) const;
    bool isSame(const std::filesystem::path& first, const std::filesystem::path& second) const;
    bool isRegularFile(const std::filesystem::path& path) const;
    bool isDirectory(const std::filesystem::path& path) const;
    bool isInUPCDirectory(const std::filesystem::path& path) const;
    bool isPUDDirectory(const std::filesystem::path& path) const;
    bool isUIFFile(const std::filesystem::path& path) const;
    bool isValidName(const std::string& name) const;

    bool rename(const std::filesystem::path& path, const std::string& new_name,
                ResponseBuilder* response = nullptr) const;
    bool move(const std::filesystem::path& input_path, std::filesystem::path output_path,
              ResponseBuilder* response = nullptr) const;
    bool copy(const std::filesystem::path& input_path, std::filesystem::path output_path,
              ResponseBuilder* response = nullptr) const;

    bool checkPathAlreadyExist(const std::filesystem::path& path, ResponseBuilder* response = nullptr) const;
    bool checkPathNotExist(const std::filesystem::path& path, ResponseBuilder* response = nullptr) const;
};
