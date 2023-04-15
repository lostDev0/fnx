#pragma once

#include <vector>

namespace fnx
{
class camera_manager
{
public:
    enum index : int
    {
        invalid = -1,
        ui = 0,
        default = 1
    };

    using handle = fnx::reference_ptr<fnx::camera>;

    camera_manager() = default;
    ~camera_manager() = default;

    camera_manager::handle get( int idx )
    {
        camera_manager::handle out;
        if ( is_valid( idx ) )
        {
            out = _cameras[idx];
        }
        return out;
    }

    int add( camera_manager::handle camera )
    {
        if ( camera )
        {
            _cameras.emplace_back( camera );
            return _cameras.size() - 1;
        }
        return index::invalid;
    }

    int add( camera_manager::handle camera, int idx )
    {
        if ( camera && is_valid( idx ) )
        {
            _cameras[idx] = camera;
            return idx;
        }
        else if( camera )
        {
            // Camera hasn't yet been added
            for(auto i = _cameras.size(); i <= idx; i++)
            {
                if (i != idx)
                {
                    _cameras.emplace_back(camera_manager::handle{});
                }
                else
                {
                    _cameras.emplace_back(camera);
                }
            }
        }
        return index::invalid;
    }

    bool destroy( int idx )
    {
        if ( is_valid( idx ) )
        {
            _cameras[idx].reset();
            return true;
        }
        return false;
    }

private:
    std::vector<camera_manager::handle> _cameras;

    inline bool is_valid( int idx )
    {
        return idx > camera_manager::index::invalid && idx < _cameras.size() && _cameras[idx] != nullptr;
    }
};
}
