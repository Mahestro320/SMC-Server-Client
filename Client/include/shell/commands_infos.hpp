#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Shared/user/role.hpp"
#include "Shared/util/concepts.hpp"
#include "shell/command.hpp"
#include "shell/commands/chdir.hpp"
#include "shell/commands/clear.hpp"
#include "shell/commands/client.hpp"
#include "shell/commands/conf.hpp"
#include "shell/commands/copy.hpp"
#include "shell/commands/delete.hpp"
#include "shell/commands/dir.hpp"
#include "shell/commands/download.hpp"
#include "shell/commands/edit.hpp"
#include "shell/commands/help.hpp"
#include "shell/commands/infos.hpp"
#include "shell/commands/lang.hpp"
#include "shell/commands/login.hpp"
#include "shell/commands/logout.hpp"
#include "shell/commands/mkdir.hpp"
#include "shell/commands/mkfile.hpp"
#include "shell/commands/move.hpp"
#include "shell/commands/print.hpp"
#include "shell/commands/quit.hpp"
#include "shell/commands/rename.hpp"
#include "shell/commands/server.hpp"
#include "shell/commands/syscmd.hpp"
#include "shell/commands/upload.hpp"
#include "shell/commands/user.hpp"

namespace command_factory {

template<is_base_of<Command> T> std::unique_ptr<Command> create() {
    return std::make_unique<T>();
}

} // namespace command_factory

struct CommandInfos {
    std::unique_ptr<Command> (*cmd)(){};
    std::string short_name{};
    bool can_use_offline{};
    bool must_be_logged{};
    Role min_role{Role::User};
};

namespace command_names {

inline const std::string HELP{"help"};
inline const std::string INFOS{"infos"};
inline const std::string CLEAR{"clear"};
inline const std::string QUIT{"quit"};
inline const std::string CONF{"conf"};
inline const std::string PRINT{"print"};
inline const std::string LOGIN{"login"};
inline const std::string LOGOUT{"logout"};
inline const std::string CLIENT{"client"};
inline const std::string SERVER{"server"};
inline const std::string DIR{"dir"};
inline const std::string CHDIR{"chdir"};
inline const std::string DOWNLOAD{"download"};
inline const std::string UPLOAD{"upload"};
inline const std::string SYSCMD{"syscmd"};
inline const std::string MKFILE{"mkfile"};
inline const std::string MKDIR{"mkdir"};
inline const std::string DELETE_{"delete"};
inline const std::string RENAME{"rename"};
inline const std::string EDIT{"edit"};
inline const std::string LANG{"lang"};
inline const std::string USER{"user"};
inline const std::string MOVE{"move"};
inline const std::string COPY{"copy"};

namespace _short {

inline const std::string CLEAR{"clr"};
inline const std::string LOGIN{"lgi"};
inline const std::string LOGOUT{"lgo"};
inline const std::string CLIENT{"clt"};
inline const std::string SERVER{"srv"};
inline const std::string CHDIR{"cd"};
inline const std::string DOWNLOAD{"dwl"};
inline const std::string UPLOAD{"upl"};
inline const std::string SYSCMD{"scm"};
inline const std::string MKFILE{"mf"};
inline const std::string MKDIR{"md"};
inline const std::string DELETE_{"del"};
inline const std::string RENAME{"ren"};
inline const std::string USER{"usr"};
inline const std::string MOVE{"mov"};
inline const std::string COPY{"cpy"};

} // namespace _short

} // namespace command_names

