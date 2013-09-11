#ifndef MEDSAFETRACKER_H
#define MEDSAFETRACKER_H

#endif // MEDSAFETRACKER_H

#include <iostream>
#include <fstream>
#include <set>
#include <ctime>

#include "itkCommand.h"
#include "igstkLogger.h"
#include "itkStdStreamLogOutput.h"
#include "itkVector.h"
#include "itkVersor.h"

#include "igstkSystemInformation.h"
#include "igstkAscension3DGTracker.h"
#include "igstkAscension3DGTrackerTool.h"
#include "igstkTransform.h"

#include "igstkTransformObserver.h"

class MedSafeTrackerCommand : public itk::Command
{
public:
  typedef  MedSafeTrackerCommand    Self;
  typedef  itk::Command                Superclass;
  typedef itk::SmartPointer<Self>      Pointer;
  itkNewMacro( Self );

protected:
  MedSafeTrackerCommand(){};

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

class MedSafeTracker
{
public:

    typedef igstk::TrackerTool                TrackerToolType;
    typedef igstk::Ascension3DGTrackerTool    AscensionTracker3DGToolType;
	typedef TrackerToolType::TransformType    TransformType;
    typedef ::itk::Vector<double, 3>          VectorType;
    typedef ::itk::Versor<double>             VersorType;
    typedef igstk::TransformObserver          ObserverType;
	typedef igstk::Object::LoggerType		  LoggerType;
	typedef itk::StdStreamLogOutput			  LogOutputType;

    MedSafeTrackerCommand::Pointer			  my_command;
    igstk::Ascension3DGTracker::Pointer		  tracker;
    AscensionTracker3DGToolType::Pointer	  trackerTool;
    ObserverType::Pointer					  coordSystemAObserver;
	LoggerType::Pointer						  logger;
	LogOutputType::Pointer					  logOutput;
	LogOutputType::Pointer					  fileOutput;
	std::ofstream							  loggerFile;


    static MedSafeTracker *New()
    {
        return new MedSafeTracker;
    }

    void InitializeTracker();
    void InitializeTrackerTool(int port);
    void InitializeLogger();
    void StartTracking();
    void StopTracking();
    void ObserveTransformations();
    void Track();

private:

    void DelayNSec(int seconds);

};
