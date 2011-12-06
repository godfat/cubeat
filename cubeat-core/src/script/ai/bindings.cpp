#include "model/SimpleMap.hpp"
#include "model/SimpleCube.hpp"
#include "model/detail/AIUtils.hpp"
#include "model/AIBrain.hpp"
#include "model/AICommand.hpp"
#include "utils/Logger.hpp"

using namespace psc;
using namespace model;
using ai_detail::AIUtils;
using utils::Logger;

extern "C" {
#include "script/ai/bindings.i"
}

void AIBrain_push_command(AIBrain* p, LuaAICommand* c) { //not shared_ptr!
    if( c->type == LuaAICommand::PSC_AI_SHOOT ) {
        pAICommand cmd = AICommand::create();
        cmd->delay(c->delay).weight(1).normal_shot(c->x, c->y);
        p->pushCommand(cmd);
    }
    else if( c->type == LuaAICommand::PSC_AI_HASTE ) {
        pAICommand cmd = AICommand::create();
        cmd->press_trig2();
        p->pushCommand(cmd);
    }
}

pSimpleMap* AIBrain_get_ally_map(AIBrain* p, unsigned int index) { //not shared_ptr!
    pSimpleMap c = p->getAllyMap(index);
    pSimpleMap* ret = new pSimpleMap;
    *ret = c;
    return ret;
}

pSimpleMap* AIBrain_get_enemy_map(AIBrain* p, unsigned int index) { //not shared_ptr!
    pSimpleMap c = p->getEnemyMap(index);
    pSimpleMap* ret = new pSimpleMap;
    *ret = c;
    return ret;
}

void SimpleMap_print_data_for_debug(pSimpleMap* p) {
    (*p)->print_data_for_debug();
}

int  SimpleMap_warning_level(pSimpleMap* p) {
    return (*p)->warning_level();
}

int  SimpleMap_garbage_left(pSimpleMap* p) {
    return (*p)->garbage_left();
}

int  SimpleMap_width(pSimpleMap* p) {
    return (*p)->ms()->width();
}

int  SimpleMap_height(pSimpleMap* p) {
    return (*p)->ms()->height();
}

void SimpleMap__gc(pSimpleMap* p) {
    Logger::i().buf("Map: ").buf(*p).buf(" __gc called.").endl();
    delete p;
}

bool SimpleMap_cube_exist_at(pSimpleMap* p, int x, int y) {
    return ( AIUtils::lookup(*p, x, y) ) ? true : false;
}

pSimpleCube* SimpleMap_get_cube(pSimpleMap* p, int x, int y) {
    pSimpleCube c = AIUtils::lookup(*p, x, y);
    pSimpleCube* ret = new pSimpleCube;
    *ret = c;
    return ret;
}

pSimpleCube* SimpleMap_get_grounded_cube(pSimpleMap* p, int x, int y) {
    pSimpleCube c = AIUtils::lookup_for_grounded(*p, x, y);
    pSimpleCube* ret = new pSimpleCube;
    *ret = c;
    return ret;
}

pSimpleCube* SimpleMap_get_firepoint_cube(pSimpleMap* p, int lower_bound, int upper_bound) {
    pSimpleCube c =
        AIUtils::find_keycube_for_highest_chain_power(*p, lower_bound, upper_bound);
    pSimpleCube* ret = new pSimpleCube;
    *ret = c;
    return ret;
}

pSimpleCube** SimpleMap_get_garbages(pSimpleMap* p, unsigned int* size_out) {
    return AIUtils::find_garbages(*p, size_out);
}

pSimpleCube** SimpleMap_get_brokens(pSimpleMap* p, unsigned int* size_out) {
    return AIUtils::find_brokens(*p, size_out);
}

int SimpleMap_grounded_cube_count(pSimpleMap* p) {
    return AIUtils::grounded_cube_count(*p);
}

bool SimpleCube_exist(pSimpleCube* p) {
    return (*p) ? true : false;
}

bool SimpleCube_is_garbage(pSimpleCube* p) {
    return (*p)->is_garbage();
}

bool SimpleCube_is_broken(pSimpleCube* p) {
    return (*p)->is_broken();
}

int  SimpleCube_x(pSimpleCube* p) {
    return (*p)->x();
}

int  SimpleCube_y(pSimpleCube* p) {
    return (*p)->y();
}

void SimpleCube__gc(pSimpleCube* p) {
    Logger::i().buf("Cube: ").buf(*p).buf(" __gc called, use_count: ").buf(p->use_count()).endl();
    delete p;
}

void SimpleCubeList__gc(pSimpleCube** list, int size) {
    Logger::i().buf("CubeList: ").buf(*list).buf(" __gc called.").endl();
    for( int i = 0; i < size; ++i ) {
        Logger::i().buf("CubeList cleaning cube: ").buf(*(list[i])).buf(", use_count: ").buf(list[i]->use_count()).endl();
        delete list[i];
    }
    delete[] list;
}

