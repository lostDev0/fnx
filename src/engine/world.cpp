using namespace reactphysics3d;
namespace fnx
{
    namespace world
    {
        void init()
        {
            { singleton<PhysicsCommon>::acquire(); }
            { singleton<event_manager>::acquire(); }
            { singleton<audio_manager>::acquire(); }
        }
    }
}