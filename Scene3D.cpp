#include "Scene3D.h"
#include "PolarisTracker.h"
#include "VTKThreeViews.h"
#include "ChangeVolumePropertiesWidget.h"

#include <QtTest/QTest>
#include <QErrorMessage>
#include <QString>
#include <QFile>
#include <QTextStream>

#include <itkStdStreamLogOutput.h>

#include <vtkSmartPointer.h>
#include <vtkMetaImageReader.h>
#include <vtkImageData.h>
#include <vtkImageChangeInformation.h>

#include <igstkLogger.h>



void Scene3D::configTracker(std::string referenceToolFilename, std::string ultrasoundProbeFilename, 
							std::string needleFilename, std::string pointerFilename, QString probeCalibrationFilename,
							 QString needleCalibrationFilename,  QString pointerCalibrationFilename, int port)
{

	PolarisTracker* polarisTracker = PolarisTracker::New();
	polarisTracker->setLoggerOn(false);
	std::cout<<"-Initializing SerialCommunication"<<std::endl;
	polarisTracker->initializeSerialCommunication(port);
	std::cout<<"-Initializing Tracker"<<std::endl;
	polarisTracker->initializeTracker();
	std::cout<<"-Initializing Reference Tracker Tool"<<std::endl;
	polarisTracker->initializeTrackerTool(referenceToolFilename);
	std::cout<<"-Initializing Ulatrasound Probe Tracker Tool"<<std::endl;
	polarisTracker->initializeTrackerTool(ultrasoundProbeFilename);
	std::cout<<"-Initializing Needle Tracker Tool"<<std::endl;
	polarisTracker->initializeTrackerTool(needleFilename);
	std::cout<<"-Initializing Pointer Tracker Tool"<<std::endl;
	polarisTracker->initializeTrackerTool(pointerFilename);
	std::cout<<"-Attaching all tools"<<std::endl;
	polarisTracker->attachAllTools();
	std::cout<<"-Creating observers for all tools"<<std::endl;
	polarisTracker->createToolsObervers();

	tracker = polarisTracker->getTracker();

	referenceTool = polarisTracker->getTools().at(0);
	ultrasoundProbeTool = polarisTracker->getTools().at(1);
	needleTool = polarisTracker->getTools().at(2);
	pointerTool = polarisTracker->getTools().at(3);

	coordSystemAObserverReferenceTool = polarisTracker->getObservers().at(0);
	coordSystemAObserverReferenceTool->ObserveTransformEventsFrom(referenceTool);
	coordSystemAObserverUltrasoundProbe = polarisTracker->getObservers().at(1);
	coordSystemAObserverUltrasoundProbe->ObserveTransformEventsFrom(ultrasoundProbeTool);
	coordSystemAObserverNeedle = polarisTracker->getObservers().at(2);
	coordSystemAObserverNeedle->ObserveTransformEventsFrom(needleTool);
	coordSystemAObserverPointer = polarisTracker->getObservers().at(3);
	coordSystemAObserverPointer->ObserveTransformEventsFrom(pointerTool);

	std::cout<<std::endl<<"-Reference Tracker Tool ID: "<<referenceTool->GetTrackerToolIdentifier()<<std::endl;
	std::cout<<"-Ultrasound Tracker Probe Tool ID: "<<ultrasoundProbeTool->GetTrackerToolIdentifier()<<std::endl;
	std::cout<<"-Needle Tracker Tool ID: "<<needleTool->GetTrackerToolIdentifier()<<std::endl;
	std::cout<<"-Pointer Tracker Tool ID: "<<pointerTool->GetTrackerToolIdentifier()<<std::endl;


	std::cout<<std::endl;
	std::cout<<"Loading Probe Calibration Data"<<std::endl;

	std::vector<double> probeCalibrationData;
	probeCalibrationData.reserve(8);

	if (!probeCalibrationFilename.isEmpty())
    {
        QFile file(probeCalibrationFilename);
        if (!file.open(QIODevice::ReadOnly))
           return;

		QTextStream stream(&file);
        QString line;

		for(int i=0;i<8;i++)
        {
			line = stream.readLine();       
			probeCalibrationData.push_back(line.toDouble());
        }
		 file.close(); 	
	}else
	{
		 std::cout<<"No Probe Calibration Data Loaded"<<std::endl;
	}

	std::cout<<std::endl;
	std::cout<<"Loading Needle Calibration Data"<<std::endl;

	std::vector<double> needleCalibrationData;
	needleCalibrationData.reserve(3);

	if (!needleCalibrationFilename.isEmpty())
    {
        QFile file(needleCalibrationFilename);
        if (!file.open(QIODevice::ReadOnly))
           return;

		QTextStream stream(&file);
        QString line;

		for(int i=0;i<8;i++)
        {
			line = stream.readLine();       
			needleCalibrationData.push_back(line.toDouble());
        }
		 file.close(); 	
	}else
	{
		 std::cout<<"No Needle Calibration Data Loaded"<<std::endl;
	}

	std::cout<<std::endl;
	std::cout<<"Loading Pointer Calibration Data"<<std::endl;

	std::vector<double> pointerCalibrationData;
	pointerCalibrationData.reserve(8);

	if (!pointerCalibrationFilename.isEmpty())
    {
        QFile file(pointerCalibrationFilename);
        if (!file.open(QIODevice::ReadOnly))
           return;

		QTextStream stream(&file);
        QString line;

		for(int i=0;i<8;i++)
        {
			line = stream.readLine();       
			pointerCalibrationData.push_back(line.toDouble());
        }
		 file.close();	
	}else
	{
		 std::cout<<"No Pointer Calibration Data Loaded"<<std::endl;
	}

	igstk::Transform::ErrorType errorValue;
	errorValue=10;

	double validityTimeInMilliseconds;
	validityTimeInMilliseconds = igstk::TimeStamp::GetLongestPossibleTime();

	igstk::Transform::VectorType probeTranslation;
	igstk::Transform probeTransform;
	igstk::Transform::VersorType probeRotation;

	probeTranslation[0] = probeCalibrationData[0];
	probeTranslation[1] = probeCalibrationData[1];
	probeTranslation[2] = probeCalibrationData[2];
    probeRotation.Set(probeCalibrationData[3], probeCalibrationData[4], probeCalibrationData[5], 0.0);
	probeTransform.SetTranslationAndRotation(probeTranslation, probeRotation, errorValue, validityTimeInMilliseconds);

	igstk::Transform::VectorType needleTranslation;
	igstk::Transform::VersorType needleRotation;
	igstk::Transform needleTransform;

	needleTranslation[0] = needleCalibrationData[0];
	needleTranslation[1] = needleCalibrationData[1];
	needleTranslation[2] = needleCalibrationData[2];
	needleRotation.Set(0.0, 0.0, 0.0, 1.0 );
	needleTransform.SetTranslationAndRotation(needleTranslation, needleRotation, errorValue, validityTimeInMilliseconds );

	igstk::Transform::VectorType pointerTranslation;
	igstk::Transform::VersorType pointerRotation;
	igstk::Transform pointerTransform;

	pointerTranslation[0] = pointerCalibrationData[0];
	pointerTranslation[1] = pointerCalibrationData[1];
	pointerTranslation[2] = pointerCalibrationData[2];
	pointerRotation.Set(0.0, 0.0, 0.0, 1.0 );
	pointerTransform.SetTranslationAndRotation(pointerTranslation, pointerRotation, errorValue, validityTimeInMilliseconds);

	identityTransform.SetToIdentity(igstk::TimeStamp::GetLongestPossibleTime());


	referenceAxes->RequestSetTransformAndParent(identityTransform, referenceTool);
	usProbe->RequestSetTransformAndParent(probeTransform, ultrasoundProbeTool);
	needle->RequestSetTransformAndParent(needleTransform, needleTool);
	pointer->RequestSetTransformAndParent(pointerTransform, pointerTool);

	scene3DWidget->SetTracker(tracker);

	configTrackerFlag = true;
}

