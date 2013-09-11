#include "CropImagesWidget.h"
#include "vtkExtractVOI.h"
#include "vtkBMPWriter.h"


CropImagesWidget::CropImagesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CropImagesWidget)
{
    ui->setupUi(this);
}

CropImagesWidget::~CropImagesWidget()
{
    delete ui;
}

void CropImagesWidget::crop()
{
	int depth = 0;
    
	std::cout<<std::endl;
    if (ui->radioButton_4->isChecked())
    {
        depth = 4;
        if (workWithStack)
        {
            cropStack.reserve(imageStack.size());
            for (uint i = 0; i < imageStack.size(); i++)
            {
				std::cout<<"Cropping image"<<i+1<<std::endl;
                cropStack.push_back(this->cropProbeImage(imageStack.at(i), depth));
            }
        }
        else
        {
            cropImage = this->cropProbeImage(this->image, depth);
        }
    }
    else if (ui->radioButton_5->isChecked())
    {
        depth = 5;
        if (workWithStack)
			for (uint i = 0; i < imageStack.size(); i++){
				std::cout<<"Cropping image"<<i+1<<std::endl;
                cropStack.push_back(this->cropProbeImage(imageStack.at(i), depth));
			}
        else
            cropImage = this->cropProbeImage(this->image, depth);
    }
    else if (ui->radioButton_6->isChecked())
    {
        depth = 6;
        if (workWithStack)
			for (uint i = 0; i < imageStack.size(); i++){
				std::cout<<"Cropping image"<<i+1<<std::endl;
                cropStack.push_back(this->cropProbeImage(imageStack.at(i), depth));
			}
        else
            cropImage = this->cropProbeImage(this->image, depth);
    }
    else if (ui->radioButton_8->isChecked())
    {
        depth = 8;
		
        if (workWithStack)
			for (uint i = 0; i < imageStack.size(); i++){
				std::cout<<"Cropping image"<<i+1<<std::endl;
                cropStack.push_back(this->cropProbeImage(imageStack.at(i), depth));
			}
        else
            cropImage = this->cropProbeImage(this->image, depth);
    }
    else
    {
        QErrorMessage errorMessage(this);
        errorMessage.showMessage("<b>No Depth selected</b> <br /> Select one depth before crop");
        errorMessage.exec();
        return;
    }
    
    // show croped images
    //    MainWindow* w = dynamic_cast<MainWindow*> (this->parentWidget());    
    if (0 != mainWindow)
    {
        if (workWithStack)
        {
            mainWindow->getDisplayWidget()->setAndDisplayMultipleImages(cropStack);
            
            QString str = "crop image stack with depth = ";
            QString num;
            num.setNum(depth);
            mainWindow->addLogText(str + num);
        }
        else
        {
            mainWindow->getDisplayWidget()->setAndDisplayImage(cropImage);
            
            QString str = "crop image with depth = ";
            QString num;
            num.setNum(depth);
            mainWindow->addLogText(str + num);
            
            cropImage = NULL;
        }
    }
}

void CropImagesWidget::save()
{

	vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
	
	QString saveDirectory = QFileDialog::getExistingDirectory(
                this, tr("Choose Directory to Save Images"), QDir::currentPath(), 
				QFileDialog::ShowDirsOnly );


	QString filename;
	std::string str;
	const char * saveFile;

	if(workWithStack){
		for(int i=0; i < cropStack.size(); i++){			
			char imageNumber[4];
			sprintf(imageNumber,"%d",i);
			
			filename = saveDirectory;

			if(i<10)
				filename.append("/IMG000");
			else if(i<100)
				filename.append("/IMG00");
			else if(i<1000)
				filename.append("/IMG0");
			else
				filename.append("/IMG");

			filename.append(imageNumber);
			filename.append(".bmp");

			str = std::string(filename.toAscii().data());
			saveFile = str.c_str();

			writer->SetFileName(saveFile);
			writer->SetInput(cropStack.at(i));
			writer->Write();

		}	
	}else{ 
		filename = saveDirectory;
		filename.append("/IMG0000.bmp");
		
		str = std::string(filename.toAscii().data());
		saveFile = str.c_str();

		writer->SetFileName(saveFile);
		writer->SetInput(cropImage);
		writer->Write();
	}	
}

vtkSmartPointer<vtkImageData> CropImagesWidget::
cropProbeImage(vtkSmartPointer<vtkImageData> image, int depthType)
{
	
    vtkSmartPointer<vtkExtractVOI> extractVOI = vtkSmartPointer<vtkExtractVOI>::New();
    
    
    int* dim = image->GetDimensions();
    
    switch (depthType)
    {
        case 4:
        {
            extractVOI->SetInput(image);
            extractVOI->SetVOI(91, 478, dim[1] - 446, dim[1] - 49, 0, 0);
            extractVOI->Update();
            return extractVOI->GetOutput();
        }
        case 5:
        {
            extractVOI->SetInput(image);
            extractVOI->SetVOI(136, 435, dim[1] - 446, dim[1] - 49, 0, 0);
            extractVOI->Update();
            return extractVOI->GetOutput();
        }
        case 6:
        {
            extractVOI->SetInput(image);
            extractVOI->SetVOI(155, 414, dim[1] - 446, dim[1] - 49, 0, 0);
            extractVOI->Update();
            return extractVOI->GetOutput();
        }
        case 8:
        {
            extractVOI->SetInput(image);
            extractVOI->SetVOI(188, 380, dim[1] - 446, dim[1] - 49, 0, 0);
            extractVOI->Update();
            return extractVOI->GetOutput();
        }
        default:
        {
            //                MainWindow* w = dynamic_cast<MainWindow*> (this->parentWidget());
            //                if (0 != w)
            if (mainWindow != 0)
            {
                //                        w->addLogText("depth not found, nothing made");
                mainWindow->addLogText("depth not found, nothing made");
            }
            return image;
        }
    }
}

void CropImagesWidget::setImage(vtkSmartPointer<vtkImageData> image)
{
    this->workWithStack = false;
    this->image = image;
}


void CropImagesWidget::
setImageStack(std::vector<vtkSmartPointer<vtkImageData> > imagestack)
{
    this->workWithStack = true;
    this->imageStack = imagestack;
}

void CropImagesWidget::setMainWindow(MainWindow* mainwindow)
{
    this->mainWindow = mainwindow;
}