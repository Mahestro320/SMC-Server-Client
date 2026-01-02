#include "shell/commands/clear.hpp"

#include <Windows.h>
#include <string.h>

#include "Shared/io/console.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "shell/system/cursor.hpp"
#include "util/vector2.hpp"

exit_code_t ClearCommand::run() {
    return (getConsoleHandle() && getConsoleModes() && setNewConsoleMode() && clearScreen() &&
            restoreOriginalConsoleMode())
               ? Silent
               : Error;
}

bool ClearCommand::getConsoleHandle() {
    h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == INVALID_HANDLE_VALUE) {
        console::out::err(lang::gt("command.clear.error.get_handle"));
        return false;
    }
    return true;
}

bool ClearCommand::getConsoleModes() {
    DWORD current_mode{};
    if (!GetConsoleMode(h_console, &current_mode)) {
        console::out::err(lang::gt("command.clear.error.get_new_mode"));
        return false;
    }
    original_mode = current_mode;
    current_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    new_mode = current_mode;
    return true;
}

bool ClearCommand::setNewConsoleMode() const {
    if (!SetConsoleMode(h_console, new_mode)) {
        console::out::err(lang::gt("command.clear.error.set_new_mode"));
        return false;
    }
    return true;
}

bool ClearCommand::clearScreen() const {
    DWORD written{};
    if (!WriteConsoleA(h_console, SEQ_CLR_VISIBLE_SCRN, static_cast<DWORD>(strlen(SEQ_CLR_VISIBLE_SCRN)), &written,
                       nullptr) ||
        !WriteConsoleA(h_console, SEQ_CLR_SCROLLBACK, static_cast<DWORD>(strlen(SEQ_CLR_SCROLLBACK)), &written,
                       nullptr)) {
        console::out::err(lang::gt("io.error.write"));
        return false;
    }
    shell::cursor::move(Vector2<shell::cursor::range_t>{0, 0});
    return true;
}

bool ClearCommand::restoreOriginalConsoleMode() const {
    if (!SetConsoleMode(h_console, original_mode)) {
        console::out::err(lang::gt("command.clear.error.restore_mode"));
        return false;
    }
    return true;
}