void Scene3D::startTracking()
{
	if(configTrackerFlag)
	{
		scene3DWidget->View1->RequestSetTransformAndParent(identityTransform, referenceTool);
		scene3DWidget->View1->RequestResetCamera();
		scene3DWidget->View1->SetCameraFocalPoint( 0.0, 290.0 ,150.0 );
		scene3DWidget->View1->SetCameraPosition( 700.0, -150.0 , 150.0 );
		scene3DWidget->View1->SetCameraViewUp( 0.0, 0.0 , 1.0 );
		scene3DWidget->View1->SetCameraClippingRange( 10.0 , 2000.0 );
		scene3DWidget->View1->SetCameraParallelProjection( false );
		scene3DWidget->View1->SetRendererBackgroundColor(183.0/255.0,197.0/255.0,253.0/255.0);

		typedef ::itk::Vector<double, 3>    VectorType;
		typedef ::itk::Versor<double>       VersorType;

		std::vector<double> coords;
		coords.reserve(6);

		while( !scene3DWidget->HasQuitted() )
		{
			coords.clear();
			QTest::qWait(0.001);
			igstk::PulseGenerator::CheckTimeouts();

			TransformType             usTransform;
			TransformType             needleTransform;

			VectorType                usPosition;
			VectorType                needlePosition;

  
			coordSystemAObserverReferenceTool->Clear();
			referenceTool->RequestGetTransformToParent();
			coordSystemAObserverUltrasoundProbe->Clear();
			ultrasoundProbeTool->RequestGetTransformToParent();
			coordSystemAObserverNeedle->Clear();
			needleTool->RequestGetTransformToParent();
			coordSystemAObserverPointer->Clear();
			pointerTool->RequestGetTransformToParent();


			if (coordSystemAObserverReferenceTool->GotTransform())
			{
				usTransform = coordSystemAObserverUltrasoundProbe->GetTransform();
				usPosition = usTransform.GetTranslation();
				
				coords.push_back(usPosition[0]);
				coords.push_back(usPosition[1]);
				coords.push_back(usPosition[2]);

				needleTransform = coordSystemAObserverNeedle->GetTransform();
				needlePosition = needleTransform.GetTranslation();

				coords.push_back(needlePosition[0]);
				coords.push_back(needlePosition[1]);
				coords.push_back(needlePosition[2]);

				scene3DWidget->setCoords(coords);
			}
		}

		tracker->RequestClose();
		delete scene3DWidget;
	}
	else{
		QErrorMessage errorMessage;
        errorMessage.showMessage(
            "Tracker is not configure, </ br> please configure tracker first");
        errorMessage.exec();
	}
}

