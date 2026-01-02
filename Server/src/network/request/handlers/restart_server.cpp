#include "network/request/handlers/restart_server.hpp"

// always first
#include <winsock2.h>

#include <chrono>
#include <string>
#include <thread>
#include <windows.h>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "Shared/server/restart_modes.hpp"
#include "Shared/util/string.hpp"
#include "common.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"
#include "users/user.hpp"

RestartServerRH::RestartServerRH() : RH{REQUIREMENTS} {
}

void RestartServerRH::_run() {
    if (!extractRestartMode() || !getRestartFuncPtr()) {
        return;
    }
    if (restart_func_ptr) {
        std::thread t{[this] {
            std::this_thread::sleep_for(std::chrono::seconds{1});
            (this->*restart_func_ptr)();
        }};
        t.detach();
    } else {
        console::out::err("the restart function is nullptr");
    }
}

bool RestartServerRH::extractRestartMode() {
    pds::restart_mode_t mode_int{};
    if (!request.extractInt<pds::restart_mode_t>(mode_int)) {
        response.buildErrorFromInvalidRequest(request);
        return false;
    }
    mode = static_cast<RestartMode>(mode_int);
    return true;
}

bool RestartServerRH::getRestartFuncPtr() {
    std::string message{"restarting server "};
    if (mode == RestartMode::Machine) {
        message += "(machine)";
        restart_func_ptr = &RestartServerRH::restartMachine;
    } else if (mode == RestartMode::Program) {
        message += "(program)";
        restart_func_ptr = &RestartServerRH::restartProgram;
    } else {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::UnknownRestartMode,
            .precision_msg = "unknown restart mode: " + std::to_string(static_cast<pds::restart_mode_t>(mode)),
        });
        return false;
    }
    console::out::inf(message);
    return true;
}

void RestartServerRH::restartMachine() {
    if (!enableShutdownPrivilege()) {
        return;
    }

    const User& user{client->getUser()};
    std::wstring message{L"The system will restart: request from user " +
                         util::string::stringToWString(user.name.data())};
    const DWORD timeout_seconds{0};
    const BOOL force_apps_closed{TRUE};
    const BOOL reboot_after_shutdown{TRUE};
    const DWORD reason{SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED};

    BOOL result{InitiateSystemShutdownExW(nullptr, message.data(), timeout_seconds, force_apps_closed,
                                          reboot_after_shutdown, reason)};
    if (!result) {
        console::out::err("error while rebooting: " + std::to_string(GetLastError()));
    }
}

bool RestartServerRH::enableShutdownPrivilege() {
    HANDLE h_token{};
    TOKEN_PRIVILEGES tkp{};

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token)) {
        console::out::err("error while rebooting: can't get process token");
        return false;
    }

    LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(h_token, FALSE, &tkp, 0, nullptr, nullptr);

    if (GetLastError() != ERROR_SUCCESS) {
        console::out::err("error while rebooting: insufficient privileges to restart");
        return false;
    }

    CloseHandle(h_token);
    return true;
}

void RestartServerRH::restartProgram() {
    // todo
    common::shutdown_requested.store(true, std::memory_order_release);
}
