#include <Animation/Stripe.h>

Stripe::Stripe(sol::table stripe) {
    if (stripe["width_in_frames"].valid()) {
        width_in_frames = stripe["width_in_frames"].get_or(0);
    } else {
        throw std::runtime_error("[Stripe] width_in_frames isnt optional, but is missing!\n");
    }
    if (stripe["height_in_frames"].valid()) {
        height_in_frames = stripe["height_in_frames"].get_or(0);
    } else {
        throw std::runtime_error("[Stripe] height_in_frames isnt optional, but is missing!\n");
    }
    if (stripe["filename"]) {
      //  stripe_texture = requestTexture(stripe["filename"].get_or(std::string{}));
    } else {
        throw std::runtime_error("[Stripe] filename isnt optional, but is missing!\n");
    }
    x = stripe["x"].get_or(0);
    y = stripe["y"].get_or(0);
}