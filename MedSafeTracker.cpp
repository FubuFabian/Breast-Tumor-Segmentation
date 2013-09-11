#include "MedSafeTracker.h"

void MedSafeTracker::InitializeTracker()
{
    my_command = MedSafeTrackerCommand::New();

	tracker = igstk::Ascension3DGTracker::New();
    tracker->AddObserver(itk::AnyEvent(), my_command);
    
	std::cout<<"	Setting Logger"<<std::endl;
	tracker->SetLogger(logger);
	std::cout<<"	Setting Frequency"<<std::endl;
    tracker->RequestSetFrequency(60);
	std::cout<<"	Opening"<<std::endl;
    tracker->RequestOpen();


}

void MedSafeTracker::InitializeTrackerTool(int port)
{

    trackerTool = AscensionTracker3DGToolType::New();
	
	std::cout<<"	Setting Port"<<std::endl;
    trackerTool->RequestSetPortNumber(port);
	std::cout<<"	Configuring"<<std::endl;
    trackerTool->RequestConfigure();
	std::cout<<"	Attaching to tracker"<<std::endl;
    trackerTool->RequestAttachToTracker(tracker);
    coordSystemAObserver = ObserverType::New();
    coordSystemAObserver->ObserveTransformEventsFrom( trackerTool );


}

void MedSafeTracker::InitializeLogger()
{
	logger = LoggerType::New();
	logOutput = LogOutputType::New(); 

	std::string filename = "Logger.txt";
	std::cout<<"	Logger output saved here: "<<filename<<std::endl;;
	
	std::cout<<"	Opening file"<<std::endl;
	loggerFile.open( filename.c_str() );
	logOutput->SetStream( loggerFile );
	std::cout<<"	Adding file"<<std::endl;
	logger->AddLogOutput( logOutput );
	std::cout<<"	Setting priority"<<std::endl;
	logger->SetPriorityLevel( LoggerType::DEBUG);

}

void MedSafeTracker::StartTracking()
{
    tracker->RequestStartTracking();
    DelayNSec(2);
}

void MedSafeTracker::StopTracking()
{

     tracker->RequestStopTracking();
     trackerTool->RequestDetachFromTracker( );
}

void MedSafeTracker::ObserveTransformations()
{

    coordSystemAObserver = ObserverType::New();
	std::cout<<"	Observing Transformation Events"<<std::endl;
    coordSystemAObserver->ObserveTransformEventsFrom(trackerTool);

}

void MedSafeTracker::Track()
{
    igstk::PulseGenerator::CheckTimeouts();

    TransformType             transform;
    VectorType                position;

    coordSystemAObserver->Clear();
    trackerTool->RequestGetTransformToParent();
    if (coordSystemAObserver->GotTransform())
    {
      transform = coordSystemAObserver->GetTransform();
      if ( transform.IsValidNow() )
      {
        position = transform.GetTranslation();
        std::cout << "Trackertool :"
          << trackerTool->GetTrackerToolIdentifier()
          << "\t\t  Position = (" << position[0]
        << "," << position[1] << "," << position[2]
        << ")" << std::endl;
      }
      else
      {
        std::cout << "Invalid transform! make sure tool is within tracking volume\n";
      }
    }
}

void MedSafeTracker::DelayNSec(int seconds)
{
  time_t start_time, cur_time;
  time(&start_time);
  do
  {
    time(&cur_time);
  }
  while((cur_time - start_time) < seconds);
}
