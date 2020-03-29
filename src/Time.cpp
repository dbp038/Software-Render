#include "pch.h"
#include "Time.h"
#include "Application.h"

UnscaledTimer::UnscaledTimer() {
	Reset();
}

UnscaledTimer::UnscaledTimer( double goal ) : goal( SecondsToTicks( goal ) ) {
	Reset();
}

uint64_t UnscaledTimer::GetTimeStamp() const {
	uint64_t now;
	::QueryPerformanceCounter( (LARGE_INTEGER *)&now );
	return now;
}

double UnscaledTimer::Elapsed() const {
	uint64_t now = GetTimeStamp();
	return TicksToSeconds( now - timeStamp );
}

double UnscaledTimer::Remaining() const {
	uint64_t now = GetTimeStamp();
	if ( now > goal )
		return 0.0;
	else 
		return TicksToSeconds( goal - now );
}

bool UnscaledTimer::IsFinished() const {
	uint64_t now = GetTimeStamp();
	return now >= goal;
}

void UnscaledTimer::Reset() {
	goal = goal - timeStamp;
	timeStamp = GetTimeStamp();
	goal += timeStamp;
}

double UnscaledTimer::ElapsedAndReset() {
	double delta = Elapsed();
	Reset();
	return delta;
}

double UnscaledTimer::TicksToSeconds( const uint64_t ticks ) const {
	LARGE_INTEGER freq;
	if ( ::QueryPerformanceFrequency( &freq ) ) {
		double delta_secs = (double)( ticks ) / (double)freq.QuadPart;
		return delta_secs;
	}
	else
		throw std::exception( "QueryPerformanceFrequency returned false." );

	return 0.0;
}

uint64_t UnscaledTimer::SecondsToTicks( const float sec ) const {
	LARGE_INTEGER freq;
	if ( ::QueryPerformanceFrequency( &freq ) )
		return uint64_t( sec * (long double)freq.QuadPart );
	else
		throw std::exception( "QueryPerformanceFrequency returned false." );

	return 0;
}

ScaledTimer::ScaledTimer() {
	Reset();
}

ScaledTimer::ScaledTimer( double goal ) : goal( goal ) {
	Reset();
}

double ScaledTimer::Elapsed() const {
	return Time.totalTime - timeStamp;
}

double ScaledTimer::Remaining() const {
	double now = Time.totalTime;
	if ( now > goal )
		return 0.0;
	else
		return goal - now;
}

bool ScaledTimer::IsFinished() const {
	return Time.totalTime >= goal;
}

void ScaledTimer::Reset() {
	goal = goal - timeStamp;
	timeStamp = Time.totalTime;
	goal += timeStamp;
}

double ScaledTimer::ElapsedAndReset() {
	double delta = Elapsed();
	Reset();
	return delta;
}
