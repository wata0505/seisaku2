#include "Mathf.h"
#include<stdlib.h>
// 線形補完
float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}

// 指定の範囲のランダム値を計算する
float Mathf::RandomRange(float min, float max)
{
    float a = static_cast<float>(rand()) / RAND_MAX;
    return min + (max - min) * a;
}