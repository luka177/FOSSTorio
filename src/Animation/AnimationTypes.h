#pragma once
#include <vector>
#include <string>

#include <sol/sol.hpp>

using AnimationFrameSequence  = std::vector<uint>;

enum class AnimationRunMode {
    Forward,
    Backward,
    ForwardThenBackward
};

AnimationRunMode parseAnimationRunMode(const std::string& s);