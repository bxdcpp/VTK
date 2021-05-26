//#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL)
//VTK_MODULE_INIT(vtkInteractionStyle);
//VTK_MODULE_INIT(vtkRenderingFreeType);
//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolume.h>

int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName("C:/Users/Bxd/Desktop/testhead/3-5mm bone");
	reader->Update();

	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	volumeMapper->SetInputData(reader->GetOutput());
	volumeMapper->SetBlendModeToComposite();

	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOn();//��Ӱ���أ�����������ò���Ч
	volumeProperty->SetAmbient(0.4);
	volumeProperty->SetDiffuse(0.6);  //������
	volumeProperty->SetSpecular(0.2); //���淴��

	//���ò�͸����
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
#if 0
	compositeOpacity->AddPoint(-200, 0.00);
	compositeOpacity->AddPoint(-50, 0.10);
	compositeOpacity->AddPoint(90, 0.40);
	compositeOpacity->AddPoint(500, 0.60);
#endif


	compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0);
	compositeOpacity->AddPoint(-16, 0, .49, .61);
	compositeOpacity->AddPoint(641, .72, .5, 0.0);
	compositeOpacity->AddPoint(3071, .71, 0.5, 0.0);

	volumeProperty->SetScalarOpacity(compositeOpacity); //���ò�͸���ȴ��亯��
	//�����ݶȲ�͸������
	//vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	//volumeGradientOpacity->AddPoint(10, 0.0);
	//volumeGradientOpacity->AddPoint(70, 0.3);
	//volumeGradientOpacity->AddPoint(90, 0.5);
	//volumeGradientOpacity->AddPoint(500, 1.0);
	//volumeProperty->SetGradientOpacity(volumeGradientOpacity);//�����ݶȲ�͸����Ч���Ա�

	//������ɫ����
	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
#if 0
	color->AddRGBPoint(-400.000, 0.9, 0.8, 0.50);
	color->AddRGBPoint(-90.00, 0.9, 0.8, 0.50);
	color->AddRGBPoint(0.00, 8.00, 0.00, 0.00);
	color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
	color->AddRGBPoint(500.0, 1.00, 1.00, 1.00);
#endif


	color->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
	color->AddRGBPoint(-16, 0.73, 0.25, 0.30, 0.49, .61);
	color->AddRGBPoint(641, .90, .82, .56, .5, 0.0);
	color->AddRGBPoint(3071, 1, 1, 1, .5, 0.0);


	volumeProperty->SetColor(color);

	//����Actor
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0, 0, 0.25);
	renderer->AddVolume(volume);

	vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
	rw->AddRenderer(renderer);
	rw->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rwi->SetRenderWindow(rw);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	rwi->SetInteractorStyle(style);

	rwi->Initialize();
	rwi->Start();

	return 0;
}
