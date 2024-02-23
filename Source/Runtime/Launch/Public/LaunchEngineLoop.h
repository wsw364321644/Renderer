#pragma once


/**
 * Implements the main engine loop.	
 */
class FEngineLoop
{
public:

	/** Default constructor. */
	FEngineLoop();

	virtual ~FEngineLoop() { }

public:

	
	bool Init();
	void Tick();
	void Run();
};

