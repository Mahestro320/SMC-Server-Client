#include "shell/commands/dir.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/io/file_info.hpp"
#include "Shared/io/file_type.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_dir_content.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "users/user.hpp"
#include "util/byte.hpp"
#include "util/math.hpp"
#include "util/string.hpp"

using boost::asio::ip::tcp;

exit_code_t DirCommand::run() {
    tcp::socket& socket{client->getSocket()};
    IOGetDirContentRH handler{};
    handler.setClient(client);
    const User& user{client->getUser()};
    handler.setPath(user.current_dir / ((args.empty()) ? "" : args[0]));
    if (!handler.run()) {
        return Error;
    }
    data = handler.getData();
    sortData();
    printData();
    return Success;
}

void DirCommand::sortData() {
    std::vector<FileInfo> directories{};
    std::vector<FileInfo> generic_files{};
    for (const FileInfo& file_info : data) {
        if (file_info.type == FileType::Directory) {
            directories.push_back(file_info);
        } else {
            generic_files.push_back(file_info);
        }
    }
    data.clear();
    data.reserve(directories.size() + generic_files.size());
    data.insert(data.end(), directories.begin(), directories.end());
    data.insert(data.end(), generic_files.begin(), generic_files.end());
}

void DirCommand::printData() const {
    const User& user{client->getUser()};
    console::out::verbose();
    console::out::inf(lang::gt("command.dir.content_of", user.current_dir.string(), data.size()));
    printArrayHeader();
    printArrayContent();
}

void DirCommand::printArrayHeader() const {
    const std::string& type{lang::gt("command.dir.type")};
    const std::string& size{lang::gt("command.dir.size")};
    const std::string& name{lang::gt("command.dir.name")};

    console::out::inf(VERTICAL_BAR + type + SPACE +
                      util::string::makeFilledString(util::math::minusNotNul(TYPE_SPACES_COUNT, type.size())) +
                      VERTICAL_BAR + size + SPACE +
                      util::string::makeFilledString(util::math::minusNotNul(SIZE_SPACES_COUNT, size.size())) +
                      VERTICAL_BAR + name + NEW_LINE);
}

void DirCommand::printArrayContent() const {
    if (data.empty()) {
        console::out::inf(lang::gt("command.dir.empty"));
        return;
    }
    const size_t vb_size{std::string{VERTICAL_BAR}.size() + 1};
    for (const FileInfo& file : data) {
        const std::string type_str{file_type::getName(file.type)};
        const std::string size_str{(file.type == FileType::File) ? util::byte::toAutoUnit(file.size) : ""};
        console::out::inf(
            BEGIN_SPACES + type_str +
            util::string::makeFilledString(util::math::minusNotNul(TYPE_SPACES_COUNT, type_str.size()) + vb_size) +
            size_str +
            util::string::makeFilledString(util::math::minusNotNul(SIZE_SPACES_COUNT, size_str.size()) + vb_size) +
            file.name);
    }
}
