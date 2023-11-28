/* -------------------------------------------------------------------------- *
 * OpenSim Moco: MocoInitialActivationGoal.cpp                                *
 * -------------------------------------------------------------------------- *
 * Copyright (c) 2019 Stanford University and the Authors                     *
 *                                                                            *
 * Author(s): Christopher Dembia                                              *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0          *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "MocoInitialActivationGoal.h"

#include <OpenSim/Simulation/SimulationUtilities.h>

using namespace OpenSim;

void MocoInitialActivationGoal::initializeOnModelImpl(
        const Model& model) const {

    // Get a map of all the state indices in the system.
    auto allSysYIndices = createSystemYIndexMap(model);

    // Get a map of all the control indices in the system. Skip muscles that
    // are already controlled by a user-defined controller.
    auto systemControlIndexMap = createSystemControlIndexMap(model, true, true);

    for (const auto& muscle : model.getComponentList<Muscle>()) {
        const std::string path = muscle.getAbsolutePathString();
        const bool muscleIsNotControlled = systemControlIndexMap.find(path) !=
                                           systemControlIndexMap.end();
        if (!muscle.get_ignore_activation_dynamics() && muscleIsNotControlled) {
            int excitationIndex = systemControlIndexMap[path];
            int activationIndex = allSysYIndices[path + "/activation"];
            m_indices.emplace_back(excitationIndex, activationIndex);
        }
    }

    setRequirements(0, (int)m_indices.size(), SimTK::Stage::Time);
}

void MocoInitialActivationGoal::calcGoalImpl(
        const GoalInput& input, SimTK::Vector& goal) const {
    const auto& controls = input.initial_controls;
    const auto& states = input.initial_state.getY();
    int i = 0;
    if (!getModeIsCost()) {
        for (const auto& indices : m_indices) {
            goal[i] = controls[indices.first] - states[indices.second];
            ++i;
        }
    } else {
        for (const auto& indices : m_indices) {
            goal[i] = SimTK::square(controls[indices.first] -
                    states[indices.second]);
            ++i;
        }
    }
}
