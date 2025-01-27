///////////////////////////////////////////////
#include "State.h"

///////////////////////////////////////////////

int State::_next_id = 0;

State::State() {
}

State::State(String name, CallbackFunction on_enter, CallbackFunction on_state, CallbackFunction on_exit, bool is_final) {
    setup(name, on_enter, on_state, on_exit, is_final);
}

void State::setup(String name, CallbackFunction on_enter, CallbackFunction on_state, CallbackFunction on_exit, bool is_final) {
    this->name = name;
    this->on_enter = on_enter;
    this->on_state = on_state;
    this->on_exit = on_exit;
    this->id = _next_id++;
    this->is_final = is_final;
}

String State::getName() const {
    return name;
}

bool State::isFinal() const {
    return is_final;
}

void State::setName(String name) {
    this->name = name;
}

void State::setOnEnterHandler(CallbackFunction f) {
    this->on_enter = f;
}

void State::setOnStateHandler(CallbackFunction f) {
    this->on_state = f;
}

void State::setOnExitHandler(CallbackFunction f) {
    this->on_exit = f;
}

int State::getID() const {
    return id;
}

void State::setAsFinal(bool final) {
    is_final = final;
}
///////////////////////////////////////////////
