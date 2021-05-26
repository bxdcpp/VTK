//#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL)
//VTK_MODULE_INIT(vtkInteractionStyle);
//VTK_MODULE_INIT(vtkRenderingFreeType);
//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)

//#include <vtkSmartPointer.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkInteractorStyleTrackballCamera.h>
//#include <vtkDICOMImageReader.h>
//#include <vtkImageShiftScale.h>
//#include <vtkGPUVolumeRayCastMapper.h>
//#include <vtkVolumeProperty.h>
//#include <vtkPiecewiseFunction.h>
//#include <vtkColorTransferFunction.h>
//#include <vtkVolume.h>
//#include <vtkMultiVolume.h>
//
//int main(int argc, char* argv[])
//{
//
//	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
//	reader->SetDirectoryName("C:/Users/Bxd/Desktop/testhead/3-5mm bone");
//	reader->Update();
//
//
//	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
//	volumeMapper->SetInputData(reader->GetOutput());
//	volumeMapper->SetBlendModeToComposite();
//
//	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
//	volumeProperty->SetInterpolationTypeToLinear();
//	volumeProperty->ShadeOn();//阴影开关，打开下面的设置才有效
//	volumeProperty->SetAmbient(0.4);
//	volumeProperty->SetDiffuse(0.6);  //漫反射
//	volumeProperty->SetSpecular(0.2); //镜面反射
//
//	//设置不透明度
//	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
//#if 0
//	compositeOpacity->AddPoint(-200, 0.00);
//	compositeOpacity->AddPoint(-50, 0.10);
//	compositeOpacity->AddPoint(90, 0.40);
//	compositeOpacity->AddPoint(500, 0.60);
//#endif
//
//
//	compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0);
//	compositeOpacity->AddPoint(-16, 0, .49, .61);
//	compositeOpacity->AddPoint(641, .72, .5, 0.0);
//	compositeOpacity->AddPoint(3071, .71, 0.5, 0.0);
//
//	volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数
//	//设置梯度不透明属性
//	//vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
//	//volumeGradientOpacity->AddPoint(10, 0.0);
//	//volumeGradientOpacity->AddPoint(70, 0.3);
//	//volumeGradientOpacity->AddPoint(90, 0.5);
//	//volumeGradientOpacity->AddPoint(500, 1.0);
//	//volumeProperty->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比
//
//	//设置颜色属性
//	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
//#if 0
//	color->AddRGBPoint(-400.000, 0.9, 0.8, 0.50);
//	color->AddRGBPoint(-90.00, 0.9, 0.8, 0.50);
//	color->AddRGBPoint(0.00, 8.00, 0.00, 0.00);
//	color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
//	color->AddRGBPoint(500.0, 1.00, 1.00, 1.00);
//#endif
//
//
//	color->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
//	color->AddRGBPoint(-16, 0.73, 0.25, 0.30, 0.49, .61);
//	color->AddRGBPoint(641, .90, .82, .56, .5, 0.0);
//	color->AddRGBPoint(3071, 1, 1, 1, .5, 0.0);
//
//
//	volumeProperty->SetColor(color);
//
//	//类似Actor
//	vtkSmartPointer<vtkMultiVolume> MultiVolumeActor = vtkSmartPointer<vtkMultiVolume>::New();
//	MultiVolumeActor->SetMapper(volumeMapper);
//	MultiVolumeActor->SetProperty(volumeProperty);
//	MultiVolumeActor->add
//
//
//	//vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
//	//volume->SetMapper(volumeMapper);
//	//volume->SetProperty(volumeProperty);
//
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	renderer->SetBackground(0, 0, 0.25);
//	renderer->AddVolume(volume);
//
//	vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
//	rw->AddRenderer(renderer);
//	rw->Render();
//
//	vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	rwi->SetRenderWindow(rw);
//
//	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
//	rwi->SetInteractorStyle(style);
//
//	rwi->Initialize();
//	rwi->Start();
//
//	return 0;
//}

