// Copyright 2024 Lukas Hrazky
//
// This file is part of the Refloat VESC package.
//
// Refloat VESC package is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// Refloat VESC package is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

#include "utils.h"
#include "vesc_c_if.h"
#include <math.h>

uint32_t rnd(uint32_t seed) {
    return seed * 1664525u + 1013904223u;
}

void rate_limitf(float *value, float target, float step) {
    if (fabsf(target - *value) < step) {
        *value = target;
    } else if (target - *value > 0) {
        *value += step;
    } else {
        *value -= step;
    }
}

float clampf(float value, float min, float max) {
    const float m = value < min ? min : value;
    return m > max ? max : m;
}

float corrected_battery_percentage() {
    // we need to map the vesc percentage (which is a linear map from 3.0-4.2V) to a battery
    // percentage based on a cell boltage table such as
    float battVoltages[11] = {
        4.2, 4.054, 4.01, 3.908, 3.827, 3.74, 3.651, 3.571, 3.485, 3.38, 3.0
    };  // P42A

    float vescVoltage = VESC_IF->mc_get_input_voltage_filtered();
    int battCells = VESC_IF->get_cfg_int(CFG_PARAM_si_battery_cells);

    float cell_voltage = vescVoltage / battCells;

    // Ensure the voltage is within the measurable range
    if (cell_voltage >= battVoltages[0]) {
        return 1.0f;  // Fully charged
    }
    if (cell_voltage <= battVoltages[10]) {
        return 0.0f;  // Fully discharged
    }

    // Find the two closest values in the array
    for (int i = 0; i < 10; i++) {  // Adjusted loop to match 11 elements
        if (cell_voltage <= battVoltages[i] && cell_voltage > battVoltages[i + 1]) {
            // Interpolate percentage between `battVoltages[i]` and `battVoltages[i+1]`
            float range = battVoltages[i] - battVoltages[i + 1];
            float position = cell_voltage - battVoltages[i + 1];
            float percentage = ((10 - i) * 0.1) + (position / range) * 0.1;
            return percentage;
        }
    }

    // Default return in case of unexpected input
    return 0.0f;
}
