#include "head.hpp"
Camera camera(Vector3(0.0, 5.0, 15.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0), 90.0);
Scene scene;

const int width = 1024;
const int height = 1024;
Sphere sphere1(Vector3(-10.0, 10.0, -10.0), 10.0);
Sphere sphere2(Vector3(10.0, 10.0, -10.0), 10.0);
void renderDepth(Vector3 *pixels,double maxDepth) {
	int index = 0;
	for (int y = 0; y < height; ++y) {
		double sy = 1 - y / static_cast<double>(height);
		for (int x = 0; x < width; ++x) {
			double sx = x / static_cast<double>(width);
			Ray ray = camera.generateRay(sx, sy);
			Result result = scene.intersect(ray);
			if (!result.isEmpty) {
				double depth = 1.0 - fmin(result.distance / maxDepth * 1.0, 1.0);
				pixels[index].x = depth;
				pixels[index].y = depth;
				pixels[index].z = depth;
			}
			
			++index;
		}
	}
}

void renderNormal(Vector3 *pixels) {
	int index = 0;
	for (int y = 0; y < height; ++y) {
		double sy = 1.0 - y / static_cast<double>(height);
		for (int x = 0; x < width; ++x) {
			double sx = x / static_cast<double>(width);
			Ray ray = camera.generateRay(sx, sy);
			Result result = scene.intersect(ray);
			if (!result.isEmpty) {
				pixels[index].x = (result.normal.x + 1.0)*0.5;
				pixels[index].y = (result.normal.y + 1.0)*0.5;
				pixels[index].z = (result.normal.z + 1.0)*0.5;
			}
			++index;
		}
	}
}

void rayTrace(Vector3 *pixels) {
	int index = 0;
	for (int y = 0; y < height; ++y) {
		double sy = 1.0 - y / static_cast<double>(height);
		for (int x = 0; x < width; ++x) {
			double sx = x / static_cast<double>(width);
			Ray ray = camera.generateRay(sx, sy);
			Result result = scene.intersect(ray);
			if (!result.isEmpty) {
				Color color = result.target->PhongMSample(ray, result, light);
				pixels[index].x = color.x;
				pixels[index].y = color.y;
				pixels[index].z = color.z;
			}
			++index;
		}
	}
}

Color rayTraceRecurisve(Ray ray,int maxReflect) {
	Result result = scene.intersect(ray);
	if (!result.isEmpty) {
		double reflectiveness = result.target->reflectiveness;
		Color color = result.target->PhongMSample(ray, result,light);
		color = color*(1 - reflectiveness);
		if (reflectiveness>0&&maxReflect>0) {
			Vector3 r = result.normal*(-2 * result.normal.Dot(ray.direction)) + ray.direction;
			ray = Ray(result.position, r);
			Color reflectColor = rayTraceRecurisve(ray, maxReflect - 1);
			color = color + reflectColor*reflectiveness;
		}
		return color;
	}else {
		return Color(0.0, 0.0, 0.0);
	}
}

void rayTraceReflection(Vector3 *pixels,int maxflect) {
	int index = 0;
	for (int y = 0; y < height; ++y) {
		double sy = 1.0 - y / static_cast<double>(height);
		for (int x = 0; x < width; ++x) {
			double sx = x / static_cast<double>(width);
			Ray ray = camera.generateRay(sx, sy);
			Color color = rayTraceRecurisve(ray, maxflect);
			pixels[index].x = color.x;
			pixels[index].y = color.y;
			pixels[index].z = color.z;
			++index;
		}
	}
}

int main() {
	light.Color = Vector3(1.0, 1.0, 1.0);
	light.Dir = Vector3(1.0, 1.0, 1.0).Normalize();
	const int pixs_sample = 10;
	sphere1.phongMaterial(Color(1.0, 0.0, 0.0), Color(1.0, 1.0, 1.0), 16, 0.25);
	sphere2.phongMaterial(Color(0.0, 0.0, 1.0), Color(1.0, 1.0, 1.0), 16, 0.25);
	scene.SceneAdd(sphere2);
	scene.SceneAdd(sphere1); 
	Vector3 *pixels = new Vector3[width*height];
	double maxDepth =30.0;
	clock_t preTime=clock();
	//renderDepth(pixels, maxDepth);
	//renderNormal(pixels);
	//rayTrace(pixels);
	rayTraceReflection(pixels, 3);
	clock_t nowTime = clock();
	cout << (nowTime - preTime)/ (double)CLOCKS_PER_SEC;
	WritePPM(width, height, pixels, "depth.ppm");
	system("pause");
	return 0;
}