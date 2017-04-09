#pragma once
struct Scene
{
	std::vector<Sphere> sphere;
	Scene() = default;
	inline const void SceneAdd(const Sphere &s) { sphere.push_back(s); };
	Result intersect(Ray ray);
};

Result Scene::intersect(Ray ray) {
	double minDis = INF;
	Result minResult;
	//3775//2335
	//0.036
	for (std::vector<Sphere>::iterator it = sphere.begin(), end = sphere.end(); it != end; ++it) {
		
		Result result = it->intersect(ray);
		if (result.isEmpty)	continue;
		if (result.distance<minDis) {
			minDis = result.distance;
			minResult = result;
		}
	}
	return minResult;
}