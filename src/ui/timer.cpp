using namespace std;
using namespace std::chrono;

namespace fnx
{
timer::timer( std::initializer_list<double> l )
    : _times{ l }
{
    for ( const auto& t : _times )
    {
        _total += t;
    }
}

void timer::start()
{
    _current_time = 0.0;
    _running = true;
}

void timer::stop()
{
    _running = false;
}

bool timer::get_time_slice()
{
    if ( _running )
    {
        if ( _repeating )
        {
            _current_time = std::fmod( _current_time, _total );
        }
        double val = 0.0;
        bool even = false;
        for ( const auto& t : _times )
        {
            val += t;
            if ( _current_time <= val )
            {
                // within time slice
                return even;
            }
            even = !even;
        }
    }
    return false;
}

bool timer::is_on()
{
    return get_time_slice();
}

void timer::update( double delta )
{
    _current_time += delta;
}
}
