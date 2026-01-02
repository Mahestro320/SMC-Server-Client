#include "network/response/builder.hpp"

#include <string>

#include "Shared/network/response/error.hpp"
#include "Shared/network/response/type.hpp"
#include "Shared/pds.hpp"
#include "network/request/analyzer.hpp"

void ResponseBuilder::buildNormal() {
    data.clear();
    type = ResponseType::Ok;
    insertType();
}

void ResponseBuilder::buildError(ResponseErrorInfos infos) {
    data.clear();
    type = ResponseType::Error;
    error = infos.error_id;
    error_message = infos.precision_msg;
    insertType();
    insertInt<pds::response_error_t>(static_cast<pds::response_error_t>(infos.error_id));
    insertString<pds::response_msg_t>(infos.precision_msg);
}

void ResponseBuilder::buildErrorFromInvalidRequest(const RequestAnalyzer& request) {
    buildError(ResponseErrorInfos{
        .error_id = ResponseError::InvalidRequestData,
        .precision_msg = request.getLastError(),
    });
}

void ResponseBuilder::insertType() {
    insertInt<pds::response_type_t>(static_cast<pds::response_type_t>(type));
}

ResponseType ResponseBuilder::getType() const {
    return type;
}

ResponseError ResponseBuilder::getError() const {
    return error;
}

const std::string& ResponseBuilder::getErrorMessage() const {
    return error_message;
}

bool ResponseBuilder::isErrorMessage() const {
    return !error_message.empty();
}
