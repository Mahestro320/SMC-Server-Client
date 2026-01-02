#pragma once

#include <cstdint>

// SMC Protocol Data Sizes (PDS)
namespace pds {

using path_t = uint16_t;
using bool_t = uint8_t;

using role_t = uint8_t;
using version_t = uint64_t;
using restart_mode_t = uint8_t;
using request_id_t = uint8_t;

using client_count_t = uint32_t;
using connection_port_t = uint16_t;
using wrong_pw_count_t = uint32_t;

using timestamp_t = uint64_t;

using command_t = uint32_t;
using command_output_t = command_t;

using response_type_t = uint8_t;
using response_error_t = uint8_t;
using response_msg_t = uint16_t;

using file_size_t = uint64_t;
using file_count_t = uint32_t;
using file_type_t = uint8_t;
using file_name_t = path_t;

using password_t = uint8_t;
using username_t = uint8_t;
using user_count_t = uint32_t;

using network_buffer_t = uint32_t;
using network_buffer_count_t = uint64_t;
using bfl_t = uint32_t;

} // namespace pds
