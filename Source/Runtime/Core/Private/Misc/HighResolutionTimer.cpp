#include "GenericPlatform/GameFrameworkPCH.h"

#include <Misc/HighResolutionTimer.h>
#include <TimeRecorder.h>
using namespace std::chrono;

#define WINDOWS_CLOCK         0
#define STEADY_CLOCK          1

#ifndef USE_CLOCK
    #define USE_CLOCK STEADY_CLOCK
#endif

#if USE_CLOCK == WINDOWS_CLOCK
class HighResolutionTimer::impl
{
public:
    impl()
    : t0 { 0 }
    , t1 { 0 }
    , frequency { 0 }
    , elapsedNanoseconds( 0.0 )
    , totalNanseconds( 0.0 )
    {
        ::QueryPerformanceFrequency( &frequency );
        ::QueryPerformanceCounter( &t0 );
    }

    void Tick()
    {
        ::QueryPerformanceCounter( &t1 );

        elapsedNanoseconds = ( t1.QuadPart - t0.QuadPart ) * ( 1e9 / frequency.QuadPart );
        totalNanseconds += elapsedNanoseconds;

        t0 = t1;
    }

    void Reset()
    {
        ::QueryPerformanceCounter( &t0 );
        elapsedNanoseconds = 0.0;
        totalNanseconds    = 0.0;
    }

    double ElapsedNanoseconds() const
    {
        return elapsedNanoseconds;
    }

    double TotalNanoseconds() const
    {
        return totalNanseconds;
    }

private:
    LARGE_INTEGER t0, t1;
    LARGE_INTEGER frequency;
    double        elapsedNanoseconds;
    double        totalNanseconds;
};
#elif USE_CLOCK == STEADY_CLOCK
class HighResolutionTimer::impl
{
public:
    impl()
    {
    }

    void Tick()
    {
        Recorder.Tick();
    }

    void Reset()
    {
        Recorder.Reset();
    }

    double ElapsedNanoseconds() const
    {
        return double(Recorder.GetDelta<std::chrono::nanoseconds>().count());
    }

    double TotalNanoseconds() const
    {
        return double(Recorder.GetTotalTime<std::chrono::nanoseconds>().count());
    }

private:
    FTimeRecorder Recorder;
};
#endif

HighResolutionTimer::HighResolutionTimer()
{
    pImpl = std::make_unique<impl>();
}

HighResolutionTimer::~HighResolutionTimer() {}

void HighResolutionTimer::Tick()
{
    pImpl->Tick();
}

void HighResolutionTimer::Reset()
{
    pImpl->Reset();
}

double HighResolutionTimer::ElapsedSeconds() const
{
    return pImpl->ElapsedNanoseconds() * 1e-9;
}

double HighResolutionTimer::ElapsedMilliseconds() const
{
    return pImpl->ElapsedNanoseconds() * 1e-6;
}

double HighResolutionTimer::ElapsedMicroseconds() const
{
    return pImpl->ElapsedNanoseconds() * 1e-3;
}

double HighResolutionTimer::ElapsedNanoseconds() const
{
    return pImpl->ElapsedNanoseconds();
}

double HighResolutionTimer::TotalSeconds() const
{
    return pImpl->TotalNanoseconds() * 1e-9;
}

double HighResolutionTimer::TotalMilliseconds() const
{
    return pImpl->TotalNanoseconds() * 1e-6;
}

double HighResolutionTimer::TotalMicroseconds() const
{
    return pImpl->TotalNanoseconds() * 1e-3;
}

double HighResolutionTimer::TotalNanoseconds() const
{
    return pImpl->TotalNanoseconds();
}