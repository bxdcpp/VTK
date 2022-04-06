#include <vtkActor.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
#include <vtkCommand.h>
#include <vtkConeSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkPlanes.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredPoints.h>//vtkStructuredPointsReader* -> vtkGPUVolumeRayCastMapper*
#include <vtkStructuredPointsReader.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolume.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkImageCroppingRegionsWidget.h>
#include <vtkExtractVOI.h>
#include <vtkBoxRepresentation.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkPlanes.h>
#include <vtkMatrix3x3.h>


//
#include <vtkGeneralTransform.h>

//ITK include
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"
#include "itkScaleTransform.h"
#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <itkNiftiImageIO.h>

namespace {
	class vtkBoxCallback : public vtkCommand
	{
	public:
		static vtkBoxCallback* New()
		{
			return new vtkBoxCallback;
		}

		vtkSmartPointer<vtkActor> m_actor;
		vtkSmartPointer<vtkImageData> m_imageData;
		double roiXYZ[3]{ 0 };
		double roiRadius[3] = { 0 };
		int flag = 0;

		//----------------------------------------------------------------------------
		void NormalizeOrientationMatrixColumns(vtkMatrix4x4* m, double scale[3])
		{
			if (!m)
			{
				vtkGenericWarningMacro("NormalizeOrientationMatrixColumns: invalid input matrix");
				return;
			}
			for (int col = 0; col < 3; col++)
			{
				double len = 0;
				for (int row = 0; row < 3; row++)
				{
					len += m->GetElement(row, col) * m->GetElement(row, col);
				}
				len = sqrt(len);
				scale[col] = len;
				for (int row = 0; row < 3; row++)
				{
					m->SetElement(row, col, m->GetElement(row, col) / len);
				}
			}
		}

		void SetActor(vtkSmartPointer<vtkActor> actor)
		{
			m_actor = actor;
			m_actor->GetMatrix()->Print(std::cout);
		}

		void SetImageData(vtkSmartPointer<vtkImageData> img)
		{
			m_imageData = img;
			double origin[3]{ -1 };
			double spacing[3]{ -1 };
			m_imageData->GetOrigin(origin);
			m_imageData->GetSpacing(spacing);
			std::cout << "(" << origin[0] << "," << origin[0] << "," << origin[0] << ")" << std::endl;
			std::cout << "(" << spacing[0] << "," << spacing[0] << "," << spacing[0] << ")" << std::endl;
			m_imageData->GetIndexToPhysicalMatrix()->Print(std::cout);

			//test


		}