#if 0

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestGPURayCastMultiVolumeOverlapping.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * Tests rendering 3 overlapping volumes as inputs in vtkGPUVolumeRCMapper
 * vtkMultiVolume.
 */
#include "vtkAxesActor.h"
#include "vtkCamera.h"
#include "vtkColorTransferFunction.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkImageResample.h"
#include "vtkImageResize.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkMultiVolume.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTestUtilities.h"
#include "vtkVolume16Reader.h"
#include "vtkVolumeProperty.h"
#include "vtkXMLImageDataReader.h"

#include "vtkAbstractMapper.h"
#include "vtkImageData.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"

#include "vtkMath.h"
#include <chrono>
#include <vtkDICOMImageReader.h>
#include <vtkImageChangeInformation.h>

namespace
{
    class MoveRotateCommand : public vtkCommand
    {
    public:
        static MoveRotateCommand* New() { return new MoveRotateCommand; }

        void Execute(vtkObject* caller, unsigned long eventId, void* /*data*/) override
        {
            switch (eventId)
            {
            case vtkCommand::KeyPressEvent:
            {
                auto interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
                const std::string key = interactor->GetKeySym();

                double times[3] = { 0, 0, 0 };
                double timesAngle[3] = { 0, 0, 0 };
                // Translations
                if (key == "Left")
                {
                    times[0] = -1;
                }
                else if (key == "Right")
                {
                    times[0] = 1;
                }
                else if (key == "Down")
                {
                    times[1] = -1;
                }
                else if (key == "Up")
                {
                    times[1] = 1;
                }
                else if (key == "n")
                {
                    times[2] = -1;
                }
                else if (key == "m")
                {
                    times[2] = 1;
                }
                // Rotations
                else if (key == "a")
                {
                    timesAngle[0] = -1;
                }
                else if (key == "s")
                {
                    timesAngle[0] = 1;
                }
                else if (key == "z")
                {
                    timesAngle[1] = -1;
                }
                else if (key == "x")
                {
                    timesAngle[1] = 1;
                }
                else if (key == "c")
                {
                    timesAngle[2] = -1;
                }
                else if (key == "v")
                {
                    timesAngle[2] = 1;
                }
                else
                    break;

                const double deltaX = times[0] * Delta;
                const double deltaY = times[1] * Delta;
                const double deltaZ = times[2] * Delta;
                const auto pos = this->Volume->GetPosition();
                this->Volume->SetPosition(pos[0] + deltaX, pos[1] + deltaY, pos[2] + deltaZ);

                const double rotX = timesAngle[0] * DeltaAngle;
                this->Volume->RotateX(rotX);
                const double rotY = timesAngle[1] * DeltaAngle;
                this->Volume->RotateY(rotY);
                const double rotZ = timesAngle[2] * DeltaAngle;
                this->Volume->RotateZ(rotZ);
            }
            break;
            }

            this->RenderWindow->Render();
        };

        void SetVolume(vtkProp3D* vol) { this->Volume = vol; }

        vtkRenderWindow* RenderWindow = nullptr;
        vtkProp3D* Volume = nullptr;
        double Delta = 10.0;
        double DeltaAngle = 5.0;
    };
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // Load data
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName("C:/Users/Bxd/Desktop/testhead/1");
	reader->Update();

    vtkSmartPointer<vtkDICOMImageReader> reader_MR = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader_MR->SetDirectoryName("C:/Users/Bxd/Desktop/testhead/2");
    reader_MR->Update();



    // Geometry
    vtkNew<vtkConeSource> coneSource;
    coneSource->SetRadius(30.);
    coneSource->SetHeight(50.);
    coneSource->SetResolution(40);
    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(coneSource->GetOutputPort());
    vtkNew<vtkActor> cone;
    cone->SetMapper(coneMapper);
    cone->RotateX(35.);
    cone->RotateY(-245.);
    cone->SetPosition(160., 90., -30.);

