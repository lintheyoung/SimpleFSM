///////////////////////////////////////////////
#pragma once
#ifndef MOONCAKE_H
#define MOONCAKE_H

#include "Arduino.h"
#include <memory>
#include <vector>
#include <map>
#include "AppAbility.h"

///////////////////////////////////////////////

class Mooncake {
public:
    Mooncake();
    ~Mooncake();

    // 安装 App，返回 App ID
    int installApp(std::unique_ptr<AppAbility> appAbility);

    // 卸载指定 ID 的 App
    bool uninstallApp(int appID);

    // 卸载所有 App
    void uninstallAllApps();

    // 打开指定 ID 的 App
    bool openApp(int appID);

    // 关闭指定 ID 的 App
    bool closeApp(int appID);

    // 获取当前 App 数量
    std::size_t getAppNum() const;

    // 获取指定 ID 的 App 信息
    AppAbility::AppInfo_t getAppInfo(int appID) const;

    // 获取所有 App 的 App 信息
    std::vector<AppAbility::AppInfo_t> getAllAppInfo() const;

    // 更新 Mooncake，刷新 App 及 Extension 状态，触发生命周期回调
    void update();

private:
    struct AppEntry {
        int appID;
        std::unique_ptr<AppAbility> app;
        bool isOpen;
    };

    std::map<int, AppEntry> apps;
    int nextAppID = 1; // 从 1 开始分配 App ID
};

#endif
///////////////////////////////////////////////
