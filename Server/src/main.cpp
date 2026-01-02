/*
 * SMC Server (Super Mega Cool Server)
 * created the 18/06/25 (DD/MM/YY)
 *
 * by Mahestro_320
 */

#include "server.hpp"

#include <exception>
#include <string>

#include "Shared/io/console.hpp"

int main(int /* argc */, char** /* argv */) {
    try {
        server::run();
    } catch (const std::bad_alloc& e) {
        console::out::err("memory error: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err(e.what());
    }
    return 0;
}
