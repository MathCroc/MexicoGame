#pragma once

#include "Player.hpp"

namespace mexico {
class DummyPlayer : public Player
{
public:
    DummyPlayer(const std::string& nameIn) : name(nameIn) {}

    virtual void prepareForRound() override {}
    virtual void observe(const Roll&) override {}
    virtual std::string getName() const override { return name; }

protected:
    virtual bool stay(const Roll& roll) override { return false; }

private:
    std::string name;
};
} // namespace mexico
