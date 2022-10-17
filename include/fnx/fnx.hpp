#pragma once

#include "utils/template_utils.hpp"
#include "utils/macro_utils.hpp"
#include "utils/string_utils.hpp"

#include "math/math.hpp"

#include "containers/dequeue.hpp"
#include "containers/ring_buffer.hpp"
#include "containers/unordered_vector.hpp"
#include "containers/bitset.hpp"

#include "memory/heap_allocator.hpp"
#include "memory/heap_indexed_pool.hpp"
#include "memory/function_ref.hpp"
#include "memory/reference_ptr.hpp"

#include "core/id_manager.hpp"
#include "core/tween.hpp"
#include "core/singleton.hpp"
#include "core/async.hpp"

// Dependencies
#pragma warning(disable: 4244)
#include <reactphysics3d/reactphysics3d.h>
#include <stbimage/stb_image.h>
#pragma warning(default: 4244)

#include "engine/logger.hpp"
#include "engine/dispatcher.hpp"
#include "engine/events.hpp"
#include "engine/keys.hpp"
#include "engine/engine_events.hpp"
#include "engine/asset.hpp"
#include "engine/asset_manager.hpp"
#include "engine/sound.hpp"
#include "engine/audio_manager.hpp"
#include "engine/display_mode.hpp"
#include "engine/window.hpp"

#include "engine/world.hpp"
