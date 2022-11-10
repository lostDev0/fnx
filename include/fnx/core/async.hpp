#pragma once

#include <atomic>
#include <mutex>
#include <thread>

namespace fnx
{
class async_task
{
public:
    async_task()
        : _thread( [ & ]()
    {
        while ( _running.load() )
        {
            {
                std::unique_lock<std::mutex> lock( _lock, std::try_to_lock );
                run();
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
        }
    } )
    {
    }

    virtual ~async_task()
    {
        _running.store( false );
        if ( _thread.joinable() )
        {
            _thread.join();
        }
    }

    virtual void stop()
    {
        _running.store( false );
    }

protected:
    virtual void run() = 0;

    virtual void join()
    {
        _running.store( false );
        if ( _thread.joinable() )
        {
            _thread.join();
        }
    }

    std::mutex _lock;
    std::thread _thread;
    std::atomic<bool> _running{ true };
};
}