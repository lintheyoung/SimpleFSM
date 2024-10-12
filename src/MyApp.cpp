///////////////////////////////////////////////
#include "MyApp.h"
#include "Arduino.h"

///////////////////////////////////////////////

MyApp::MyApp() {
    name = "MyApp";

    // 定义状态
    stateIdle = new State("Idle", 
                          [](){ Serial.println("[MyApp] Enter Idle"); }, 
                          [](){ Serial.println("[MyApp] Running Idle"); }, 
                          [](){ Serial.println("[MyApp] Exit Idle"); }, 
                          false);

    stateActive = new State("Active", 
                            [](){ Serial.println("[MyApp] Enter Active"); }, 
                            [](){ Serial.println("[MyApp] Running Active"); }, 
                            [](){ Serial.println("[MyApp] Exit Active"); }, 
                            true);

    // 定义转换
    transStart.setup(stateIdle, stateActive, EVENT_START, 
                     [](){ Serial.println("[MyApp] Transition Start"); }, 
                     "StartTransition", NULL);

    transStop.setup(stateActive, stateIdle, EVENT_STOP, 
                    [](){ Serial.println("[MyApp] Transition Stop"); }, 
                    "StopTransition", NULL);

    // 添加转换到 FSM
    Transition transitions_arr[] = { transStart, transStop };
    fsm.add(transitions_arr, 2);

    // 设置初始状态
    fsm.setInitialState(stateIdle);
}

MyApp::~MyApp() {
    delete stateIdle;
    delete stateActive;
}

void MyApp::onOpen() {
    Serial.println("[MyApp] on open");
    fsm.reset();
}

void MyApp::onRunning() {
    // 仅在 Idle 状态时触发 EVENT_START
    if (fsm.getState() == stateIdle) {
        Serial.println("[MyApp] on running - triggering EVENT_START");
        fsm.trigger(MyApp::EVENT_START);
    }
    else if (fsm.getState() == stateActive) {
        Serial.println("[MyApp] on running - App is in final state");
    }
}

void MyApp::onClose() {
    Serial.println("[MyApp] on close");
    // 仅在 Active 状态时触发 EVENT_STOP
    if (fsm.getState() == stateActive) {
        Serial.println("[MyApp] on close - triggering EVENT_STOP");
        fsm.trigger(MyApp::EVENT_STOP);
    }
}
///////////////////////////////////////////////
