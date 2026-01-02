#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class IOGetRealPathRH : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::filesystem::path input_path{}, real_path{};

    void _run() override;

    bool extractInputPath();
    bool checkInputPath();
    void getRealPath();
    void insertRealPath();

  public:
    IOGetRealPathRH();
};
