#pragma once

#include "ll/api/mod/NativeMod.h"

#pragma once

#include "ll/api/mod/NativeMod.h"

namespace fix_raknet_bug {

class FixRakNetBug {
public:
    static FixRakNetBug& getInstance();

    FixRakNetBug() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace fix_raknet_bug 