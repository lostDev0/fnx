#pragma once

#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include <map>
#include <unordered_map>
#include <array>
#include <type_traits>
#include <list>
#include <climits>
#include <stdio.h>

// Dependencies
#pragma warning(disable: 4244)
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(default: 4244)

#include "utils/template_utils.hpp"
#include "utils/macro_utils.hpp"
#include "utils/string_utils.hpp"
#include "utils/react_utils.hpp"

#include "math/math.hpp"
#include "math/angle.hpp"
#include "math/matrix4x4.hpp"

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

#include "engine/colors.hpp"
#include "engine/constants.hpp"
#include "engine/config_constants.hpp"
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
#include "engine/alignment.hpp"
#include "engine/window.hpp"
#include "engine/shader.hpp"
#include "engine/raw_image.hpp"
#include "engine/texture.hpp"
#include "engine/material_map.hpp"
#include "engine/material.hpp"
#include "engine/raw_model.hpp"
#include "engine/model.hpp"
#include "engine/lights.hpp"
#include "engine/font.hpp"
#include "engine/type_names.hpp"
#include "engine/camera.hpp"
#include "engine/ortho_camera.hpp"
#include "engine/perspective_camera.hpp"
#include "engine/layer.hpp"
#include "engine/layer_stack.hpp"

#include "engine/world.hpp"