    vtkNew<vtkAxesActor> axis;
    axis->SetTotalLength(100., 100., 100.);
    axis->SetNormalizedTipLength(0.1, 0.1, 0.1);
    axis->SetNormalizedShaftLength(1., 1., 1.);
    axis->AxisLabelsOff();
    axis->SetConeRadius(0.5);

    // Volume 0 (upsampled headmr)
    // ---------------------------
    vtkNew<vtkImageResize> headmrSource;
    headmrSource->SetInputConnection(reader->GetOutputPort());
    headmrSource->SetResizeMethodToOutputDimensions();
    headmrSource->SetOutputDimensions(128, 128, 128);
    headmrSource->Update();

    vtkNew<vtkColorTransferFunction> ctf;
    ctf->AddRGBPoint(0, 0.0, 0.0, 0.0);
    ctf->AddRGBPoint(500, 1.0, 0.5, 0.3);
    ctf->AddRGBPoint(1000, 1.0, 0.5, 0.3);
    ctf->AddRGBPoint(1150, 1.0, 1.0, 0.9);

    vtkNew<vtkPiecewiseFunction> pf;
    pf->AddPoint(0, 0.00);
    pf->AddPoint(500, 0.15);
    pf->AddPoint(1000, 0.15);
    pf->AddPoint(1150, 0.85);

    vtkNew<vtkPiecewiseFunction> gf;
    gf->AddPoint(0, 0.0);
    gf->AddPoint(50, 0.5);
    gf->AddPoint(100, 0.7);

    vtkNew<vtkVolume> vol;
    vol->GetProperty()->SetScalarOpacity(pf);
    vol->GetProperty()->SetColor(ctf);
   vol->GetProperty()->SetGradientOpacity(gf);
    vol->GetProperty()->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
    vol->GetProperty()->ShadeOn();
    vol->SetVisibility(false);

    // Volume 1 (vase)
    // -----------------------------
    vtkNew<vtkColorTransferFunction> ctf1;
    /*ctf1->AddRGBPoint(0, 0.0, 0.0, 0.0);
    ctf1->AddRGBPoint(500, 0.1, 1.0, 0.3);
    ctf1->AddRGBPoint(1000, 0.1, 1.0, 0.3);
    ctf1->AddRGBPoint(1150, 1.0, 1.0, 0.9);*/

    ctf1->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
    ctf1->AddRGBPoint(-1000, .62, .36, .18, 0.5, 0.0);
    ctf1->AddRGBPoint(-500, .88, .60, .29, 0.33, 0.45);
    ctf1->AddRGBPoint(3071, .83, .66, 1, 0.5, 0.0);

    vtkNew<vtkPiecewiseFunction> pf1;
    pf1->AddPoint(0, 0.0);
    pf1->AddPoint(500, 1.0);


    pf1->AddPoint(-3024, 0, 0.5, 0.0);
    pf1->AddPoint(-1000, 0, 0.5, 0.0);
    pf1->AddPoint(-500, 1.0, 0.33, 0.45);
    pf1->AddPoint(3071, 1.0, 0.5, 0.0);

    vtkNew<vtkPiecewiseFunction> gf1;
    gf1->AddPoint(0, 0.0);
    gf1->AddPoint(500, 1.0);

    vtkNew<vtkVolume> vol1;
    vol1->GetProperty()->SetScalarOpacity(pf1);
    vol1->GetProperty()->SetColor(ctf1);
    vol1->GetProperty()->SetGradientOpacity(gf1);
    vol1->GetProperty()->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
    vol1->SetVisibility(false);

    // this is actually not used, the shader looks at the property of the first volume
    // vol1->GetProperty()->ShadeOn();

    vol1->RotateX(-55.);
    vol1->SetPosition(80., 50., 130.);

    // Volume 2 (brain)
    // -----------------------------
    vtkNew<vtkPiecewiseFunction> pf2;
    pf2->AddPoint(0, 0.0);
    pf2->AddPoint(5022, 0.09);

    vtkNew<vtkColorTransferFunction> ctf2;
    ctf2->AddRGBPoint(0, 1.0, 0.3, 0.2);
    ctf2->AddRGBPoint(2511, 0.3, 0.2, 0.9);
    ctf2->AddRGBPoint(5022, 0.5, 0.6, 1.0);

