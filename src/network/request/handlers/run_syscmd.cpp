#include "network/request/handlers/run_syscmd.hpp"

#include <string>

#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "network/request/handler.hpp"

RunSyscmdRH::RunSyscmdRH() : RH{RequestId::RunSyscmd} {
}

bool RunSyscmdRH::_run() {
    request.insertString<pds::command_t>(command);
    return sendRequest() && checkResponse() && extractOutput();
}

bool RunSyscmdRH::extractOutput() {
    return response.extractString<pds::command_output_t>(output);
}

void RunSyscmdRH::setCommand(const std::string& command) {
    this->command = command;
}

const std::string& RunSyscmdRH::getOutput() const {
    return output;
}
