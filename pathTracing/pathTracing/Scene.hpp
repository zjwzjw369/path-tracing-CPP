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
	for (auto it = sphere.begin(); it != sphere.end(); ++it) {
		Result result = it->intersect(ray);
		if (result.distance<minDis) {
			minDis = result.distance;
			minResult = result;
		}
	}
	return minResult;
}