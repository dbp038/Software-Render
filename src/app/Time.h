#pragma once

struct TimeData {
	float delta = 0.0f;
	double totalTime = 0.0f;
	float timeScale = 1.0f;
};

class ITimer {

public:

	virtual double Elapsed() const = 0;
	virtual double Remaining() const = 0;
	virtual bool IsFinished() const = 0;

	// Reset counter to current timestamp
	virtual void Reset() = 0;

	virtual double ElapsedAndReset() = 0;

};

class UnscaledTimer : public ITimer {

public:

	UnscaledTimer();
	UnscaledTimer( float goal );

	double Elapsed() const override;
	double Remaining() const override;
	bool IsFinished() const override;

	void Reset() override;

	double ElapsedAndReset() override;

private:

	uint64_t timeStamp = 0, goal = 0;

	// In ticks!
	uint64_t GetTimeStamp() const;

	double TicksToSeconds( const uint64_t ticks ) const;
	uint64_t SecondsToTicks( const float sec ) const;

};

class ScaledTimer : public ITimer {

public:

	ScaledTimer();
	ScaledTimer( float goal );

	double Elapsed() const override;
	double Remaining() const override;
	bool IsFinished() const override;

	void Reset() override;

	double ElapsedAndReset() override;

private:

	double timeStamp = 0.0, goal = 0.0;

};