#pragma once
struct Sphere;
struct Result
{
	double distance;
	Vector3 position;
	Vector3 normal;
	Sphere *target;
	bool isEmpty=true;
};

struct Sphere {
	Sphere(Vector3 c, double  r) :center(c), redius(r),sqrRadius(r*r) {};
	void phongMaterial(const Color &diffuse,const Color &specular,const double &shininess,const double &reflectiveness);
	Color PhongMSample(const Ray&,const Result&,const Light&) const;
	Result intersect(Ray ray);
	Vector3 center;
	double sqrRadius, redius;
	Color diffuse, specular;
	double shininess,reflectiveness;
};

Color Sphere::PhongMSample(const Ray& ray,const Result &result,const Light& light) const{
	double NdotL = result.normal.Dot(light.Dir);
	Vector3 H = (light.Dir - ray.direction).Normalize();
	double NdotH = result.normal.Dot(H);
	//cout << NdotH << endl;
	Color diffusTerm = this->diffuse*fmax(NdotH, 0.0);
	Color specularTerm = this->specular*pow(fmax(NdotH, 0.0), this->shininess);
	//cout << specularTerm << endl;
	return light.Color*(diffusTerm + specularTerm);
}

void Sphere::phongMaterial(const Color &diffuse,const Color &specular,const double &shininess,const double &reflectiveness) {
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	this->reflectiveness = reflectiveness;
}

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
			result.target = this;
			result.normal = (result.position - this->center).Normalize();
			return result;
		}
	}
	return result;
}