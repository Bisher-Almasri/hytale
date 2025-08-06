#pragma once

#include <cstdint>
#include <numeric>
#include <algorithm>
#include <random>

namespace siv {

template <typename Float>
class BasicPerlinNoise {
public:
    using value_type = Float;

private:
    std::int32_t p[512];

    static constexpr value_type Fade(value_type t) noexcept {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static constexpr value_type Lerp(value_type t, value_type a, value_type b) noexcept {
        return a + t * (b - a);
    }

    static constexpr value_type Grad(std::int32_t hash, value_type x, value_type y, value_type z) noexcept {
        const std::int32_t h = hash & 15;
        return ((h & 1) == 0 ? x : -x) + ((h & 2) == 0 ? y : -y) + ((h == 12 || h == 14) ? z : -z);
    }

public:
    explicit BasicPerlinNoise(std::uint32_t seed = std::default_random_engine::default_seed) {
        std::iota(std::begin(p), std::begin(p) + 256, 0);

        std::default_random_engine engine(seed);
        std::shuffle(std::begin(p), std::begin(p) + 256, engine);

        std::copy(std::begin(p), std::begin(p) + 256, std::begin(p) + 256);
    }

    value_type noise1(value_type x) const {
        return noise3(x, 0, 0);
    }

    value_type noise2(value_type x, value_type y) const {
        return noise3(x, y, 0);
    }

    value_type noise3(value_type x, value_type y, value_type z) const {
        const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
        const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
        const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        const value_type u = Fade(x);
        const value_type v = Fade(y);
        const value_type w = Fade(z);

        const std::int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
        const std::int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

        return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
                                        Grad(p[BA], x - 1, y, z)),
                                Lerp(u, Grad(p[AB], x, y - 1, z),
                                        Grad(p[BB], x - 1, y - 1, z))),
                       Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
                                        Grad(p[BA + 1], x - 1, y, z - 1)),
                                Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
                                        Grad(p[BB + 1], x - 1, y - 1, z - 1))));
    }

    value_type octaveNoise1(value_type x, std::int32_t octaves, value_type persistence = value_type(0.5)) const {
        value_type total = 0;
        value_type frequency = 1;
        value_type amplitude = 1;
        value_type maxValue = 0;

        for (std::int32_t i = 0; i < octaves; ++i) {
            value_type current = noise1(x * frequency) * amplitude;
            total += current;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }
        return total / maxValue;
    }

    value_type octaveNoise2(value_type x, value_type y, std::int32_t octaves, value_type persistence = value_type(0.5)) const {
        value_type total = 0;
        value_type frequency = 1;
        value_type amplitude = 1;
        value_type maxValue = 0;

        for (std::int32_t i = 0; i < octaves; ++i) {
            value_type current = noise2(x * frequency, y * frequency) * amplitude;
            total += current;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }
        return total / maxValue;
    }

    value_type octaveNoise3(value_type x, value_type y, value_type z, std::int32_t octaves, value_type persistence = value_type(0.5)) const {
        value_type total = 0;
        value_type frequency = 1;
        value_type amplitude = 1;
        value_type maxValue = 0;

        for (std::int32_t i = 0; i < octaves; ++i) {
            value_type current = noise3(x * frequency, y * frequency, z * frequency) * amplitude;
            total += current;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }
        return total / maxValue;
    }
};

using PerlinNoise = BasicPerlinNoise<double>;

}