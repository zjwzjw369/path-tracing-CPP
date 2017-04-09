#pragma once

#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
using std::string;
using std::cout;
using std::endl;
#include <tchar.h>
#include "math_tools.hpp"
#include "vector.hpp"
using Color = Vector3;
struct Light {
	Vector3 Dir;
	Color Color;
}light;
#include "imageio.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "camera.hpp"
#include "Scene.hpp"
#include <time.h>