static inline std::unordered_map<std::string, CommandInfos> commands_infos{
    {command_names::HELP,
     CommandInfos{
         .cmd = &command_factory::create<HelpCommand>,
         .can_use_offline = true,
     }},
    {command_names::INFOS,
     CommandInfos{
         .cmd = &command_factory::create<InfosCommand>,
         .can_use_offline = true,
     }},
    {command_names::CLEAR,
     CommandInfos{
         .cmd = &command_factory::create<ClearCommand>,
         .short_name = command_names::_short::CLEAR,
         .can_use_offline = true,
     }},
    {command_names::QUIT,
     CommandInfos{
         .cmd = &command_factory::create<QuitCommand>,
         .can_use_offline = true,
     }},
    {command_names::CONF,
     CommandInfos{
         .cmd = &command_factory::create<ConfCommand>,
         .can_use_offline = true,
     }},
    {command_names::PRINT,
     CommandInfos{
         .cmd = &command_factory::create<PrintCommand>,
         .can_use_offline = true,
     }},
    {command_names::LOGIN,
     CommandInfos{
         .cmd = &command_factory::create<LoginCommand>,
         .short_name = command_names::_short::LOGIN,
     }},
    {command_names::LOGOUT,
     CommandInfos{
         .cmd = &command_factory::create<LogoutCommand>,
         .short_name = command_names::_short::LOGOUT,
         .must_be_logged = true,
     }},
    {command_names::CLIENT,
     CommandInfos{
         .cmd = &command_factory::create<ClientCommand>,
         .short_name = command_names::_short::CLIENT,
     }},
    {command_names::SERVER,
     CommandInfos{
         .cmd = &command_factory::create<ServerCommand>,
         .short_name = command_names::_short::SERVER,
     }},
    {command_names::DIR,
     CommandInfos{
         .cmd = &command_factory::create<DirCommand>,
         .must_be_logged = true,
     }},
    {command_names::CHDIR,
     CommandInfos{
         .cmd = &command_factory::create<ChDirCommand>,
         .short_name = command_names::_short::CHDIR,
         .must_be_logged = true,
     }},
    {command_names::DOWNLOAD,
     CommandInfos{
         .cmd = &command_factory::create<DownloadCommand>,
         .short_name = command_names::_short::DOWNLOAD,
         .must_be_logged = true,
     }},
    {command_names::UPLOAD,
     CommandInfos{
         .cmd = &command_factory::create<UploadCommand>,
         .short_name = command_names::_short::UPLOAD,
         .must_be_logged = true,
     }},
    {command_names::SYSCMD,
     CommandInfos{
         .cmd = &command_factory::create<SyscmdCommand>,
         .short_name = command_names::_short::SYSCMD,
         .must_be_logged = true,
         .min_role = Role::Developer,
     }},
    {command_names::MKFILE,
     CommandInfos{
         .cmd = &command_factory::create<MkFileCommand>,
         .short_name = command_names::_short::MKFILE,
         .must_be_logged = true,
     }},
    {command_names::MKDIR,
     CommandInfos{
         .cmd = &command_factory::create<MkDirCommand>,
         .short_name = command_names::_short::MKDIR,
         .must_be_logged = true,
     }},
    {command_names::DELETE_,
     CommandInfos{
         .cmd = &command_factory::create<DeleteCommand>,
         .short_name = command_names::_short::DELETE_,
         .must_be_logged = true,
     }},
    {command_names::RENAME,
     CommandInfos{
         .cmd = &command_factory::create<RenameCommand>,
         .short_name = command_names::_short::RENAME,
         .must_be_logged = true,
     }},
    {command_names::EDIT,
     CommandInfos{
         .cmd = &command_factory::create<EditCommand>,
         .must_be_logged = true,
     }},
    {command_names::LANG,
     CommandInfos{
         .cmd = &command_factory::create<LangCommand>,
         .can_use_offline = true,
     }},
    {command_names::USER,
     CommandInfos{
         .cmd = &command_factory::create<UserCommand>,
         .short_name = command_names::_short::USER,
         .must_be_logged = true,
     }},
    {command_names::MOVE,
     CommandInfos{
         .cmd = &command_factory::create<MoveCommand>,
         .short_name = command_names::_short::MOVE,
         .must_be_logged = true,
     }},
    {command_names::COPY,
     CommandInfos{
         .cmd = &command_factory::create<CopyCommand>,
         .short_name = command_names::_short::COPY,
         .must_be_logged = true,
     }},
};
