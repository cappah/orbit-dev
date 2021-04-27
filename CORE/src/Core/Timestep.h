#pragma once

/// <summary>
/// Just a basic time definition
/// </summary>

namespace Orbit {
	struct Timestep {
		Timestep() = default;
		Timestep(const Timestep&) = default;
		Timestep(float time) : _time(time) { }
		operator float() const { return _time; }

		float Seconds() const { return _time; }
		float MilliSeconds() const { return _time * 1000.0f; }

	private:
		float _time = 0.0f;
	};
}