void Scene3D::initLogger()
{
	if(configTrackerFlag)
	{
		typedef igstk::Object::LoggerType LoggerType;

		LoggerType::Pointer logger = LoggerType::New();
		itk::StdStreamLogOutput::Pointer fileOutput = itk::StdStreamLogOutput::New();

		std::ofstream ofs( "log.txt" );
		fileOutput->SetStream(ofs);
		logger->AddLogOutput( fileOutput );

		scene3DWidget->qtDisplay1->SetLogger( logger );
		tracker->SetLogger(logger);
	}else{
		QErrorMessage errorMessage;
        errorMessage.showMessage(
            "Tracker is not configure, </ br> please configure tracker first");
        errorMessage.exec();
	}
}


void Scene3D::init3DScene()
{
	igstk::RealTimeClock::Initialize();
    scene3DWidget = new Scene3DWidget();

	scene3DWidget->setScene3D(this);

    referenceAxes = igstk::AxesObject::New();
	usProbe = igstk::USProbeObject::New();
    needle = igstk::NeedleObject::New();
	pointer = igstk::PolarisPointerObject::New();
	usVolume = igstk::USImageObject::New();

    referenceAxes->SetSize(100,100,100);
	
	referenceAxesRepresentation = igstk::AxesObjectRepresentation::New();
	referenceAxesRepresentation->RequestSetAxesObject(referenceAxes);

	usProbeRepresentation = igstk::USProbeObjectRepresentation::New();
	usProbeRepresentation->RequestSetUSProbeObject(usProbe);

	needleRepresentation = igstk::NeedleObjectRepresentation::New();
	needleRepresentation->RequestSetNeedleObject(needle);

	pointerRepresentation = igstk::PolarisPointerObjectRepresentation::New();
	pointerRepresentation->RequestSetPolarisPointerObject(pointer);


	scene3DWidget->View1->RequestAddObject(referenceAxesRepresentation);
	scene3DWidget->View1->RequestAddObject(usProbeRepresentation);
	scene3DWidget->View1->RequestAddObject(needleRepresentation);
	scene3DWidget->View1->RequestAddObject(pointerRepresentation);

	scene3DWidget->qtDisplay1->RequestEnableInteractions();

	scene3DWidget->Show();

	configTrackerFlag = false;
	volumeLoaded = false;
	fourViews = false;
}

