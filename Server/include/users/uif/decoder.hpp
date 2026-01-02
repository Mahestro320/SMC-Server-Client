#pragma once

#include <filesystem>
#include <vector>

#include "Shared/pds.hpp"
#include "values.hpp"

// UIF = User Informations

class UIFDecoder final {
  private:
    static constexpr pds::file_size_t UN_PW_SIZE{0x10};
    static constexpr pds::file_size_t SIGNATURE_SIZE{4};
    static constexpr char SIGNATURE[SIGNATURE_SIZE]{'U', 'I', 'F', 0x00};

    static constexpr pds::file_size_t SIGNATURE_OFFSET{0x00};
    static constexpr pds::file_size_t ROLE_OFFSET{SIGNATURE_SIZE};
    static constexpr pds::file_size_t USERNAME_BLOCK_OFFSET{0x10};
    static constexpr pds::file_size_t USERNAME_BLOCK_SIZE{UN_PW_SIZE};
    static constexpr pds::file_size_t PASSWORD_BLOCK_OFFSET{USERNAME_BLOCK_OFFSET + USERNAME_BLOCK_SIZE};
    static constexpr pds::file_size_t PASSWORD_BLOCK_SIZE{UN_PW_SIZE};

    static constexpr pds::file_size_t DATA_SIZE{PASSWORD_BLOCK_OFFSET + PASSWORD_BLOCK_SIZE};

    UIFValues values{};
    std::vector<char> data{};

    bool checkSize() const;
    bool checkSignature() const;
    void getRole();
    void getUsername();
    void getPassword();

  public:
    UIFDecoder() = default;

    bool loadDataFromPath(const std::filesystem::path& path);
    bool decode();
    const UIFValues& getValues() const;
};
