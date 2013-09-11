#include "PolarisTracker.h"

void PolarisTracker::attachTool(int tool)
{
	std::cout<<"	Attaching tool "<<tool<<std::endl;
	trackerTools.at(tool)->RequestAttachToTracker(tracker);
}

void PolarisTracker::attachAllTools()
{
	for(int i=0;i<trackerTools.size();i++){
		std::cout<<"	Attaching tool "<<i<<std::endl;
		trackerTools.at(i)->RequestAttachToTracker(tracker);
	}
}

void PolarisTracker::createToolsObervers(){

	for(int i = 0; i < trackerTools.size(); i++){
		coordSystemAObserver = ObserverType::New();
		coordSystemAObservers.push_back(coordSystemAObserver);
	}

}

void PolarisTracker::detachTool(int tool)
{
	std::cout<<"	Detaching tool "<<tool<<std::endl;
	trackerTools.at(tool)->RequestDetachFromTracker( );
}

void PolarisTracker::detachAllTools()
{
	for(int i = 0; i < trackerTools.size(); i++){
		std::cout<<"	Detaching tool "<<i<<std::endl;
		trackerTools.at(i)->RequestDetachFromTracker( );
	}
}

void PolarisTracker::initializeTracker()
{

    tracker = igstk::PolarisTracker::New();
    //tracker->AddObserver(itk::AnyEvent(), my_command);

	if(loggerOn){
		std::cout<<"	Setting Logger"<<std::endl;
		tracker->SetLogger(logger);
	}

    std::cout<<"	Setting SerialCommunication"<<std::endl;
    tracker->SetCommunication( serialComm );
	tracker->RequestSetFrequency(60);
    std::cout<<"	Opening"<<std::endl;
    tracker->RequestOpen();

}

void PolarisTracker::initializeTrackerTool(std::string romFile)
{

	trackerTool = PolarisToolType::New();	

    //trackerTool->AddObserver( itk::AnyEvent(), my_command);

	if(loggerOn){
		std::cout<<"	Setting Logger"<<std::endl;
		trackerTool->SetLogger(logger);
	}

    std::cout<<"	Requesting Wireless Tool"<<std::endl;
    trackerTool->RequestSelectWirelessTrackerTool();
    std::cout <<"	Setting ROM file: " << romFile << std::endl;
    trackerTool->RequestSetSROMFileName( romFile );
    std::cout<<"	Configuring"<<std::endl;
    trackerTool->RequestConfigure();

	trackerTools.push_back(trackerTool);

}

void PolarisTracker::initializeLogger()
{
    logger = LoggerType::New();
    logOutput = LogOutputType::New();

    std::ofstream filename("Logger.txt");
    std::cout<<"	Logger output saved here: "<<filename<<std::endl;
    logOutput->SetStream(filename);
    std::cout<<"	Adding file"<<std::endl;
    logger->AddLogOutput( logOutput );
    std::cout<<"	Setting priority"<<std::endl;

}

void PolarisTracker::initializeSerialCommunication(int portNumber)
{

    my_command = PolarisTrackerCommand::New();
	serialComm = igstk::SerialCommunication::New();

    //serialComm->AddObserver( itk::AnyEvent(), my_command);
	
	if(loggerOn){
		std::cout<<"	Setting Logger"<<std::endl;
		serialComm->SetLogger( logger );
	}

	polarisPortNumber = PortNumberType(portNumber);
	std::cout<<"	Setting PortNumber"<<std::endl;
    serialComm->SetPortNumber( polarisPortNumber );
	std::cout<<"	Setting Parity"<<std::endl;
    serialComm->SetParity( igstk::SerialCommunication::NoParity );
	std::cout<<"	Setting Boudrate"<<std::endl;
    serialComm->SetBaudRate( igstk::SerialCommunication::BaudRate115200);
	std::cout<<"	Setting DataBits"<<std::endl;
    serialComm->SetDataBits( igstk::SerialCommunication::DataBits8 );
	std::cout<<"	Setting StopBits"<<std::endl;
    serialComm->SetStopBits( igstk::SerialCommunication::StopBits1 );
	std::cout<<"	Setting HandShake"<<std::endl;
    serialComm->SetHardwareHandshake( igstk::SerialCommunication::HandshakeOff );
	std::cout<<"	Setting RecordedStreamFile"<<std::endl;
    serialComm->SetCaptureFileName( "RecordedStreamByPolarisTrackerTest.txt" );
    serialComm->SetCapture( true );

	serialComm->OpenCommunication();

}

void PolarisTracker::startTracking()
{
    tracker->RequestStartTracking();
    delayNSec(2);
}

void PolarisTracker::stopTracking()
{
	 delayNSec(2);
     tracker->RequestStopTracking();
}


void PolarisTracker::observeAllToolsTransformations()
{

	for(int i = 0; i < trackerTools.size(); i++){
		std::cout<<"	Observing Transformation Events tool "<<i+1<<std::endl;
		coordSystemAObservers.at(i)->ObserveTransformEventsFrom(trackerTools.at(i));
		coordSystemAObservers.at(i)->Clear();
	}

}



void PolarisTracker::track(int tool)
{
    igstk::PulseGenerator::CheckTimeouts();

    TransformType             transform;
    VectorType                position;

    coordSystemAObservers.at(tool)->Clear();
    trackerTools.at(tool)->RequestGetTransformToParent();
    if (coordSystemAObservers.at(tool)->GotTransform())
    {
      transform = coordSystemAObservers.at(tool)->GetTransform();
      if ( transform.IsValidNow() )
      {
        position = transform.GetTranslation();
        std::cout << "Trackertool :"
          << trackerTools.at(tool)->GetTrackerToolIdentifier()
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

void PolarisTracker::delayNSec(int n)
{
  time_t start_time, cur_time;
  time(&start_time);
  do
  {
    time(&cur_time);
  }
  while((cur_time - start_time) < n);
}

int PolarisTracker::getNumberofTools()
{
	return trackerTools.size();
}

igstk::PolarisTracker::Pointer PolarisTracker::getTracker()
{
	return this->tracker;
}

typedef igstk::PolarisTrackerTool                   PolarisToolType;
vector<PolarisToolType::Pointer> PolarisTracker::getTools()
{
	return this->trackerTools;
}

vector<igstk::TransformObserver::Pointer> PolarisTracker::getObservers()
{
	return this->coordSystemAObservers;
}

void PolarisTracker::setLoggerOn(bool loggerOn)
{
	this->loggerOn = loggerOn;
}