		void GetInterpolatedCropOutputGeometry(vtkImageData* input, vtkTransform* roiTransform, double*bounds, int outputExtent[6], double outputSpacing[3], double spacingScale = 1.0)
		{
			if (!input || !roiTransform||!bounds)
				return;

			double* inputSpacing = input->GetSpacing();
			vtkNew<vtkMatrix4x4> volumeIJKToLPS;
			volumeIJKToLPS->DeepCopy(input->GetIndexToPhysicalMatrix());
			vtkNew<vtkMatrix4x4> volumeLPSTOROI;
			//volumeLPSTOROI->DeepCopy(roiTransform->GetMatrix());
			vtkMatrix4x4::Invert(roiTransform->GetMatrix(), volumeLPSTOROI);

			// Find which image axis corresponds to each ROI axis, to get the correct spacing value for each ROI axis
			vtkNew<vtkMatrix4x4> volumeIJKToROI;
			vtkMatrix4x4::Multiply4x4(volumeLPSTOROI.GetPointer(), volumeIJKToLPS.GetPointer(), volumeIJKToROI.GetPointer());
			double scale[3] = { 1.0 };
			NormalizeOrientationMatrixColumns(volumeIJKToROI.GetPointer(), scale);
			int inputAxisIndexForROIAxis[3] = { 0, 1, 2 };
			// Find the volumeIJK axis that is best aligned with each ROI azis
			for (int roiAxisIndex = 0; roiAxisIndex < 3; roiAxisIndex++)
			{
				double largestComponentValue = 0;
				for (int volumeIJKAxisIndex = 0; volumeIJKAxisIndex < 3; volumeIJKAxisIndex++)
				{
					double currentComponentValue = fabs(volumeIJKToROI->GetElement(roiAxisIndex, volumeIJKAxisIndex));
					if (currentComponentValue > largestComponentValue)
					{
						largestComponentValue = currentComponentValue;
						inputAxisIndexForROIAxis[roiAxisIndex] = volumeIJKAxisIndex;
					}
				}
			}
			outputSpacing[0] = inputSpacing[inputAxisIndexForROIAxis[0]] * spacingScale;
			outputSpacing[1] = inputSpacing[inputAxisIndexForROIAxis[1]] * spacingScale;
			outputSpacing[2] = inputSpacing[inputAxisIndexForROIAxis[2]] * spacingScale;

			// prepare the resampling reference volume
			double roiRadius[3] = { 0 };
			roiRadius[0] = (bounds[1] - bounds[0])/2.0;
			roiRadius[1] = (bounds[3] - bounds[2])/2.0;
			roiRadius[2] = (bounds[5] - bounds[4])/2.0;

			outputExtent[0] = outputExtent[2] = outputExtent[4] = 0;
			// add a bit of tolerance in deciding how many voxels the output should contain
			// to make sure that if the ROI size is set to match the image size exactly then we
			// output extent contains the whole image
			double tolerance = 0.001;
			outputExtent[1] = int(roiRadius[0] / outputSpacing[0] * 2. + tolerance) - 1;
			outputExtent[3] = int(roiRadius[1] / outputSpacing[1] * 2. + tolerance) - 1;
			outputExtent[5] = int(roiRadius[2] / outputSpacing[2] * 2. + tolerance) - 1;
		}

