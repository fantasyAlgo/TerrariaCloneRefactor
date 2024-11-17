#pragma once
#include <cmath>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <random>
#include <functional>

class ValueNoise1D { 
    public: 
        ValueNoise1D(unsigned seed = 2016) { 
            srand48(seed); 
            for (unsigned i = 0; i < kMaxVertices; ++i) { 
                r[i] = drand48(); 
            } 
        } 
        float lerp(const float &a, const float &b, const float &t) { 
            return a + t * (b - a); 
        }
        float eval(const float &x) { 
            // Floor
            int xi = (int)x - (x < 0 && x != (int)x); 
            float t = x - xi; 
            t = t * t * (3.0f - 2.0f * t);
            // Modulo using &
            int xMin = xi & kMaxVerticesMask; 
            int xMax = (xMin + 1) & kMaxVerticesMask; 
    
            return lerp(r[xMin], r[xMax], t); 
        } 
        static const unsigned kMaxVertices = 256; 
        static const unsigned kMaxVerticesMask = kMaxVertices - 1; 
        float r[ kMaxVertices ]; 
}; 


class Vec3f {
    public:
        float x, y, z;
        Vec3f(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
        Vec3f operator+(const Vec3f &v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }
        Vec3f operator-(const Vec3f &v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
        Vec3f operator*(float f) const { return Vec3f(x * f, y * f, z * f); }
        Vec3f operator/(float f) const { return Vec3f(x / f, y / f, z / f); }
        void divideBy(float f){
            x /= f;
            y /= f;
            z /= f;
        }

        float dot(const Vec3f &v) const { return x * v.x + y * v.y + z * v.z; }
        float length2() const { return x * x + y * y + z * z; }
        Vec3f &normalize() { return *this = *this * (1 / sqrtf(length2())); }
};

class PerlinNoise { 
public:
    static const unsigned tableSize = 256; 
    static const unsigned tableSizeMask = tableSize - 1; 
    Vec3f gradients[tableSize]; 
    unsigned permutationTable[tableSize * 2]; 
 
    PerlinNoise() 
    { 
        unsigned seed = 2016; 
        std::mt19937 generator(seed); 
        std::uniform_real_distribution<float> distribution; 
        auto dice = std::bind(distribution, generator); 
        float gradientLen2; 
        for (unsigned i = 0; i < tableSize; ++i) { 
            do { 
                gradients[i] = Vec3f(2 * dice() - 1, 2 * dice() - 1, 2 * dice() - 1); 
                gradientLen2 = gradients[i].length2(); 
            } while (gradientLen2 > 1); 
            gradients[i].divideBy(sqrtf(gradientLen2));
            permutationTable[i] = i; 
        } 
 
        std::uniform_int_distribution<unsigned> distributionInt; 
        auto diceInt = std::bind(distributionInt, generator); 
        // create permutation table
        for (unsigned i = 0; i < tableSize; ++i) 
            std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]); 
        // extend the permutation table in the index range [256:512]
        for (unsigned i = 0; i < tableSize; ++i) { 
            permutationTable[tableSize + i] = permutationTable[i]; 
        } 
    } 
    virtual ~PerlinNoise() {} 
 
    /* inline */ 
    int hash(const int &x, const int &y, const int &z) const 
    { return permutationTable[permutationTable[permutationTable[x] + y] + z]; } 
 
    float smoothstep(float t) const { return t * t * (3 - 2 * t); }
    float lerp(float a, float b, float t) const { return a + t * (b - a); }
    float dotProduct(const Vec3f &a, const Vec3f &b) const { return a.x * b.x + a.y * b.y + a.z * b.z; }
    float eval(const Vec3f &p) const { 
        int xi0 = ((int)std::floor(p.x)) & tableSizeMask; 
        int yi0 = ((int)std::floor(p.y)) & tableSizeMask; 
        int zi0 = ((int)std::floor(p.z)) & tableSizeMask; 
 
        int xi1 = (xi0 + 1) & tableSizeMask; 
        int yi1 = (yi0 + 1) & tableSizeMask; 
        int zi1 = (zi0 + 1) & tableSizeMask; 
 
        float tx = p.x - ((int)std::floor(p.x)); 
        float ty = p.y - ((int)std::floor(p.y)); 
        float tz = p.z - ((int)std::floor(p.z)); 
 
        float u = smoothstep(tx); 
        float v = smoothstep(ty); 
        float w = smoothstep(tz); 
 
        // gradients at the corner of the cell
        const Vec3f &c000 = gradients[hash(xi0, yi0, zi0)]; 
        const Vec3f &c100 = gradients[hash(xi1, yi0, zi0)]; 
        const Vec3f &c010 = gradients[hash(xi0, yi1, zi0)]; 
        const Vec3f &c110 = gradients[hash(xi1, yi1, zi0)]; 
 
        const Vec3f &c001 = gradients[hash(xi0, yi0, zi1)]; 
        const Vec3f &c101 = gradients[hash(xi1, yi0, zi1)]; 
        const Vec3f &c011 = gradients[hash(xi0, yi1, zi1)]; 
        const Vec3f &c111 = gradients[hash(xi1, yi1, zi1)]; 
 
        // generate vectors going from the grid points to p
        float x0 = tx, x1 = tx - 1; 
        float y0 = ty, y1 = ty - 1; 
        float z0 = tz, z1 = tz - 1; 
 
        Vec3f p000 = Vec3f(x0, y0, z0); 
        Vec3f p100 = Vec3f(x1, y0, z0); 
        Vec3f p010 = Vec3f(x0, y1, z0); 
        Vec3f p110 = Vec3f(x1, y1, z0); 
 
        Vec3f p001 = Vec3f(x0, y0, z1); 
        Vec3f p101 = Vec3f(x1, y0, z1); 
        Vec3f p011 = Vec3f(x0, y1, z1); 
        Vec3f p111 = Vec3f(x1, y1, z1); 
 
        // linear interpolation
        float a = lerp(dotProduct(c000, p000), dotProduct(c100, p100), u); 
        float b = lerp(dotProduct(c010, p010), dotProduct(c110, p110), u); 
        float c = lerp(dotProduct(c001, p001), dotProduct(c101, p101), u); 
        float d = lerp(dotProduct(c011, p011), dotProduct(c111, p111), u); 
 
        float e = lerp(a, b, v); 
        float f = lerp(c, d, v); 
 
        return lerp(e, f, w);  //g 
    } 
};
