#include "save_load.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

// 定义 Player 类的 to_json 函数
void to_json(json& j, const Player& p) {
    j = json{
        {"x", p.x},
        {"y", p.y},
        {"targetX", p.targetX},
        {"targetY", p.targetY},
        {"stepProgress", p.stepProgress},
        {"isStepping", p.isStepping},
        {"direction", p.direction},
        {"optionTargetX", p.optionTargetX},
        {"optionTargetY", p.optionTargetY},
        {"isFree", p.isFree},
        {"isMoving", p.isMoving},
        {"isCutting", p.isCutting},
        {"isStoring", p.isStoring},
        {"isPickingUp", p.isPickingUp},
        {"isDropping", p.isDropping},
        {"isFetching", p.isFetching},
        {"isBuilding", p.isBuilding},
        {"buildProgress", p.buildProgress},
        {"cutProgress", p.cutProgress},
        {"carryingWood", p.carryingWood},
        {"path", p.path},
        {"unreachableTargets", std::vector<int>(p.unreachableTargets.begin(), p.unreachableTargets.end())}
    };
}

// 定义 Player 类的 from_json 函数
void from_json(const json& j, Player& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("targetX").get_to(p.targetX);
    j.at("targetY").get_to(p.targetY);
    j.at("stepProgress").get_to(p.stepProgress);
    j.at("isStepping").get_to(p.isStepping);
    j.at("direction").get_to(p.direction);
    j.at("optionTargetX").get_to(p.optionTargetX);
    j.at("optionTargetY").get_to(p.optionTargetY);
    j.at("isFree").get_to(p.isFree);
    j.at("isMoving").get_to(p.isMoving);
    j.at("isCutting").get_to(p.isCutting);
    j.at("isStoring").get_to(p.isStoring);
    j.at("isPickingUp").get_to(p.isPickingUp);
    j.at("isDropping").get_to(p.isDropping);
    j.at("isFetching").get_to(p.isFetching);
    j.at("isBuilding").get_to(p.isBuilding);
    j.at("buildProgress").get_to(p.buildProgress);
    j.at("cutProgress").get_to(p.cutProgress);
    j.at("carryingWood").get_to(p.carryingWood);
    j.at("path").get_to(p.path);
    std::vector<int> unreachableTargets;
    j.at("unreachableTargets").get_to(unreachableTargets);
    p.unreachableTargets = std::unordered_set<int>(unreachableTargets.begin(), unreachableTargets.end());
}

// 定义 Animal 类的 to_json 函数
void to_json(json& j, const Animal& a) {
    j = json{
        {"x", a.x},
        {"y", a.y},
        {"targetX", a.targetX},
        {"targetY", a.targetY},
        {"stepProgress", a.stepProgress},
        {"isStepping", a.isStepping},
        {"direction", a.direction},
        {"img_direction", a.img_direction}
    };
}

// 定义 Animal 类的 from_json 函数
void from_json(const json& j, Animal& a) {
    j.at("x").get_to(a.x);
    j.at("y").get_to(a.y);
    j.at("targetX").get_to(a.targetX);
    j.at("targetY").get_to(a.targetY);
    j.at("stepProgress").get_to(a.stepProgress);
    j.at("isStepping").get_to(a.isStepping);
    j.at("direction").get_to(a.direction);
    j.at("img_direction").get_to(a.img_direction);
}

// 定义 Tile 类的 to_json 函数
void to_json(json& j, const Tile& t) {
    j = json{
        {"type", t.m_type},
        {"wood_count", t.m_wood_count}
    };
}

// 定义 Tile 类的 from_json 函数
void from_json(const json& j, Tile& t) {
    j.at("type").get_to(t.m_type);
    j.at("wood_count").get_to(t.m_wood_count);
}

// 定义 Map 类的 to_json 函数
void to_json(json& j, const Map& m) {
    j = json{
        {"width", m.m_width},
        {"height", m.m_height},
        {"tiles", m.m_tiles}
    };
}

// 定义 Map 类的 from_json 函数
void from_json(const json& j, Map& m) {
    j.at("width").get_to(m.m_width);
    j.at("height").get_to(m.m_height);
    j.at("tiles").get_to(m.m_tiles);
}

void saveGame(const Game& game, const std::string& filename) {
    json j;
    j["players"] = game.m_map.m_player_entity;
    j["animals"] = game.m_map.m_animal_entity;
    j["map"] = game.m_map;
    // 其他需要保存的游戏状态信息

    std::ofstream file(filename);
    file << j.dump(4); // 格式化输出 JSON
}

void loadGame(Game& game, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open save file");
    }

    json j;
    file >> j;

    j.at("players").get_to(game.m_map.m_player_entity);
    j.at("animals").get_to(game.m_map.m_animal_entity);
    j.at("map").get_to(game.m_map);
    // 其他需要加载的游戏状态信息
}