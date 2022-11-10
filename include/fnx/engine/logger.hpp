#pragma once

#define FNX_ERROR(...) { auto [physicsCommon,_] = fnx::singleton<reactphysics3d::PhysicsCommon>::acquire(); physicsCommon.getLogger()->log(reactphysics3d::Logger::Level::Error, "fnx", reactphysics3d::Logger::Category::World, __VA_ARGS__, __FILE__, __LINE__); }
#define FNX_WARN(...) { auto [physicsCommon,_] = fnx::singleton<reactphysics3d::PhysicsCommon>::acquire(); physicsCommon.getLogger()->log(reactphysics3d::Logger::Level::Warning, "fnx", reactphysics3d::Logger::Category::World, __VA_ARGS__, __FILE__, __LINE__); }
#define FNX_INFO(...) { auto [physicsCommon,_] = fnx::singleton<reactphysics3d::PhysicsCommon>::acquire(); physicsCommon.getLogger()->log(reactphysics3d::Logger::Level::Information, "fnx", reactphysics3d::Logger::Category::World, __VA_ARGS__, __FILE__, __LINE__); }
#ifdef _DEBUG
    #define FNX_DEBUG(...) { auto [physicsCommon,_] = fnx::singleton<reactphysics3d::PhysicsCommon>::acquire(); physicsCommon.getLogger()->log(reactphysics3d::Logger::Level::Information, "fnx", reactphysics3d::Logger::Category::World, __VA_ARGS__, __FILE__, __LINE__); }
#else
    #define FNX_DEBUG(...)
#endif