#include "model/SimpleMap.hpp"
#include "model/SimpleCube.hpp"
#include "model/detail/AIUtils.hpp"
#include "model/AICommand.hpp"
#include "ctrl/AIPlayer.hpp"
#include "utils/Logger.hpp"

#include <cstdio>

using namespace psc;
using namespace ctrl;
using namespace model;
using ai_detail::AIUtils;
using utils::Logger;

extern "C" {
#include "script/ai/bindings.h"
}

void AIPlayer_push_command(AIPlayer* p, LuaAICommand* c) { //not shared_ptr!
    pAICommand cmd = AICommand::create(c->type);
    switch( c->type ) {
        case LuaAICommand::AI_SHOOT:
        case LuaAICommand::AI_SHOOT_OTHER:
        case LuaAICommand::AI_USE_ABILITY:
            cmd->delay(c->delay).weight(1).normal_shot(c->x, c->y);
            break;
        case LuaAICommand::AI_HASTE:
            cmd->press_trig2();
            break;
        default: break;
    }
    p->pushCommand(cmd);
}

pSimpleMap* AIPlayer_get_ally_map(AIPlayer* p, unsigned int index) { //not shared_ptr!
    pSimpleMap c = p->getAllyMap(index);
    pSimpleMap* ret = new pSimpleMap;
    *ret = c;
    return ret;
}

pSimpleMap* AIPlayer_get_enemy_map(AIPlayer* p, unsigned int index) { //not shared_ptr!
    pSimpleMap c = p->getEnemyMap(index);
    pSimpleMap* ret = new pSimpleMap;
    *ret = c;
    return ret;
}

int AIPlayer_cmdqueue_size(AIPlayer* p) { //not shared_ptr!
    return p->cmdQueueSize();
}

double AIPlayer_get_heat(AIPlayer* p) {   //not shared_ptr!
    return p->heat();
}

int AIPlayer_ability_kind(AIPlayer* p) { //not shared_ptr!
    return p->ability_kind();
}

unsigned int AIPlayer_ability_left(AIPlayer* p) { //not shared_ptr!
    return p->ability_left();
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
    //Logger::i().buf("Map: ").buf(*p).buf(" __gc called.").endl();
    delete p;
}

bool SimpleMap_cube_exist_at(pSimpleMap* p, int x, int y) {
    return ( AIUtils::lookup(*p, x, y) ) ? true : false;
}

bool SimpleMap_dropping_locked(pSimpleMap* p) {
    return (*p)->dropping_locked();
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

pSimpleCube* SimpleMap_get_firepoint_cube(pSimpleMap* p, int lower_bound, int upper_bound, int em_level) {
    pSimpleCube c =
        AIUtils::find_keycube_for_highest_chain_power(*p, lower_bound, upper_bound, em_level);
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

bool SimpleMap_still_chaining(pSimpleMap* p) {
    return AIUtils::still_chaining(*p);
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
    //Logger::i().buf("Cube: ").buf(*p).buf(" __gc called, use_count: ").buf(p->use_count()).endl();
    delete p;
}

void SimpleCubeList__gc(pSimpleCube** list, int size) {
    //Logger::i().buf("CubeList: ").buf(*list).buf(" __gc called.").endl();
    for( int i = 0; i < size; ++i ) {
        //Logger::i().buf("CubeList cleaning cube: ").buf(*(list[i])).buf(", use_count: ").buf(list[i]->use_count()).endl();
        delete list[i];
    }
    delete[] list;
}

