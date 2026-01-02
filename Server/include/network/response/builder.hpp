#pragma once

#include <string>

#include "Shared/network/array_editor.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/network/response/type.hpp"

class RequestAnalyzer;

class ResponseBuilder final : public ArrayBuilder {
  private:
    ResponseType type{};
    ResponseError error{};
    std::string error_message{};

    void insertType();

  public:
    ResponseBuilder() = default;

    void buildNormal();
    void buildError(ResponseErrorInfos infos);
    void buildErrorFromInvalidRequest(const RequestAnalyzer& request);

    ResponseType getType() const;
    ResponseError getError() const;
    const std::string& getErrorMessage() const;

    bool isErrorMessage() const;
};
