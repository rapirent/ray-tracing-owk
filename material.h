#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h" // why??

struct hit_record; // why??

class material {
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        //https://en.wikipedia.org/wiki/Albedo
        color albedo;
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        )  const override {
            auto scatter_direction = rec.normal + random_unit_vector(); //Lambertian Reflection: consine distribution approx
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {
    public:
        color albedo;
        double fuzz;
        metal(const color& a, double f) : albedo(a), fuzz(f<1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        )  const override {
            auto reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, fuzz*random_in_unit_sphere() + reflected);
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal)>0;
        }

};

class dielectric : public material {
    public:
        double ir;
        dielectric(double index_of_refraction): ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        )  const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir; //eta
            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            vec3 direction;
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            if (cannot_refract||reflectance(cos_theta, refraction_ratio)>random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction= refract(unit_direction, rec.normal, refraction_ratio);
            scattered = ray(rec.p, direction);
            return true;
        }
    private:
        static double reflectance(double cosine, double ref_idx) {
            //https://en.wikipedia.org/wiki/Schlick%27s_approximation
            //https://graphicscompendium.com/raytracing/11-fresnel-beer
            auto r0 = (1-ref_idx)/(1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1-cosine), 5);
        }
};

#endif
