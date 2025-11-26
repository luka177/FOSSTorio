#pragma once
#include <cstdint>

#include <Animation/Animation4Way.h>

#include <sol/sol.hpp>

class WorkingVisualisations {
public:
    WorkingVisualisations(sol::table workingvisualisations);
protected:
    std::optional<Animation4Way> animation;
    std::optional<Animation4Way> idle_animation;
    bool always_draw_idle_animation = false;
    //GlobalRecipeTints default_recipe_tint;
    //GlobalRecipeTints recipe_not_set_tint;
    //vector<VisualState> states;
    std::vector<WorkingVisualisations> working_visualisations;
    //ShiftAnimationWaypoints shift_animation_waypoints;
    uint16_t shift_animation_waypoint_stop_duration = 0;
    uint16_t shift_animation_transition_duration = 0;
    // StatusColors status_colors;
};