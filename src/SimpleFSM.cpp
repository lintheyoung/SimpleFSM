///////////////////////////////////////////////
#include "SimpleFSM.h"

///////////////////////////////////////////////

SimpleFSM::SimpleFSM() {
}

SimpleFSM::SimpleFSM(State* initial_state) {
    setInitialState(initial_state);
}

SimpleFSM::~SimpleFSM() {
    // 使用 std::vector，无需手动释放内存
}

void SimpleFSM::reset() {
    is_initialized = false;
    is_finished = false;
    last_run = 0;
    setInitialState(inital_state);
}

void SimpleFSM::setInitialState(State* state) {
    inital_state = state;
}

bool SimpleFSM::trigger(int event_id) {
    if (!is_initialized) _initFSM();
    // 查找当前状态和给定事件的转换
    for (size_t i = 0; i < transitions.size(); i++) {
        if (transitions[i].from == current_state && transitions[i].getEventID() == event_id) {
            return _transitionTo(&transitions[i]);
        }
    }
    return false;
}

State* SimpleFSM::getPreviousState() const {
    return prev_state;
}

State* SimpleFSM::getState() const {
    return current_state;
}

bool SimpleFSM::isInState(State* t) const {
    return t == current_state;
}

void SimpleFSM::setTransitionHandler(CallbackFunction f) {
    on_transition_cb = f;
}

void SimpleFSM::add(Transition t[], int size) {
    for (int i = 0; i < size; i++) {
        transitions.push_back(t[i]);
        _addDotTransition(transitions.back());
    }
    num_standard += size;
}

void SimpleFSM::add(TimedTransition t[], int size) {
    for (int i = 0; i < size; i++) {
        timed.push_back(t[i]);
        _addDotTransition(timed.back());
    }
    num_timed += size;
}

void SimpleFSM::setFinishedHandler(CallbackFunction f) {
    finished_cb = f;
}

unsigned long SimpleFSM::lastTransitioned() const {
    return (last_transition == 0) ? 0 : (millis() - last_transition);
}

bool SimpleFSM::isFinished() const {
    return is_finished;
}

void SimpleFSM::run(int interval, CallbackFunction tick_cb) {
    unsigned long now = millis();
    // 检查是否初始化
    if (!is_initialized) _initFSM();
    // 检查当前状态是否有效
    if (current_state == NULL) return;
    // 检查时间间隔
    if (now < last_run + interval) return;
    // 检查是否完成
    if (is_finished) return;
    last_run = now;

    // 处理定时转换
    for (size_t i = 0; i < timed.size(); i++) {
        if (timed[i].from != current_state) continue;

        // 开始转换计时
        if (timed[i].start == 0) {
            timed[i].start = now;
            continue;
        }
        // 检查是否达到时间间隔
        if (now - timed[i].start >= timed[i].getInterval()) {
            if (_transitionTo(&timed[i])) {
                timed[i].start = 0;
                return;
            }
        }
    }
    // 触发 on_state 事件
    if (current_state->on_state != NULL) current_state->on_state();
    // 触发常规 tick 事件
    if (tick_cb != NULL) tick_cb();
}

bool SimpleFSM::_initFSM() {
    if (is_initialized) return false;
    is_initialized = true;
    if (inital_state == NULL) return false;
    return _changeToState(inital_state, millis());
}

bool SimpleFSM::_changeToState(State* s, unsigned long now) {
    if (s == NULL) return false;
    // 设置新状态
    prev_state = current_state;
    current_state = s;
    if (s->on_enter != NULL) s->on_enter();
    // 记录时间
    last_run = now;
    last_transition = now;
    // 检查是否为最终状态
    if (s->is_final && finished_cb != NULL) finished_cb();
    if (s->is_final) is_finished = true;
    return true;
}

bool SimpleFSM::_transitionTo(AbstractTransition* transition) {
    // 检查转换参数是否为空
    if (transition->to == NULL) return false;
    // 检查守卫条件
    if (transition->guard_cb != NULL && !transition->guard_cb()) return false;
    // 触发事件
    if (transition->from->on_exit != NULL) transition->from->on_exit();      
    if (transition->on_run_cb != NULL) transition->on_run_cb();
    if (on_transition_cb != NULL) on_transition_cb();
    return _changeToState(transition->to, millis());
}

String SimpleFSM::getDotDefinition() {
    return "digraph G {\n" + _dot_header() + dot_definition + _dot_active_node() + _dot_inital_state() + "}\n";
}

String SimpleFSM::_dot_transition(String from, String to, String label, String param) {
    return "\t\"" + from + "\" -> \"" + to + "\"" +  " [label=\"" + label + " (" + param + ")\"];\n";
}

String SimpleFSM::_dot_inital_state() {
    return inital_state ? "\t\"" + inital_state->getName() + "\" [style=filled fontcolor=white fillcolor=black];\n\n" : "";
}

String SimpleFSM::_dot_active_node() {
    return current_state ? "\t\"" + current_state->getName() + "\" [style=filled fontcolor=white];\n" : "";
}

String SimpleFSM::_dot_header() {
    return "\trankdir=LR; pad=0.5\n\tnode [shape=circle fixedsize=true width=1.5];\n";
}

void SimpleFSM::_addDotTransition(Transition& t) {
    dot_definition = dot_definition + _dot_transition(t.from->getName(), t.to->getName(), t.getName(), "ID=" + String(t.getEventID()));
}

void SimpleFSM::_addDotTransition(TimedTransition& t) {
    dot_definition = dot_definition + _dot_transition(t.from->getName(), t.to->getName(), t.getName(), String(t.getInterval()) + "ms");
}
///////////////////////////////////////////////
