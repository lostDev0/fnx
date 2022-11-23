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
#include <yaml-cpp/yaml.h>
#pragma warning(default: 4244)

namespace fnx
{
using vector2 = reactphysics3d::Vector2;
using vector3 = reactphysics3d::Vector3;
using decimal = reactphysics3d::decimal;
}

#include "utils/template_utils.hpp"
#include "utils/macro_utils.hpp"
#include "utils/string_utils.hpp"
#include "utils/react_utils.hpp"
#include "utils/yaml_utils.hpp"

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
#include "core/alignment.hpp"
#include "core/byte_stream.hpp"
#include "core/serializer.hpp"

#include "math/math.hpp"
#include "math/angle.hpp"
#include "math/matrix4x4.hpp"
#include "math/vector4.hpp"
#include "math/rect.hpp"

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
#include "engine/renderer.hpp"
#include "engine/property.hpp"
#include "engine/property_manager.hpp"

#include "ui/interface.hpp"
#include "ui/transition.hpp"
#include "ui/animator.hpp"
#include "ui/constraints.hpp"
#include "ui/timer.hpp"
#include "ui/widget.hpp"
#include "ui/widget_config.hpp"
#include "ui/block.hpp"
#include "ui/image.hpp"
#include "ui/label.hpp"
#include "ui/text_box.hpp"
#include "ui/text_input.hpp"
#include "ui/radio.hpp"
#include "ui/progress_bar.hpp"
#include "ui/slider.hpp"
#include "ui/sizer.hpp"
#include "ui/button.hpp"
#include "ui/line.hpp"
#include "ui/collapsable.hpp"
#include "ui/clipping_container.hpp"
#include "ui/panel.hpp"
#include "ui/mutually_exclusive_container.hpp"
#include "ui/scroll_view.hpp"
#include "ui/tabbed_notebook.hpp"
#include "ui/layer.hpp"
#include "ui/layer_stack.hpp"
#include "ui/layer_serializer.hpp"

#include "engine/world.hpp"
