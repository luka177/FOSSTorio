#include <Visualisations/WorkingVisualisations.h>

WorkingVisualisations::WorkingVisualisations(sol::table workingvisualisations) {
    if(workingvisualisations["working_visualisations"].valid()) {
        std::cout << "[WorkingVisualisations] Valid nested WorkingVisualisations\n";
        sol::optional<sol::table> workingvisualisations_opt = workingvisualisations["working_visualisations"];
        sol::table workingvisualisations_table = workingvisualisations_opt.value();
        for (auto const& [key, value] : workingvisualisations_table.pairs()) {
            if (value.is<sol::table>()) {
                sol::table layer_table = value.as<sol::table>();
                working_visualisations.emplace_back(WorkingVisualisations(layer_table));
            } else {
                std::cout << "[WorkingVisualisations] Invalid workingvisualisations value (not a table)\n";
            }
        }
        std::cout << "[WorkingVisualisations] Found " << working_visualisations.size() << " layers\n";
    }
    if(workingvisualisations["animation"].valid()) {
        animation = Animation4Way(workingvisualisations["animation"].get<sol::table>());
    }
    if(workingvisualisations["idle_animation"].valid()) {
        idle_animation = Animation4Way(workingvisualisations["idle_animation"].get<sol::table>());
    }
};