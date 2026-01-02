#include "Shared/io/console.hpp"

// always before Windows.h
#include <winsock2.h>

#include <Windows.h>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include "Shared/pds.hpp"
#include "Shared/util/time.hpp"
#include "common.hpp"
#include "io/logger.hpp"
#include "network/client.hpp"
#include "termcolor.hpp"
#include "util/resolver/con_str_prop.hpp"
#include "util/resolver/evs_str.hpp"
#include "util/thread.hpp"
#include "util/time.hpp"

/*
 * message format:
 * [<message type> @clt-<client count> | <date: DD/MM/YYYY> <time: HH:MM:SS>] <message>
 *
 * for example:
 * [INFO @clt-221 | 10/09/25 4:33:17] hello !
 *
 * Messages can contains colors, environnement vars and sysvars.
 * To set the color:
 * <:color=[COLOR]>
 *
 */

namespace {

using message_type_t = uint8_t;

enum class MessageType : message_type_t {
    Info,
    Warning,
    Error
};

struct MessageInfo final {
    MessageType type{};
    std::string message{};
    uint32_t thread_id{};
    pds::timestamp_t timestamp{};
    pds::client_count_t client_index{};
    std::ostream* stream{&std::cout};
};

std::mutex queue_mutex{};
std::deque<MessageInfo> queue{};
std::condition_variable cv{};

void changeColorAccordingToType(MessageType type, std::ostream& stream) {
    if (type == MessageType::Info) {
        stream << termcolor::green;
    } else if (type == MessageType::Warning) {
        stream << termcolor::yellow;
    } else if (type == MessageType::Error) {
        stream << termcolor::red;
    } else {
        stream << termcolor::white;
    }
}

std::string messageTypeToString(MessageType type) {
    if (type == MessageType::Info) {
        return "INFO";
    } else if (type == MessageType::Warning) {
        return "WARNING";
    } else if (type == MessageType::Error) {
        return "ERROR";
    }
    return "UNKNOWN";
}

std::string getMessageLocation(pds::client_count_t client_index, uint32_t thread_id) {
    if (thread_id == common::main_thread_id) {
        return "@Main";
    }
    return "@clt-" + std::to_string(client_index);
}

void printMessageContent(const std::string& message, std::ostream* stream) {
    ConStrPropResolver resolver{};
    resolver.setStream(stream);
    resolver.setText(message);
    resolver.print();
}

void printMessage(MessageInfo msg_info) {
    *msg_info.stream << termcolor::bright_white << '[';
    changeColorAccordingToType(msg_info.type, *msg_info.stream);
    *msg_info.stream << messageTypeToString(msg_info.type) << termcolor::blue << ' '
                     << getMessageLocation(msg_info.client_index, msg_info.thread_id) << termcolor::cyan << " | "
                     << termcolor::bright_green << util::time::formatTime(msg_info.timestamp) << termcolor::bright_white
                     << "] " << termcolor::white;
    printMessageContent(msg_info.message, msg_info.stream);
    *msg_info.stream << '\n';
}

void logMessage(MessageInfo msg_info) {
    Logger& logger{Logger::getInstance()};
    const std::string formated_message{'[' + messageTypeToString(msg_info.type) + ' ' +
                                       getMessageLocation(msg_info.client_index, msg_info.thread_id) + " | " +
                                       util::time::formatTime(msg_info.timestamp) + "] " + msg_info.message + '\n'};
    ConStrPropResolver resolver{};
    resolver.setStream(&logger.getStream());
    resolver.setText(formated_message);
    resolver.print();
}

std::string getResolvedMessage(const std::string& input) {
    EVSStrResolver<false> resolver{};
    resolver.setInput(input);
    resolver.resolve();
    return resolver.getOutput();
}

void processMessage(MessageInfo msg_info) {
    msg_info.timestamp = util::time::getSeconds();
    msg_info.thread_id = util::thread::getCurrentThreadId();
    if (msg_info.thread_id != common::main_thread_id) {
        try {
            const Client& client{Client::getInstance()};
            msg_info.client_index = client.getIndex();
        } catch (const std::exception& e) {
            std::cout << "console error: " << e.what() << std::endl;
        }
    }
    {
        std::lock_guard lock(queue_mutex);
        queue.push_back(std::move(msg_info));
    }
    cv.notify_one();
}

void printerThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cv.wait(lock, [] { return !queue.empty(); });
        MessageInfo msg_info{std::move(queue.front())};
        queue.pop_front();
        lock.unlock();
        MessageInfo resolved_msg_info{msg_info};
        resolved_msg_info.message = getResolvedMessage(msg_info.message);
        printMessage(resolved_msg_info);
        logMessage(resolved_msg_info);
    }
}

} // namespace

void console::init() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::thread t{[] { printerThread(); }};
    t.detach();
}

void console::out::inf(const std::string& message, bool /* end_line */) {
    processMessage(MessageInfo{
        .type = MessageType::Info,
        .message = message,
    });
}

void console::out::warn(const std::string& message, bool /* end_line */) {
    processMessage(MessageInfo{
        .type = MessageType::Warning,
        .message = "<:color=yellow>" + message,
    });
}

void console::out::err(const std::string& message, bool /* end_line */) {
    processMessage(MessageInfo{
        .type = MessageType::Error,
        .message = "<:color=red>" + message,
        .stream = &std::cerr,
    });
}

void console::out::err(const std::exception& e, bool /* end_line */) {
    console::out::err(e.what());
}

void console::out::verbose(const std::string&, bool) {
}
