#pragma once
#include <cstdint>
#include <vector>

#include <Animation/AnimationParameters.h>
#include <Animation/Stripe.h>

#include <sol/sol.hpp>

class RotatedAnimation : public AnimationParameters {
public:
    explicit RotatedAnimation(sol::table animation);

    int getLayersCount() const {
        return static_cast<int>(layers.size());
    }

    const std::vector<RotatedAnimation>& getLayers() const {
        return layers;
    }

    int getSpritesCount() const {
        return static_cast<int>(sprites.size());
    }

    const std::vector<TextureId>& getSprites() const {
        return sprites;
    }

protected:
    std::vector<RotatedAnimation> layers;
    uint32_t direction_count = 1;
    //TBD: filenames
    uint32_t lines_per_file = 0;
    uint32_t slice = 0;
    uint32_t still_frame = 0;
    bool counterclockwise = false;
    //RealOrientation middle_orientation = 0.5;
    bool orientation_range = 1;
    bool apply_projection = true;
    std::vector<Stripe> stripes;
    std::vector<TextureId> sprites;
   // std::vector<TextureId> textures;
};
