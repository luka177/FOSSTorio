#pragma once
#include <cstdint>
#include <vector>

#include <Animation/AnimationParameters.h>
#include <Animation/Stripe.h>

#include <sol/sol.hpp>

class Animation : public AnimationParameters {
public:
    Animation(sol::table animation);
protected:
    std::vector<Animation> layers;
    std::vector<Stripe> stripes;
    std::vector<TextureId> textures;
    uint32_t slice;
    uint32_t lines_per_file;
};
