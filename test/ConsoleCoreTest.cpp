//
// Created by W on 10/9/20.
//
#include "catch.hpp"
#include "willow/console/ConsoleCore.hpp"
using namespace wlo;
int main(){
    ConsoleCore core;
    core.moveCursor(ConsoleCore::CursorMovement::RIGHT);
    assert(core.getCursorPos().first==0 && core.getCursorPos().second==0);
    core.moveCursor(ConsoleCore::CursorMovement::LEFT);
    assert(core.getCursorPos().first==0 && core.getCursorPos().second==0);
    core.moveCursor(ConsoleCore::CursorMovement::UP);
    assert(core.getCursorPos().first==0 && core.getCursorPos().second==0);
    core.moveCursor(ConsoleCore::CursorMovement::DOWN);
    assert(core.getCursorPos().first==0 && core.getCursorPos().second==0);

    assert(core.getVertexBuffer().size()==8);
    assert(core.getIndexBuffer().size()==12);
    core.deleteChar();
    assert(core.getVertexBuffer().size()==4);

    assert(core.getIndexBuffer().size()==6);
    assert(core.flushInputBuffer()=="");


}