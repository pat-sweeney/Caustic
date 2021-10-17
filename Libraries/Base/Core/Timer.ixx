//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <chrono>
#include <ctime>
#include <cmath>

export module Base.Core.Timer;
import Base.Core.Core;

export namespace Caustic
{
	//**********************************************************************
	// Class: CTimer
	// CPU based timer
	//
	// Module:
	// {Link:import Base.Core.Timer;{Base/Core/Timer.ixx}}
	//**********************************************************************
	class CTimer
	{
		std::chrono::system_clock::time_point m_startTime;
	public:
		//**********************************************************************
		// Constructor: CTimer
		// Defines the default constructor
		//**********************************************************************
		CTimer()
		{
		}

		//**********************************************************************
		// Destructor: ~CTimer
		// Defines the destructor
		//**********************************************************************
		~CTimer()
		{
		}

		//**********************************************************************
		// Method: Start
		// Starts the timer
		//**********************************************************************
		void Start()
		{
			m_startTime = std::chrono::system_clock::now();
		}

		//**********************************************************************
		// Method: Stop
		// Stops the timer.
		//
		// Returns:
		// Number of milliseconds since timer started
		//**********************************************************************
		uint32 Stop()
		{
			auto t = std::chrono::system_clock::now();
			return (uint32)(std::chrono::duration_cast<std::chrono::milliseconds>(t - m_startTime).count());
		}
	};
};
