///////////////////////////////////////////////
#pragma once
#ifndef MYAPP_H
#define MYAPP_H

#include "AppAbility.h"
#include "SimpleFSM.h"

class MyApp : public AppAbility {
public:
    MyApp();
    ~MyApp();

    // 重写生命周期回调函数
    void onOpen() override;
    void onRunning() override;
    void onClose() override;

private:
    SimpleFSM fsm;

    // 定义状态
    State* stateIdle;
    State* stateActive;

    // 定义转换
    Transition transStart;
    Transition transStop;

    // 事件 ID
    enum Events {
        EVENT_START = 1,
        EVENT_STOP = 2
    };

    // 定义状态转换
    void setupFSM();
};

#endif
///////////////////////////////////////////////
