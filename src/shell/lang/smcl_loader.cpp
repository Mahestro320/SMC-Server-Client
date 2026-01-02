#include "shell/lang/smcl_loader.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "Shared/io/console.hpp"
#include "shell/lang.hpp"
#include "shell/lang/smcl_lexer.hpp"
#include "shell/lang/smcl_parser.hpp"

void SMCLLoader::setData(LangData* data) {
    this->data = data;
}

bool SMCLLoader::load(const std::filesystem::path& path) {
    console::out::inf("[SMCL loader] loading lang file " + path.string());
    std::ifstream file{path};
    if (!file.is_open()) {
        console::out::err("[SMCL loader] error while loading lang file");
        return false;
    }
    const std::string file_content{std::istreambuf_iterator<char>(file), {}};
    SMCLLexer lexer{&file_content};
    SMCLParser parser{&lexer};
    parser.setData(data);
    if (!parser.parse()) {
        console::out::err("[SMCL loader] the parser got an error !");
        return false;
    }
    console::out::verbose("[SMCL loader] successfully loaded " + std::to_string(data->translations.size()) +
                          " translations");
    return true;
}
