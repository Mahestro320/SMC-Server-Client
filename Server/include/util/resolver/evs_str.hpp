#pragma once

/*
 * EVSStrResolver (Env Var and Sysvar String Resolver)
 *
 * Modify a string and replaces vars between <> by the real value
 * Supports env var and sysvars but not the properties
 *
 * Env vars syntax:
 * <[VAR NAME]>
 *
 * Sysvars syntax:
 * <$[SYSVAR]>
 *
 * See README.md for sysvars names
 */

#include <cstdint>
#include <cstdlib>
#include <malloc.h>
#include <string>

#include "Shared/io/console.hpp"
#include "common.hpp"
#include "server/sysvar.hpp"

using evs_type_t = uint8_t;

enum class VarType : evs_type_t {
    Environment,
    Sysvar,
    Property
};

template<bool EnableErrors = true> class EVSStrResolver final {
  private:
    std::string input{}, output{};
    bool is_in_var{};
    std::string current_var{};

    void printError(const std::string& message) {
        if constexpr (EnableErrors) {
            console::out::err(message);
        }
    }

    bool processNextChar(char c) {
        if (c == '<') {
            if (is_in_var && EnableErrors) {
                printError("syntax error: two var opening found");
                return false;
            }
            is_in_var = true;
            return true;
        }
        if (c == '>') {
            if (!is_in_var && EnableErrors) {
                printError("syntax error: close of var found without opening");
                return false;
            }
            is_in_var = false;
            if (!insertVar()) {
                return false;
            }
            current_var.clear();
            return true;
        }
        flushChar(c);
        return true;
    }

    void flushChar(char c) {
        if (is_in_var) {
            current_var += c;
        } else {
            output += c;
        }
    }

    bool insertVar() {
        const VarType var_type{getVarType()};
        if (insertEmptyIfEmpty(var_type)) {
            return !EnableErrors;
        }
        if (var_type == VarType::Sysvar) {
            return insertSysvar();
        } else if (var_type == VarType::Environment) {
            return insertEnvVar();
        }
        output += "<" + current_var + '>';
        return true;
    }

    bool insertEmptyIfEmpty(VarType var_type) {
        if (current_var.empty() || (current_var.size() == 1 && var_type != VarType::Property)) {
            if constexpr (EnableErrors) {
                printError("syntax error: empty env var");
            } else {
                output +=
                    "<" +
                    std::string{(var_type == VarType::Property) ? ":" : ((var_type == VarType::Sysvar) ? "$" : "")} +
                    ">";
            }
            return true;
        }
        return false;
    }

    VarType getVarType() {
        if (current_var.empty() || current_var.starts_with(":")) {
            return VarType::Property;
        } else if (current_var.starts_with("$")) {
            return VarType::Sysvar;
        }
        return VarType::Environment;
    }

    bool insertSysvar() {
        std::string sysvar_val{sysvar::get(current_var.substr(1))};
        if (sysvar_val.empty()) {
            if constexpr (EnableErrors) {
                printError("unknown sysval: " + current_var.substr(1));
                return false;
            }
            output += common::UNDEFINED_TEXT;
            return true;
        }
        output += sysvar_val;
        return true;
    }

    bool insertEnvVar() {
        char* env_var_val{};
        size_t size{};
        if (_dupenv_s(&env_var_val, &size, current_var.c_str()) != 0 || !env_var_val) {
            if constexpr (EnableErrors) {
                printError("error: unknown env var: " + current_var);
                return false;
            }
            output += '<' + current_var + '>';
            return true;
        }
        output += env_var_val;
        free(env_var_val);
        return true;
    }

  public:
    EVSStrResolver() = default;

    void setInput(const std::string& input) {
        this->input = input;
    }

    bool resolve() {
        for (char c : input) {
            if (!processNextChar(c)) {
                output = input;
                return false;
            }
        }
        return true;
    }

    const std::string& getOutput() const {
        return output;
    }
};