    vtkNew<vtkPiecewiseFunction> gf2;
    gf2->AddPoint(0, 0.0);
    gf2->AddPoint(550, 0.5);

    vtkNew<vtkVolume> vol2;
    vol2->GetProperty()->SetScalarOpacity(pf2);
    vol2->GetProperty()->SetColor(ctf2);
    vol2->GetProperty()->SetGradientOpacity(gf2);
    vol2->GetProperty()->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
    vol2->SetVisibility(false);
    // this is actually not used, the shader looks at the property of the first volume
    // vol2->GetProperty()->ShadeOn();

    vol2->SetScale(0.8, 0.8, 0.8);
    vol2->SetPosition(210., 200., -90.);
    vol2->RotateX(90.);
    vol2->RotateY(-95.);
    vol2->RotateZ(-5.);

    // Multi volume instance
    // ---------------------
    // Create an overlapping volume prop (add specific properties to each
    // entity).
    vtkNew<vtkMultiVolume> overlappingVol;
    vtkNew<vtkGPUVolumeRayCastMapper> mapper;
    mapper->UseJitteringOn();
    overlappingVol->SetMapper(mapper);



    mapper->SetInputConnection(0, headmrSource->GetOutputPort());
    overlappingVol->SetVolume(vol, 0);


    //vtkNew<vtkImageChangeInformation> changeInfo;

    //changeInfo->SetInputData(reader->GetOutput());
    //changeInfo->Update();

    mapper->SetInputConnection(1, reader->GetOutputPort());
    overlappingVol->SetVolume(vol1, 1);

    mapper->SetInputConnection(2, reader_MR->GetOutputPort());
    overlappingVol->SetVolume(vol2, 2);

    // Rendering context
    vtkNew<vtkRenderWindow> renWin;
    renWin->SetSize(512, 512);
    renWin->SetMultiSamples(0);

    vtkNew<vtkRenderer> ren;
    renWin->AddRenderer(ren);
    ren->SetBackground(1.0, 1.0, 1.0);

    ren->AddActor(axis);
    ren->AddActor(cone);
    ren->AddVolume(overlappingVol);

    vtkNew<vtkRenderWindowInteractor> iren;
    iren->SetRenderWindow(renWin);

    vtkNew<MoveRotateCommand> moveComm;
    moveComm->SetVolume(vol1);
    moveComm->RenderWindow = renWin;
    iren->AddObserver(vtkCommand::KeyPressEvent, moveComm);

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    iren->SetInteractorStyle(style);

    auto cam = ren->GetActiveCamera();
    cam->SetFocalPoint(41.9596, -17.9662, 78.5903);
    cam->SetPosition(373.891, 619.954, -53.5932);
    cam->SetViewUp(-0.0358384, -0.184856, -0.982112);
    renWin->Render();
    iren->Start();

    //int retVal = vtkTesting::Test(argc, argv, renWin, 90);
    //if (retVal == vtkRegressionTester::DO_INTERACTOR)
    //{
    //    iren->Start();
    //}

    //return !((retVal == vtkTesting::PASSED) || (retVal == vtkTesting::DO_INTERACTOR));
    return 0;
}


#endif

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Mace.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#if 0
#include "vtkActor.h"
#include "vtkConeSource.h"
#include "vtkDebugLeaks.h"
#include "vtkGlyph3D.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRegressionTestImage.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"
#include <vtkShrinkFilter.h>
#include <vtkElevationFilter.h>
#include <vtkDataSetMapper.h>

