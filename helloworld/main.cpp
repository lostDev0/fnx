/*
 * This is a "hello world" project with a basic example of how to use
 * the ReactPhysics3D library in your project. This example is a simple
 * rigid body that is falling down because of gravity.
 */

// Libraries
#include "fnx/fnx.hpp"
#include <iostream>

// ReactPhysics3D namespace
using namespace reactphysics3d;

// Main function
int main(int argc, char** argv) {

    // First you need to create the PhysicsCommon object. This is a factory module
    // that you can use to create physics world and other objects. It is also responsible
    // for logging and memory management
    PhysicsWorld* world{nullptr};
    DefaultLogger* logger{nullptr};

    {
        // Initialize global singleton physics world for use by the FNX engine
        auto [physicsCommon,_] = fnx::singleton<PhysicsCommon>::acquire();

        // Create a physics world
        world = physicsCommon.createPhysicsWorld();
        logger = physicsCommon.createDefaultLogger();
        auto logLevel = static_cast<uint>(static_cast<uint>(Logger::Level::Warning) | static_cast<uint>(Logger::Level::Error) | static_cast<uint>(Logger::Level::Information));
        logger->addStreamDestination(std::cout, logLevel, DefaultLogger::Format::Text);
        physicsCommon.setLogger(logger);
    }

    // Create a rigid body in the world
    Vector3 position(0, 20, 0);
    Quaternion orientation = Quaternion::identity();
    Transform transform(position, orientation);
    RigidBody* body = world->createRigidBody(transform);

    const decimal timeStep = 1.0f / 60.0f;

    // Step the simulation a few steps
    for (int i=0; i < 20; i++) {

        world->update(timeStep);

        // Get the updated position of the body
        const Transform& transform = body->getTransform();
        const Vector3& position = transform.getPosition();

        // Display the position of the body
        logger->log(Logger::Level::Information, world->getName(), Logger::Category::World, 
            std::string("Body Position: ") + std::to_string(position.x) + std::string(", ") + std::to_string(position.y) + std::string(", ") + std::to_string(position.z), __FILE__, __LINE__);
        
    }

    return 0;
}