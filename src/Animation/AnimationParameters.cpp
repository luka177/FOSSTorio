#include <bits/stdc++.h>
#include <Animation/AnimationParameters.h>

AnimationParameters::AnimationParameters(sol::table animationparameters)
    :SpriteParmeters(animationparameters) {
    run_mode = parseAnimationRunMode(animationparameters["run_mode"].get_or(std::string{"forward"}));
    frame_count = animationparameters["frame_count"].get_or(1);
    line_length = animationparameters["line_length"].get_or(0);
    animation_speed = animationparameters["animation_speed"].get_or(1);
    if(animation_speed <=0) {
        std::cerr << "animation_speed should never be <=0\n";
        animation_speed = 1;
    }
    max_advance = animationparameters["max_advance"].get_or(MAXFLOAT);
    repeat_count = animationparameters["repeat_count"].get_or(1);
    dice_x = dice_y = animationparameters["dice"].get_or(0);
    dice_x = animationparameters["dice_x"].get_or(dice_x);
    dice_y = animationparameters["dice_y"].get_or(dice_y);
};