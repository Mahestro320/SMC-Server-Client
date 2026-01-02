#include "network/request/handlers/run_syscmd.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "cstdio"
#include "network/request/handler.hpp"

RunSyscmdRH::RunSyscmdRH() : RH{REQUIREMENTS} {
}

void RunSyscmdRH::_run() {
    if (extractCommand() && runCommand()) {
        insertOutput();
    }
}

bool RunSyscmdRH::extractCommand() {
    return request.extractString<pds::command_t>(command);
}

bool RunSyscmdRH::runCommand() {
    console::out::inf("executing command '" + command + "'");
    command += " 2>&1"; // redirects stderr to stdout
    FILE* pipe{_popen(command.data(), "r")};
    if (!pipe) {
        response.buildError(ResponseErrorInfos{.error_id = ResponseError::OpenPipeError});
        return false;
    }
    char buffer[256]{};
    while (fgets(buffer, 256, pipe)) {
        output += buffer;
    }
    const int code{_pclose(pipe)};
    if (code != 0) {
        output += "\nreturn code: " + std::to_string(code);
    }
    return true;
}

void RunSyscmdRH::insertOutput() {
    response.insertString<pds::command_output_t>(output);
}
