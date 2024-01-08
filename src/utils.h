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

#pragma once

// Return the sign of the argument. -1 if negative, 1 if zero or positive.
#define SIGN(x) (((x) < 0) ? -1 : 1)

#define DEG2RAD_f(deg) ((deg) * (float) (M_PI / 180.0))
#define RAD2DEG_f(rad) ((rad) * (float) (180.0 / M_PI))

#define UNUSED(x) (void) (x)

/**
 * Rate-limits @p value towards @p target by an amount of maximum value of @p step.
 *
 * If the difference between @p value and @p target is greated than step, @p
 * value is increased or decreased (if @p target is greater or less than @p
 * value respectively) by @p step. Otherwise, @p value is set to @p target.
 *
 * @param value A pointer to value to rate-limit towards @p target.
 * @param target A target to rate-limit @p value towards.
 * @param step A maximum unit of change of @p value.
 */
void rate_limit(float *value, float target, float step);
