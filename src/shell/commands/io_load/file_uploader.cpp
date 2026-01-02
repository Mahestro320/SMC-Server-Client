#include "shell/commands/io_load/file_uploader.hpp"

#include <istream>
#include <string>

#include "Shared/pds.hpp"
#include "shell/system/cursor.hpp"
#include "util/byte.hpp"

void FileUploader::setStream(std::istream* stream) {
    this->stream = stream;
}

void FileUploader::setStreamSize(pds::file_size_t size) {
    this->size = size;
}

bool FileUploader::uploadFile() {
    initHandler();
    if (!handler.run()) {
        return false;
    }
    console::out::inf("uploading file \"" + target_path.string() + "\" (" + getFileSizeAsStr() + ')');
    shell::cursor::setVisible(false);
    initInfoPrinter();
    while (handler.isAvailable()) {
        if (!handler.uploadNextBuffer()) {
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

void FileUploader::initHandler() {
    handler.setClient(client);
    handler.setPath(target_path);
    handler.setStream(stream);
    handler.setStreamSize(size);
}

void FileUploader::initInfoPrinter() {
    info_printer.setTotalBufferCount(handler.getBufferCount());
    info_printer.setBufferSize(handler.getBufferSize());
    info_printer.init();
}

std::string FileUploader::getFileSizeAsStr() {
    return util::byte::toAutoUnit(handler.getFileSize());
}
