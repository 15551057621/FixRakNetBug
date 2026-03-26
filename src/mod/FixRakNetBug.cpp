#include "mod/FixRakNetBug.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/memory/Hook.h"
#include "mc/deps/raknet/RakNet.h"
#include "mc/deps/raknet/SystemAddress.h"

namespace fix_raknet_bug {

// 全局 Logger 指针（在 load 时初始化）
static ll::io::Logger* g_logger = nullptr;

// 修复 MCPE-228407: 处理 ID_PONG_ADDRESS_INFO (0x86) 包的缓冲区溢出问题
LL_STATIC_HOOK(
    RakNetHook,
    HookPriority::High,
    &RakNet::ProcessOfflineNetworkPacket,
    bool,
    RakNet::SystemAddress  systemAddress,
    char const*            data,
    int                    length,
    RakNet::RakPeer*       rakPeer,
    RakNet::RakNetSocket2* rakNetSocket,
    bool*                  isOfflineMessage,
    uint64                 timeRead
) {
    // 检查是否是 0x86 包且长度不足（会导致崩溃）
    if (static_cast<unsigned char>(data[0]) == 0x86 && 
        length < sizeof(unsigned char) + sizeof(RakNet::SystemAddress)
    ) {
        if (g_logger) {
            g_logger->warn("收到不合规的数据包，已拦截...");  // ✅ 用全局指针
        }
        return false;  // 丢弃这个包，不处理
    }
    return origin(systemAddress, data, length, rakPeer, rakNetSocket, isOfflineMessage, timeRead);
}

// 注册 Hook
void registerHooks() {
    static ll::memory::HookRegistrar<RakNetHook> registrar;
}

// 插件实现
FixRakNetBug& FixRakNetBug::getInstance() {
    static FixRakNetBug instance;
    return instance;
}

bool FixRakNetBug::load() {
    g_logger = &getSelf().getLogger();  // ← 保存 Logger 指针
    return true;
}

bool FixRakNetBug::enable() {
    registerHooks();  // 激活 Hook 
    return true;
}

bool FixRakNetBug::disable() {
    RakNetHook::unhook(); // 卸载 Hook
    return true;
}

bool FixRakNetBug::unload() {
    g_logger = nullptr;
    RakNetHook::unhook(); // 卸载 Hook
    return true;
}

} // namespace fix_raknet_bug

LL_REGISTER_MOD(fix_raknet_bug::FixRakNetBug, fix_raknet_bug::FixRakNetBug::getInstance());