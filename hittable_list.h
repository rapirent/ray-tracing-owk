#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr; //record how many ptr point to same obj
using std::make_shared;

// specify using public part of hittable
class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects; // >> is writtable after c++11
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record tmp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;
    for (const auto& object : objects) { // type inference and loop over objects
        if (object->hit(r, t_min, closest_so_far, tmp_rec)) { //(*object).hit
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }
    return hit_anything;
}

#endif
