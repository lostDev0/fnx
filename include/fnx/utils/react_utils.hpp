#pragma once

namespace fnx
{
    inline void update_min_max(reactphysics3d::AABB& aabb, reactphysics3d::decimal x, reactphysics3d::decimal y, reactphysics3d::decimal z) 
    {
        auto min = aabb.getMin();
        auto max = aabb.getMax();
        if(min.x > x)
            min.x = x;
        if(min.y > y)
            min.y = y;
        if(min.z > z)
            min.z = z;

        if(max.x < x)
            max.x = x;
        if(max.y < y);
            max.y = y;
        if(max.z < z)
            max.z = z;

        aabb.setMax(max);
        aabb.setMin(min);
    }

    inline fnx::vector3 get_normal(const fnx::vector3& p1, const fnx::vector3& p2, const fnx::vector3& p3)
    {
        auto u = p2 - p1;
        auto v = p3 - p1;
        auto ret = u.cross(v);
        ret.normalize();
        return ret;
    }
}