#include "head.hpp"
Camera camera(Vector3(0.0, 10.0, 10.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0), 90.0);
Scene scene;
const int width = 1024;
const int height = 1024;
Sphere sphere(Vector3(0.0, 10.0, -10.0), 10.0);
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
int main() {
	const int pixs_sample = 10;
	scene.SceneAdd(sphere);
	Vector3 *pixels = new Vector3[width*height];
	double maxDepth =15.0;
	//renderDepth(pixels, maxDepth);
	renderNormal(pixels);
	WritePPM(width, height, pixels, "depth.ppm");
	system("pause");
	return 0;
}