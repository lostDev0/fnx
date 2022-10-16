#include "utils/template_utils.hpp"

#include "math/math.hpp"

#include "containers/dequeue.hpp"
#include "containers/ring_buffer.hpp"
#include "containers/unordered_vector.hpp"
#include "containers/bitset.hpp"

#include "memory/heap_allocator.hpp"
#include "memory/heap_indexed_pool.hpp"
#include "memory/function_ref.hpp"

#include "core/id_manager.hpp"
#include "core/tween.hpp"
#include "core/singleton.hpp"

#include <reactphysics3d/reactphysics3d.h>

#include "ecs/config.hpp"
#include "ecs/component.hpp"
//#include "ecs/entity.hpp"

#include "engine/dispatcher.hpp"
#include "engine/events.hpp"
