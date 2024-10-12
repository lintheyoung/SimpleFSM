///////////////////////////////////////////////
#pragma once
#ifndef APPABILITY_H
#define APPABILITY_H

#include "Arduino.h"

///////////////////////////////////////////////

class AppAbility {
public:
    struct AppInfo_t {
        int appID;
        String name;
        bool isOpen;
    };

    AppAbility();
    virtual ~AppAbility();

    // 生命周期回调函数
    virtual void onOpen() {}
    virtual void onRunning() {}
    virtual void onClose() {}

    // 获取 App 名称
    String getName() const { return name; }

    // 获取 App ID
    int getAppID() const { return appID; }

    // 设置 App ID
    void setAppID(int id) { appID = id; }

protected:
    String name = "UnnamedApp";
    int appID = -1; // 由 Mooncake 分配
};

#endif
///////////////////////////////////////////////
