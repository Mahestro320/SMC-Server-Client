#include "shell/commands/io_load/file_downloader.hpp"

#include <ostream>
#include <string>

#include "shell/system/cursor.hpp"
#include "util/byte.hpp"

void FileDownloader::setStream(std::ostream* stream) {
    this->stream = stream;
}

bool FileDownloader::downloadFile() {
    initHandler();
    if (!handler.run()) {
        return false;
    }
    console::out::inf("downloading file \"" + target_path.string() + "\" (" + getFileSizeAsStr() + ')');
    shell::cursor::setVisible(false);
    initInfoPrinter();
    while (handler.isAvailable()) {
        if (!handler.downloadNextBuffer()) {
            shell::cursor::setVisible(true);
            return false;
        }
        info_printer.setCurrentBufferCount(handler.getCurrentBufferIndex());
        info_printer.update();
    }
    info_printer.clear();
    shell::cursor::setVisible(true);
    return true;
}

void FileDownloader::initHandler() {
    handler.setClient(client);
    handler.setPath(target_path);
    handler.setStream(stream);
}

void FileDownloader::initInfoPrinter() {
    info_printer.setTotalBufferCount(handler.getBufferCount());
    info_printer.setBufferSize(handler.getBufferSize());
    info_printer.init();
}

std::string FileDownloader::getFileSizeAsStr() {
    return util::byte::toAutoUnit(handler.getFileSize());
}
