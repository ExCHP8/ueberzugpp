// Display images inside a terminal
// Copyright (C) 2023  JustKidding
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "application.hpp"
#include "logging.hpp"
#include "process_info.hpp"
#include "os.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Application::Application():
terminal(ProcessInfo(os::get_pid()))
{
    canvas = Canvas::init(terminal);
}

Application::~Application()
{
    canvas->clear();
}

auto Application::execute(const std::string& cmd) -> void
{
    json j;
    try {
        j = json::parse(cmd);
    } catch (const json::parse_error& e) {
        logger << "There was an error parsing the command." << std::endl;
        return;
    }
    logger << "=== Command received:\n" << j.dump() << std::endl;
    if (j["action"] == "add") {
        int max_width = static_cast<int>(j["max_width"]) - 1;
        int max_height = static_cast<int>(j["max_height"]) - 1;
        int x = static_cast<int>(j["x"]);
        int y = static_cast<int>(j["y"]);
        canvas->create(x, y, max_width, max_height);
        image = Image::load(j["path"], max_width, max_height, terminal);
        if (!image) {
            logger << "Unable to load image file." << std::endl;
            return;
        }
        canvas->draw(*image);
    } else if (j["action"] == "remove") {
        canvas->clear();
    } else {
        logger << "=== Command not supported!" << std::endl;
    }
}