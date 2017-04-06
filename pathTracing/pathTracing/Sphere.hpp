#pragma once
struct Result
{
	double distance;
	Vector3 position;
	Vector3 normal;
	bool isEmpty=true;
};

struct Sphere {
	Sphere(Vector3 c, int  r) :center(c), redius(r),sqrRadius(r*r) {};
	Result intersect(Ray ray);
	Vector3 center;
	double sqrRadius, redius;
};

Result Sphere::intersect(Ray ray) {
	Vector3 v = ray.origin-this->center;
	double a0 = v.Norm2_squared() - this->sqrRadius;
	double  DdotV = ray.direction.Dot(v);
	Result result;
	if (DdotV<=0.0) {
		double discr = DdotV*DdotV - a0;
		if (discr>=0.0) {
			result.isEmpty = false;
			result.distance = -DdotV - sqrt(discr);
			result.position = ray.getPoint(result.distance);
			result.normal = (result.position - this->center).Normalize();
			return result;
		}
	}
	return result;
}