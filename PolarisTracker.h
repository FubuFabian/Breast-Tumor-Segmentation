#ifndef POLARISTRACKER_H
#define POLARISTRACKER_H

#endif // POLARISTRACKER_H

#include <iostream>
#include <fstream>
#include <set>
#include <ctime>
#include <vector>

#include "itkCommand.h"
#include "igstkLogger.h"
#include "itkStdStreamLogOutput.h"
#include "itkVector.h"
#include "itkVersor.h"

#include "igstkSystemInformation.h"
#include "igstkSerialCommunication.h"
#include "igstkPolarisTracker.h"
#include "igstkPolarisTrackerTool.h"
#include "igstkTransform.h"
#include "igstkTransformObserver.h"

using namespace std;

class PolarisTrackerCommand : public itk::Command
{
public:
  typedef  PolarisTrackerCommand   Self;
  typedef  itk::Command                Superclass;
  typedef itk::SmartPointer<Self>      Pointer;
  itkNewMacro( Self );
protected:
  PolarisTrackerCommand() {};

public:
  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
    Execute( (const itk::Object *)caller, event);
    }

  void Execute(const itk::Object * object, const itk::EventObject & event)
    {
    // don't print "CompletedEvent", only print interesting events
    if (!igstk::CompletedEvent().CheckEvent(&event) &&
        !itk::DeleteEvent().CheckEvent(&event) )
      {
      std::cout << event.GetEventName() << std::endl;
      }
    }
};

/*!
  This class contains the basic steps to configure and initialize a Polaris Optic Tracker
*/

class PolarisTracker
{
public:

	typedef igstk::TrackerTool                          TrackerToolType;
    typedef igstk::PolarisTrackerTool                   PolarisToolType;
    typedef TrackerToolType::TransformType              TransformType;
    typedef ::itk::Vector<double, 3>                    VectorType;
    typedef ::itk::Versor<double>                       VersorType;
    typedef igstk::TransformObserver                    ObserverType;
    typedef igstk::Object::LoggerType                   LoggerType;
    typedef itk::StdStreamLogOutput                     LogOutputType;
    typedef igstk::SerialCommunication::PortNumberType  PortNumberType;
	
    ///Constructor of the class
    static PolarisTracker *New()
    {
		return new PolarisTracker;
    }

	
	 /** 
	 * \brief Attach one tool to the tracker
	 * \param[in] Tool number*/
	void attachTool(int);

	/** \brief Attach all tool to tracker*/
	void attachAllTools();

	/** \brief Create the observers for all tools*/
	void createToolsObervers();

	/** \brief Detach one tool from tracker
	* \param[in] Tool number*/
	void detachTool(int);

	/** \brief Detach all tools from tracker*/
	void detachAllTools();

	/** \brief Initialize tracker*/
    void initializeTracker();

	/** \brief Initilize one tracker tool
	* \param[in] Rom file*/
	void initializeTrackerTool(std::string);

	/** \brief Initilize Logger for the tracke*/
    void initializeLogger();

	/** \brief Initilize serial communication
	* \param[in] number of port*/
    void initializeSerialCommunication(int);

	/** \brief Return number of tools attached to tracker
	* \param[out] number of tools*/
    int	 getNumberofTools();

	/** \brief Start observers*/
	void observeAllToolsTransformations();
	
	/** \brief Start Tracking*/
	void startTracking();

	/** \brief Stop Tracking*/
    void stopTracking(); 

	/** \brief Track one tool
	* \param[in] number of tool*/
    void track(int);

	/** \brief Set logger*/
	void setLoggerOn(bool);	
	
	/** \brief Returns tracker
	* \param[out] Pointer to the Polaris Tracker*/
	igstk::PolarisTracker::Pointer getTracker();

	/** \brief Return all tools
	* \param[out] Vector with all the tools*/
	vector<PolarisToolType::Pointer> getTools();

	/** \brief Return all observers*/
	vector<igstk::TransformObserver::Pointer> getObservers();
	
	bool loggerOn; ///<Logger flag

private:

    PolarisTrackerCommand::Pointer			  my_command; ///<Command for tracker events
    igstk::PolarisTracker::Pointer            tracker; ///<Polaris tracker object
    LoggerType::Pointer						  logger; ///<Logger for the Polaris tracker object
    LogOutputType::Pointer					  logOutput; ///<Stream for the logger
    std::ofstream							  loggerFile; ///<File to save logger
    igstk::SerialCommunication::Pointer       serialComm; ///<Serial communuication with tracker
    PortNumberType                            polarisPortNumber; ///<Port number for the tracker
	vector<PolarisToolType::Pointer>		  trackerTools; ///<Vector with all tools
	vector<ObserverType::Pointer>			  coordSystemAObservers; ///<Vector with all observers
	PolarisToolType::Pointer 				  trackerTool; ///<One Tool
	ObserverType::Pointer					  coordSystemAObserver; ///<One observer
	
	/** \brief Delay n seconds
	* \param[in] number of seconds*/
    void delayNSec(int n);

};


