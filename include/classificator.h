#pragma once
#include "common.h"

double distance(sampleType point1, sampleType point2)
{
    double dx(point2(0) - point1(0)), dy(point2(1) - point1(1));
    return dx * dx + dy * dy;
}