		void CropInterpolated(vtkImageData* input, vtkTransform* roiTransform, double* bounds, int outputExtent[6], double outputSpacing[3], double spacingScale = 1.0,double fillValue = -1024)
		{
			flag += 1;
			if (flag % 10 != 0) return;
			GetInterpolatedCropOutputGeometry(input, roiTransform, bounds, outputExtent, outputSpacing);
			vtkNew<vtkMatrix4x4> outputIJKToLPS;
			outputIJKToLPS->SetElement(0, 0, outputSpacing[0]);
			outputIJKToLPS->SetElement(1, 1, outputSpacing[1]);
			outputIJKToLPS->SetElement(2, 2, outputSpacing[2]);
	/*		outputIJKToLPS->SetElement(0, 3, 0);
			outputIJKToLPS->SetElement(1, 3, 0);
			outputIJKToLPS->SetElement(2, 3, 0);*/

			outputIJKToLPS->SetElement(0, 3, roiXYZ[0] - roiRadius[0]);
			outputIJKToLPS->SetElement(1, 3, roiXYZ[1] - roiRadius[1]);
			outputIJKToLPS->SetElement(2, 3, roiXYZ[2] - roiRadius[2]);

			/*vtkNew<vtkMatrix4x4> volumeIJKToLPS;
			volumeIJKToLPS->DeepCopy(input->GetIndexToPhysicalMatrix());*/
			vtkNew<vtkMatrix4x4> roi2OutVolumeMat;
			roi2OutVolumeMat->DeepCopy(roiTransform->GetMatrix());

			//vtkMatrix4x4::Multiply4x4(roi2VolumeMat.GetPointer(), outputIJKToLPS.GetPointer(), outputIJKToLPS.GetPointer());

			vtkMatrix4x4::Multiply4x4(roi2OutVolumeMat.GetPointer(), outputIJKToLPS.GetPointer(), outputIJKToLPS.GetPointer());

			// contains axis directions, in unconventional indexing (column, row)
			// so that it can be conveniently normalized
			double outputDirectionColRow[3][3] = { { 0 } };
			for (int column = 0; column < 3; column++)
			{
				for (int row = 0; row < 3; row++)
				{
					outputDirectionColRow[column][row] = outputIJKToLPS->GetElement(row, column);
				}
				outputSpacing[column] = vtkMath::Normalize(outputDirectionColRow[column]);
			}


			// Center the output image in the ROI. For that, compute the size difference between
			// the ROI and the output image.
			double sizeDifference_IJK[3] =
			{
			roiRadius[0] * 2 / outputSpacing[0] - (outputExtent[1] - outputExtent[0] + 1),
			roiRadius[1] * 2 / outputSpacing[1] - (outputExtent[3] - outputExtent[2] + 1),
			roiRadius[2] * 2 / outputSpacing[2] - (outputExtent[5] - outputExtent[4] + 1)
			};

			// Origin is in the voxel's center. Shift the origin by half voxel
			// to have the ROI edge at the output image voxel edge.
			double outputOrigin_IJK[4] =
			{
			0.5 + sizeDifference_IJK[0] / 2,
			0.5 + sizeDifference_IJK[1] / 2,
			0.5 + sizeDifference_IJK[2] / 2,
			1.0
			};
			double outputOrigin_LPS[4] = { 0.0, 0.0, 0.0, 1.0 };
			outputIJKToLPS->MultiplyPoint(outputOrigin_IJK, outputOrigin_LPS);



			////itk
			//using PixelType = unsigned char;
			using PixelType = int;
			using ImageType = itk::Image<PixelType, 3>;
			using ScalarType = double;
			using IndexValueType = typename itk::Index<3>::IndexValueType;
			//using AddImageFilterType = itk::AddImageFilter<ImageType, ImageType>;
			using LinearInterpolatorType = itk::LinearInterpolateImageFunction<ImageType, ScalarType>;
			typename LinearInterpolatorType::Pointer interpolator = LinearInterpolatorType::New();

			using VTKImage2ITKImageType = itk::VTKImageToImageFilter<ImageType>;
			using ITKImage2VTKImageType = itk::ImageToVTKImageFilter<ImageType>;

			using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType>;
			typename ResampleFilterType::Pointer resampleFilter = ResampleFilterType::New();
			resampleFilter->SetDefaultPixelValue(fillValue);

			vtkNew<vtkImageData> copyData;
			copyData->DeepCopy(m_imageData);
			copyData->SetOrigin(0, 0, 0); 
			copyData->SetSpacing(1.0, 1.0, 1.0);
			vtkNew<vtkMatrix3x3> iM3;
			copyData->SetDirectionMatrix(iM3);
			VTKImage2ITKImageType::Pointer vtk2itkFilter = VTKImage2ITKImageType::New();
			vtk2itkFilter->SetInput(m_imageData);
			vtk2itkFilter->Update();

			ImageType* segITKImage = vtk2itkFilter->GetOutput();
			resampleFilter->SetInput(segITKImage);
			//resampleFilter->SetTransform(transform);
			resampleFilter->SetInterpolator(interpolator);
			typename ImageType::SizeType outputSize;
			outputSize[0] = outputExtent[1] - outputExtent[0] + 1;
			outputSize[1] = outputExtent[3] - outputExtent[2] + 1;
			outputSize[2] = outputExtent[5] - outputExtent[4] + 1;

			resampleFilter->SetSize(outputSize);
			resampleFilter->SetOutputSpacing(outputSpacing);
			resampleFilter->SetOutputOrigin(outputOrigin_LPS);
			typename ImageType::DirectionType direction;
			direction.SetIdentity();
	
			for (int row = 0; row < 3; row++)
			{
				for (int column = 0; column < 3; column++)
				{
					
					direction[column][row] =  outputDirectionColRow[column][row];
				}
			}
			resampleFilter->SetOutputDirection(direction);
			resampleFilter->Update();

			/*ITKImage2VTKImageType::Pointer itk2vtkFilter = ITKImage2VTKImageType::New();
			itk2vtkFilter->SetInput(resampleFilter->GetOutput());
			itk2vtkFilter->Update();*/

			using WriterType = itk::ImageFileWriter<ImageType>;
			typename WriterType::Pointer writer = WriterType::New();
			const char* outputFileName = "crop_volume.data.nrrd";
			writer->SetFileName(outputFileName);
			writer->SetInput(resampleFilter->GetOutput());
			flag = 1;

			try
			{
				writer->Update();
			}
			catch (itk::ExceptionObject& error)
			{
				std::cerr << "Error: " << error << std::endl;
				return;
			}

		/*	itk::NiftiImageIO::Pointer nifti_io = itk::NiftiImageIO::New();
			itk::CommonEnums::IOPixel
			nifti_io->SetPixelType(PixelType);

			itk::ImageFileWriter<ImageType>::Pointer dwi_writer = itk::ImageFileWriter<ImageType>::New();
			dwi_writer->SetFileName("test.nii.gz");
			dwi_writer->SetInput(out_dwi);
			dwi_writer->SetImageIO(nifti_io);
			dwi_writer->Update();*/
			
		}


