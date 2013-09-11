#include "MedSafeTracker.h"

int main(int argc, char *  argv[])
{

    igstk::RealTimeClock::Initialize();

    MedSafeTracker * tracker = MedSafeTracker::New();

	std::cout<<"Initializing Logger"<<std::endl;
    tracker->InitializeLogger();
	std::cout<<"Initializing Tracker"<<std::endl;
    tracker->InitializeTracker();
	std::cout<<"Initializing Tracker Tool"<<std::endl;
    tracker->InitializeTrackerTool(3);
	std::cout<<"Start Tracking"<<std::endl;
    tracker->StartTracking();
	std::cout<<"Transformation observer"<<std::endl;
    tracker->ObserveTransformations();

    bool exit = false;

    while(!exit){

        tracker->Track();

        if(std::cin.get()=='\n')
            exit=true;

    }

    tracker->StopTracking();

    return 0;


}
