#include <unordered_map>
#include <iostream>

#include <Animation/AnimationTypes.h>

#include <sol/sol.hpp>

static const std::unordered_map<std::string, AnimationRunMode> AnimationRunModeMap = {
    { "forward",               AnimationRunMode::Forward },
    { "backward",              AnimationRunMode::Backward },
    { "forward-then-backward", AnimationRunMode::ForwardThenBackward }
};

AnimationRunMode parseAnimationRunMode(const std::string& s) {
    auto it = AnimationRunModeMap.find(s);
    if (it != AnimationRunModeMap.end())
        return it->second;

    return AnimationRunMode::Forward;
}