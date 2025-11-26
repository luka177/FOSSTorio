#pragma once
#include <cmath>
#include <vector>

#include <Sprite/SpriteParameters.h>
#include <Animation/AnimationTypes.h>

#include <sol/sol.hpp>

class AnimationParameters : public SpriteParmeters {
public:
    AnimationParameters(sol::table animationparameters);
protected:
    AnimationRunMode run_mode;
    uint32_t frame_count = 1;
    uint32_t line_length = 0;
    float animation_speed = 1;
    float max_advance = MAXFLOAT;
    uint8_t repeat_count = 1;
    uint8_t dice_x = 1;
    uint8_t dice_y = 1;
 //   AnimationFrameSequence frame_sequence;
};