void Scene3D::addVolumeToScene(std::string volumeFilename)
{
	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(volumeFilename.c_str());
    reader->Update();

	vtkSmartPointer<vtkImageData> volumeData = reader->GetOutput();
	double * usVolumePosition = volumeData->GetOrigin();

	VTKThreeViews * threeViews = new VTKThreeViews();
        threeViews->setVolumeData(volumeData);
	threeViews->show();

	vtkSmartPointer<vtkImageChangeInformation> changeInformation = vtkSmartPointer<vtkImageChangeInformation>::New();
	changeInformation->SetInput(volumeData);
	changeInformation->SetOutputOrigin(0,0,0);
	changeInformation->Update();
	usVolume->volumeData =  volumeData;

	usVolumeRepresentation = igstk::ImageSpatialObjectVolumeRepresentation<igstk::USImageObject>::New();
	usVolumeRepresentation->RequestSetImageSpatialObject(usVolume);

	ChangeVolumePropertiesWidget * propertiesChanger = new ChangeVolumePropertiesWidget();
	propertiesChanger->setigstkImageSpatialObjectVolumeRepresentation(usVolumeRepresentation);
	propertiesChanger->show();

	igstk::Transform::VectorType usVolumeTranslation;
	igstk::Transform::VersorType usVolumeRotation;
	igstk::Transform usVolumeTransform;

	igstk::Transform::ErrorType errorValue;
	errorValue=10;

	double validityTimeInMilliseconds;
	validityTimeInMilliseconds = igstk::TimeStamp::GetLongestPossibleTime();
	std::cout<<usVolumePosition[0]<<","<<usVolumePosition[1]<<","<<usVolumePosition[2]<<std::endl;
	usVolumeTranslation[0] = usVolumePosition[0];
	usVolumeTranslation[1] = usVolumePosition[1];
	usVolumeTranslation[2] = usVolumePosition[2];
	usVolumeRotation.Set(0.0, 0.0, 0.0, 1.0 );
	usVolumeTransform.SetTranslationAndRotation(usVolumeTranslation, usVolumeRotation, errorValue, validityTimeInMilliseconds);

	scene3DWidget->View1->RequestAddObject(usVolumeRepresentation);

	if (fourViews){
		scene3DWidget->View2->RequestAddObject(usVolumeRepresentation);
		scene3DWidget->View3->RequestAddObject(usVolumeRepresentation);
		scene3DWidget->View4->RequestAddObject(usVolumeRepresentation);
	}

	usVolume->RequestSetTransformAndParent(usVolumeTransform, referenceTool);

	volumeLoaded = true;

}