int main(int argc, char* argv[])
{
#if 0
    vtkRenderer* renderer = vtkRenderer::New();
    vtkRenderWindow* renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);

    vtkSphereSource* sphere = vtkSphereSource::New();
    sphere->SetThetaResolution(8);
    sphere->SetPhiResolution(8);
    vtkPolyDataMapper* sphereMapper = vtkPolyDataMapper::New();
    sphereMapper->SetInputConnection(sphere->GetOutputPort());
    vtkActor* sphereActor = vtkActor::New();
    sphereActor->SetMapper(sphereMapper);

    vtkConeSource* cone = vtkConeSource::New();
    cone->SetResolution(6);

    vtkGlyph3D* glyph = vtkGlyph3D::New();
    glyph->SetInputConnection(sphere->GetOutputPort());
    glyph->SetSourceConnection(cone->GetOutputPort());
    glyph->SetVectorModeToUseNormal();
    glyph->SetScaleModeToScaleByVector();
    glyph->SetScaleFactor(0.25);

    vtkPolyDataMapper* spikeMapper = vtkPolyDataMapper::New();
    spikeMapper->SetInputConnection(glyph->GetOutputPort());

    vtkActor* spikeActor = vtkActor::New();
    spikeActor->SetMapper(spikeMapper);

    //renderer->AddActor(sphereActor);
    renderer->AddActor(spikeActor);
    renderer->SetBackground(1, 1, 1);
    renWin->SetSize(300, 300);

    // interact with data
    renWin->Render();
    iren->Start();

    //int retVal = vtkRegressionTestImage(renWin);

    //if (retVal == vtkRegressionTester::DO_INTERACTOR)
    //{
    //    iren->Start();
    //}
    // Clean up
    renderer->Delete();
    renWin->Delete();
    iren->Delete();
    sphere->Delete();
    sphereMapper->Delete();
    sphereActor->Delete();
    cone->Delete();
    glyph->Delete();
    spikeMapper->Delete();
    spikeActor->Delete();

   // return !retVal;
    return 0;


#endif

#if 0


    vtkRenderWindow* renWin = vtkRenderWindow::New();

    vtkSphereSource* sphere = vtkSphereSource::New();
    sphere->SetThetaResolution(12); sphere->SetPhiResolution(12);
    vtkShrinkFilter* shrink = vtkShrinkFilter::New();
    shrink->SetInputConnection(sphere->GetOutputPort());
    shrink->SetShrinkFactor(0.9);
    vtkElevationFilter* colorIt = vtkElevationFilter::New();
    colorIt->SetInputConnection(shrink->GetOutputPort());
    colorIt->SetLowPoint(0, 0, -.5);
    colorIt->SetHighPoint(0, 0, .5);
    vtkDataSetMapper* mapper = vtkDataSetMapper::New();
    mapper->SetInputConnection(colorIt->GetOutputPort());
    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);
    renWin->Render(); // execute first time
    // create loop
    shrink->SetInputConnection(colorIt->GetOutputPort());
    renWin->Render(); // begin looping





#endif




}
#endif


#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSphereSource.h"
#include "vtkElevationFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkSphereSource.h"
#include "vtkDataSetMapper.h"

//#include "SaveImage.h"

int main(int argc, char* argv[])
{
    vtkRenderer* renderer = vtkRenderer::New();
    vtkRenderWindow* renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);

    vtkSphereSource* sphere = vtkSphereSource::New();
    sphere->SetPhiResolution(12); sphere->SetThetaResolution(12);
    sphere->Update();

    vtkElevationFilter* colorIt = vtkElevationFilter::New();
    //colorIt->SetInput(sphere->GetOutput());
    colorIt->SetInputData(sphere->GetOutput());
    colorIt->SetLowPoint(0, 0, -1);
    colorIt->SetHighPoint(0, 0, 1);
    colorIt->Update();

    vtkDataSetMapper* mapper = vtkDataSetMapper::New();
    mapper->SetInputData(colorIt->GetOutput());

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
    renderer->SetBackground(1, 1, 1);
    renWin->SetSize(300, 300);

    renWin->Render();

  //  SAVEIMAGE(renWin);

    // interact with data
    iren->Start();

    // Clean up
    renderer->Delete();
    renWin->Delete();
    iren->Delete();
    sphere->Delete();
    colorIt->Delete();
    mapper->Delete();
    actor->Delete();
}

