#include "s_fluid.h"

float s_fluid::get_volume(float delta) {
	auto f = delta / 60 * up_speed * percent / 100;
	if (f + pressure > max_pressure)f = max_pressure - pressure;
	pressure += f;
	limit_max(pressure, max_pressure);
	return f;
}