		virtual void Execute(vtkObject* caller, unsigned long, void*)
		{
			vtkSmartPointer<vtkBoxWidget2> boxWidget =
				dynamic_cast<vtkBoxWidget2*>(caller);

			vtkNew<vtkTransform> volumeToROITransform;
			vtkNew<vtkTransform> ROITransform;

			vtkBoxRepresentation* boxRep = dynamic_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation());
			boxRep->GetTransform(ROITransform);
			double* bounds = boxRep->GetBounds();
			roiRadius[0] = (bounds[1] - bounds[0]) / 2.0;
			roiRadius[1] = (bounds[3] - bounds[2]) / 2.0;
			roiRadius[2] = (bounds[5] - bounds[4]) / 2.0;

			std::cout << "bounds:" << bounds[0] << "," << bounds[1] << "," << bounds[2]
				<< "," << bounds[3] << "," << bounds[4] << "," << bounds[5] << std::endl;
			this->m_actor->SetUserTransform(ROITransform);

		
			ROITransform->GetMatrix()->Print(std::cout);
			vtkNew<vtkPolyData> polyData;
			boxRep->GetPolyData(polyData);
			vtkIdType num = polyData->GetNumberOfPoints();
			vtkPoints* ps = polyData->GetPoints();
			ps->GetPoint(14, roiXYZ);

			int outExtent[6]{ 0 };
			double outputSpacing[3]{ 1,1,1 };
			CropInterpolated(m_imageData, ROITransform, bounds, outExtent, outputSpacing);
#if 0
			for (vtkIdType i = 0; i < num; i++)
			{
				double pos[3]{ -1,-1,-1 };
				ps->GetPoint(i, pos);
				std::cout << i << "  " << "(" << pos[0] << "," << pos[1] << "," << pos[2] << ")" << std::endl;
			}
#endif
		}

		vtkBoxCallback()
		{
		}
	};
} // namespace

