/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef _RESOURCE_H
#define _RESOURCE_H

#include <string>
#include <iterator>
#include "System/creg/creg_cond.h"


struct SResourcePack {
	static constexpr int MAX_RESOURCES = 4;

	union {
		float res[MAX_RESOURCES];
		struct { float metal, energy; };
		struct { float res1, res2, res3, res4; };
	};

public:
	SResourcePack() {
		for (int i = 0; i < MAX_RESOURCES; ++i)
			res[i] = 0.0f;
	}
	SResourcePack(const float m, const float e) : metal(m), energy(e) {
		for (int i = 2; i < MAX_RESOURCES; ++i)
			res[i] = 0.0f;
	}
	CR_DECLARE_STRUCT(SResourcePack)

	bool empty() const {
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			if (res[i] != 0.0f) return false;
		}
		return true;
	}

	decltype(std::begin(res)) begin() { return std::begin(res); }
	decltype(std::end(res)) end() { return std::end(res); }

	float& operator[](const size_t i) {
		return res[i];
	}
	const float& operator[](const size_t i) const {
		return res[i];
	}

	bool operator<=(const SResourcePack& other) const {
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			if (res[i] > other.res[i]) return false;
		}
		return true;
	}
	bool operator>=(const SResourcePack& other) const {
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			if (res[i] < other.res[i]) return false;
		}
		return true;
	}

	SResourcePack operator+(const SResourcePack& other) const {
		SResourcePack out = *this;
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			out[i] += other.res[i];
		}
		return out;
	}

	SResourcePack operator-(const SResourcePack& other) const {
		SResourcePack out = *this;
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			out[i] -= other.res[i];
		}
		return out;
	}

	SResourcePack operator*(const SResourcePack& other) const {
		SResourcePack out = *this;
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			out[i] *= other.res[i];
		}
		return out;
	}

	SResourcePack operator/(const SResourcePack& other) const {
		SResourcePack out = *this;
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			out[i] /= other.res[i];
		}
		return out;
	}

	SResourcePack operator+(float value) const {
		SResourcePack out = *this;
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			out[i] += value;
		}
		return out;
	}

	SResourcePack operator*(float scale) const {
		SResourcePack out = *this;
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			out[i] *= scale;
		}
		return out;
	}

	SResourcePack operator-() const {
		SResourcePack out = *this;
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			out[i] = -out[i];
		}
		return out;
	}

	// void operator=(const SResourcePack& other) {
	// 	for (int i = 0; i < MAX_RESOURCES; ++i) {
	// 		res[i] = other.res[i];
	// 	}
	// 	//return *this;
	// }

	SResourcePack& operator+=(const SResourcePack& other) {
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			res[i] += other.res[i];
		}
		return *this;
	}
	SResourcePack& operator-=(const SResourcePack& other) {
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			res[i] -= other.res[i];
		}
		return *this;
	}

	SResourcePack& operator*=(float scale) {
		for (int i = 0; i < MAX_RESOURCES; ++i) {
			res[i] *= scale;
		}
		return *this;
	}
};


struct SResourceOrder {
	SResourcePack use;
	SResourcePack add;

	//! allow to split a resource when storage is empty/full?
	bool quantum;

	//! allow excessing when storages are full (only matters when quantum := true)
	bool overflow;

	//! handle resources separate, i.e. when metal storage is full still allow energy one to be filled?
	bool separate;

public:
	SResourceOrder() : quantum(false), overflow(false), separate(false) {}
	CR_DECLARE_STRUCT(SResourceOrder)
};


class CResourceDescription
{
public:
	CR_DECLARE_STRUCT(CResourceDescription)

	CResourceDescription();
	~CResourceDescription();

	/// The name of this resource, eg. "Energy" or "Metal"
	std::string name;
	std::string description;

	/// The optimum value for this resource, eg. 0 for "Waste" or FLT_MAX for "Metal"
	float optimum;

	/// The default extractor radius for the resource map, 0.0f if non applicable
	float extractorRadius;

	/// What value 255 in the resource map is worth
	float maxWorth;
};


#endif // _RESOURCE_H
