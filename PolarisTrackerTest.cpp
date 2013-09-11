#include "PolarisTracker.h"

int main(int argc, char *  argv[])
{

    igstk::RealTimeClock::Initialize();

    PolarisTracker * tracker = PolarisTracker::New();

    std::cout<<"-Initializing Logger"<<std::endl;
    tracker->initializeLogger();
    std::cout<<"-Initializing SerialCommunication"<<std::endl;
    tracker->initializeSerialCommunication(3);
    std::cout<<"-Initializing Tracker"<<std::endl;
    tracker->initializeTracker();
    std::cout<<"-Initializing Tracker Tool 8700340"<<std::endl;
    tracker->initializeTrackerTool("8700340.rom");
	std::cout<<"-Initializing Tracker Tool PolarisTool120"<<std::endl;
    tracker->initializeTrackerTool("PolarisTool120.rom");
	std::cout<<"-Attaching all tools"<<std::endl;
	tracker->attachAllTools();
	std::cout<<"-Create all tools observers"<<std::endl;
	tracker->createToolsObervers();
    std::cout<<"-Start Tracking"<<std::endl;
    tracker->startTracking();
    std::cout<<"-Transformation observer"<<std::endl;
	tracker->observeAllToolsTransformations();
	std::cout<<"-Number of tools attached to tracker "<<tracker->getNumberofTools()<<std::endl;
    std::cout<<"-Tracking with tool 0"<<std::endl;    
	tracker->track(0);
	std::cout<<"-Tracking with tool 1"<<std::endl;
	tracker->track(1);
	std::cout<<"-Detaching all tools"<<std::endl;
    tracker->detachAllTools();
	std::cout<<"-Stop Tracking"<<std::endl;
    tracker->stopTracking();
	
	bool exit = false;

    while(!exit){
		if(std::cin.get()=='\n')
			exit=true;
    }

	

    return 0;


}