void Scene3D::addFourViews()
{
	scene3DWidget->View2->RequestSetTransformAndParent(identityTransform, referenceTool);
	scene3DWidget->View2->RequestResetCamera();
	scene3DWidget->View2->SetCameraFocalPoint( 290.0, 0.0 ,150.0 );
	scene3DWidget->View2->SetCameraPosition( -150.0 , 700.0 , 150.0 );
	scene3DWidget->View2->SetCameraViewUp( 0.0, 0.0 , 1.0 );
	scene3DWidget->View2->SetCameraClippingRange( 10.0 , 2000.0 );
	scene3DWidget->View2->SetCameraParallelProjection( false );
	scene3DWidget->View2->SetRendererBackgroundColor(183.0/255.0,197.0/255.0,253.0/255.0);

	scene3DWidget->View3->RequestSetTransformAndParent(identityTransform, referenceTool);
	scene3DWidget->View3->RequestResetCamera();
	scene3DWidget->View3->SetCameraFocalPoint( 110.0, 110.0 , 0.0 );
	scene3DWidget->View3->SetCameraPosition( 80.0, 80.0 , 700.0 );
	scene3DWidget->View3->SetCameraViewUp( 0.0, 0.0 , 0.5 );
	scene3DWidget->View3->SetCameraClippingRange( 10.0 , 2000.0 );
	scene3DWidget->View3->SetCameraParallelProjection( false );
	scene3DWidget->View3->SetRendererBackgroundColor(183.0/255.0,197.0/255.0,253.0/255.0);

	scene3DWidget->View4->RequestSetTransformAndParent(identityTransform, referenceTool);
	scene3DWidget->View4->RequestResetCamera();
	scene3DWidget->View4->SetCameraFocalPoint( 110.0, 110.0 , 0.0 );
	scene3DWidget->View4->SetCameraPosition( 80.0, 80.0 , -700.0 );
	scene3DWidget->View4->SetCameraViewUp( 0.0, 0.0 , 0.5 );
	scene3DWidget->View4->SetCameraClippingRange( 10.0 , 2000.0 );
	scene3DWidget->View4->SetCameraParallelProjection( false );
	scene3DWidget->View4->SetRendererBackgroundColor(183.0/255.0,197.0/255.0,253.0/255.0);

	scene3DWidget->View2->RequestAddObject(referenceAxesRepresentation->Copy());
	scene3DWidget->View2->RequestAddObject(usProbeRepresentation->Copy());
	scene3DWidget->View2->RequestAddObject(needleRepresentation->Copy());
	scene3DWidget->View2->RequestAddObject(pointerRepresentation->Copy());

	scene3DWidget->View3->RequestAddObject(referenceAxesRepresentation->Copy());
	scene3DWidget->View3->RequestAddObject(usProbeRepresentation->Copy());
	scene3DWidget->View3->RequestAddObject(needleRepresentation->Copy());
	scene3DWidget->View3->RequestAddObject(pointerRepresentation->Copy());

	scene3DWidget->View4->RequestAddObject(referenceAxesRepresentation->Copy());
	scene3DWidget->View4->RequestAddObject(usProbeRepresentation->Copy());
	scene3DWidget->View4->RequestAddObject(needleRepresentation->Copy());
	scene3DWidget->View4->RequestAddObject(pointerRepresentation->Copy());

	scene3DWidget->qtDisplay2->RequestEnableInteractions();
	scene3DWidget->qtDisplay3->RequestEnableInteractions();
	scene3DWidget->qtDisplay4->RequestEnableInteractions();

	if (volumeLoaded){
		scene3DWidget->View2->RequestAddObject(usVolumeRepresentation);
		scene3DWidget->View3->RequestAddObject(usVolumeRepresentation);
		scene3DWidget->View4->RequestAddObject(usVolumeRepresentation);
	}

	fourViews = true;

}

void Scene3D::removeFourViews()
{
	fourViews = false;
}