int main(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
	/*double tolerance = 0.001;
	int b = int(2 + tolerance) - 1;
	std::cout << "b:" << b << std::endl;*/
	vtkNew<vtkNamedColors> colors;

	/* vtkNew<vtkConeSource> coneSource;
	 coneSource->SetHeight(1.5);*/

	vtkSmartPointer<vtkStructuredPointsReader> reader =
		vtkSmartPointer<vtkStructuredPointsReader>::New();
	const char* name = "C:/project/data/predict_input/Task002_VerSe/1.3.6.1.4.1.9328.50.4.0090_0000.vtk";
	const char* name2 = "C:/Users/Bxd/Downloads/mummy.128.vtk";
	reader->SetFileName(name);
	reader->Update();

	vtkSmartPointer<vtkGPUVolumeRayCastMapper> origMapper =
		vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	origMapper->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper =
		vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	//volumeMapper->SetInputData(reader->GetOutput());
	//volumeMapper->SetCropping(1);//����Cropping����
	//volumeMapper->SetCroppingRegionPlanes(50, 150, 50, 200, 50, 150);
	//volumeMapper->SetCroppingRegionFlags(0x0002000);
	/*****************************************************************/
	//����������������
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->SetInterpolationTypeToLinear(); //�������Բ�ֵ��ʽ
	volumeProperty->ShadeOn();//������Ӱ����
	volumeProperty->SetAmbient(0.4);//���û����¶�
	volumeProperty->SetDiffuse(0.6);//����������ϵ��
	volumeProperty->SetSpecular(0.2);//���þ��淴��ϵ��
	//���ӻҶȲ�͸��������
	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(70, 0.0);
	compositeOpacity->AddPoint(90, 0.4);
	compositeOpacity->AddPoint(180, 0.6);
	volumeProperty->SetScalarOpacity(compositeOpacity);
	//�����ݶȲ�ͬ��������
	vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	gradientOpacity->AddPoint(10, 0.0);
	gradientOpacity->AddPoint(90, 0.5);
	gradientOpacity->AddPoint(100, 1.0);
	volumeProperty->SetGradientOpacity(gradientOpacity);
	//������ɫ����
	vtkSmartPointer<vtkColorTransferFunction> color =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0, 0, 0, 0);
	color->AddRGBPoint(64, 1.0, 0.52, 0.3);
	color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
	color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
	volumeProperty->SetColor(color);
	/***********************************************************/

	//��Ⱦ�ܵ�
	vtkSmartPointer<vtkVolume> origVolume =
		vtkSmartPointer<vtkVolume>::New();
	origVolume->SetMapper(origMapper);
	origVolume->SetProperty(volumeProperty);

	vtkSmartPointer<vtkRenderer> origRender =
		vtkSmartPointer<vtkRenderer>::New();
	origRender->AddVolume(origVolume);
	origRender->SetBackground(1, 1, 0);
	//origRender->SetViewport(origView);
	origRender->ResetCamera(); // Reposition camera so the whole scene is visible

	vtkNew<vtkPolyDataMapper> mapper;
	//mapper->SetInputConnection(coneSource->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d("BurlyWood").GetData());

	//vtkNew<vtkRenderer> renderer;
	//renderer->AddActor(actor);
	//renderer->SetBackground(colors->GetColor3d("Blue").GetData());
	//renderer->ResetCamera(); // Reposition camera so the whole scene is visible

	vtkNew<vtkRenderWindow> renderWindow;
	//renderWindow->AddRenderer(renderer);
	renderWindow->AddRenderer(origRender);
	renderWindow->SetWindowName("BoxWidget2");

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Use the "trackball camera" interactor style, rather than the default
	// "joystick camera"
	vtkNew<vtkInteractorStyleTrackballCamera> style;
	//style->Set
	renderWindowInteractor->SetInteractorStyle(style);

	vtkNew<vtkBoxWidget2> boxWidget;
	boxWidget->SetInteractor(renderWindowInteractor);
	boxWidget->GetRepresentation()->SetPlaceFactor(1); // Default is 0.5
	//boxWidget->GetRepresentation()->PlaceWidget(actor->GetBounds());
	//boxWidget->SetRotationEnabled(false);
	double* bd = origVolume->GetBounds();
	double bounds[6]{ bd[0],bd[1]-80, bd[2], bd[3]-130, bd[4], bd[5] - 150 };
	//boxWidget->GetRepresentation()->PlaceWidget(origVolume->GetBounds());
	boxWidget->GetRepresentation()->PlaceWidget(bounds);
	vtkBoxRepresentation* boxRep = dynamic_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation());
	vtkNew<vtkTransform> ROITransform;
	boxRep->GetTransform(ROITransform);
	
	vtkMatrix4x4* mat4 = ROITransform->GetMatrix();
	double* extent = boxRep->GetBounds();
	std::cout << "extend:" << extent[0] << "," << extent[1] << "," << extent[2]
		<< "," << extent[3] << "," << extent[4] << "," << extent[5] << std::endl;

	// Set up a callback for the interactor to call so we can manipulate the actor
	vtkNew<vtkBoxCallback> boxCallback;
	boxCallback->SetActor(actor);
	boxCallback->SetImageData(reader->GetOutput());

	//int extent[6]{ 0 };
	//double sp[3]{ 0 };
	//boxCallback->CropInterpolated(boxCallback->m_imageData, ROITransform, bounds, extent, sp);
	boxWidget->AddObserver(vtkCommand::InteractionEvent, boxCallback);



	boxWidget->On();

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
