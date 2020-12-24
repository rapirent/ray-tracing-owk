#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

class camera {
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist
        ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0*h;
            auto viewport_width = aspect_ratio*viewport_height;

            auto w = unit_vector(lookfrom - lookat); //front
            auto u = unit_vector(cross(vup, w));  //up
            auto v = cross(w, u); // right
            //auto focal_length = 1.0; // distance between the projection plane and projection point

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w; //left lower point of image
            /* The focus distance is controlled by the distance between the lens and the film/sensor.
             * The “aperture” is a hole to control how big the lens is effectively.
             * For a real camera, if you need more light you make the aperture bigger,
             * and will get more defocus blur. */


        }
        ray get_ray(double s, double t) const {
            vec3 random_dot = lens_radius * random_in_unit_disk();
            vec3 offset  = u * random_dot.x() + v * random_dot.y();
            //ray from origin to image
            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }
    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
};

#endif
