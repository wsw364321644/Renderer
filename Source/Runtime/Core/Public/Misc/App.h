#pragma once


/**
 * Provides information about the application.
 */
class  FApp
{
public:

	
	/**
	 * Gets current time in seconds.
	 *
	 * @return Current time in seconds.
	 */
	static double GetCurrentTime()
	{
		return CurrentTime;
	}

	/**
	 * Sets current time in seconds.
	 *
	 * @param seconds - Time in seconds.
	 */
	static void SetCurrentTime(double Seconds)
	{
		CurrentTime = Seconds;
	}

	/**
	 * Gets previous value of CurrentTime.
	 *
	 * @return Previous value of CurrentTime.
	 */
	static double GetLastTime()
	{
		return LastTime;
	}

	/** Updates Last time to CurrentTime. */
	static void UpdateLastTime()
	{
		LastTime = CurrentTime;
	}

	/**
	 * Gets time delta in seconds.
	 *
	 * @return Time delta in seconds.
	 */
	static double GetDeltaTime()
	{
		return DeltaTime;
	}

	/**
	 * Sets time delta in seconds.
	 *
	 * @param seconds Time in seconds.
	 */
	static void SetDeltaTime(double Seconds)
	{
		DeltaTime = Seconds;
	}

	/**
	 * Gets idle time in seconds.
	 *
	 * @return Idle time in seconds.
	 */
	 static double GetIdleTime()
	{
		return IdleTime;
	}

	/**
	 * Sets idle time in seconds.
	 *
	 * @param seconds - Idle time in seconds.
	 */
	static void SetIdleTime(double Seconds)
	{
		IdleTime = Seconds;
	}

	/**
	 * Gets overall game time in seconds.
	 *
	 * @return Overall game time in seconds.
	 */
	 static double GetGameTime()
	{
		return GameTime;
	}

	/**
	 * Sets overall game time in seconds.
	 *
	* @param seconds - Overall game time in seconds.
	 */
	static void SetGameTime(double Seconds)
	{
		GameTime = Seconds;
	}

	/**
	 * Gets idle time overshoot in seconds (the time beyond the wait time we requested for the frame). Only valid when IdleTime is > 0.
	 *
	 * @return Idle time in seconds.
	 */
	 static double GetIdleTimeOvershoot()
	{
		return IdleTimeOvershoot;
	}

	/**
	 * Sets idle time overshoot in seconds (the time beyond the wait time we requested for the frame). Only valid when IdleTime is > 0.
	 *
	 * @param seconds - Idle time in seconds.
	 */
	static void SetIdleTimeOvershoot(double Seconds)
	{
		IdleTimeOvershoot = Seconds;
	}

	

private:


	/** Holds current time. */
	static double CurrentTime;

	/** Holds previous value of CurrentTime. */
	static double LastTime;

	/** Holds current delta time in seconds. */
	static double DeltaTime;

	/** Holds time we spent sleeping in UpdateTimeAndHandleMaxTickRate() if our frame time was smaller than one allowed by target FPS. */
	static double IdleTime;

	/** Holds the amount of IdleTime that was LONGER than we tried to sleep. The OS can't sleep the exact amount of time, so this measures that overshoot. */
	static double IdleTimeOvershoot;

	/** Holds overall game time. */
	static double GameTime;

	
};

