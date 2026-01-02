#pragma once

#include "Shared/network/array_editor.hpp"
#include "Shared/network/request/id.hpp"
#include "network/with_client.hpp"

struct ResponseErrorInfos;

// Request Handler
class RH : public WithClient {
  private:
    RequestId id{};

    bool readResponse();
    void processResponseError();
    void printResponseLastError();
    void printResponseError(const ResponseErrorInfos& infos);

  protected:
    ArrayBuilder request{};
    ArrayAnalyzer response{};

    bool sendRequest();
    bool checkResponse();

    virtual bool _run() = 0;

  public:
    RH() = delete;
    explicit RH(RequestId id);

    [[nodiscard]] bool run();
};
