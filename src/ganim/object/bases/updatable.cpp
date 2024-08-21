#include "updatable.hpp"

using namespace ganim;

int Updatable::add_updater_void(std::move_only_function<void()> updater, bool persistent)
{
    return add_updater_bool([f = std::move(updater)] mutable {
        f();
        return true;
    }, persistent);
}

int Updatable::add_updater_bool(std::move_only_function<bool()> updater, bool persistent)
{
    auto handle = 0;
    if (!M_updaters.empty()) {
        handle = M_updaters.rbegin()->first + 1;
    }
    M_updaters.emplace(handle, std::pair{std::move(updater), persistent});
    return handle;
}

void Updatable::remove_updater(int updater_handle)
{
    M_updaters.erase(updater_handle);
}

void Updatable::clear_updaters()
{
    for (auto it = M_updaters.begin(); it != M_updaters.end();) {
        if (!it->second.second) it = M_updaters.erase(it);
        else ++it;
    }
}

void Updatable::update()
{
    for (auto it = M_updaters.begin(); it != M_updaters.end();) {
        if (!it->second.first()) {
            it = M_updaters.erase(it);
        }
        else ++it;
    }
}
