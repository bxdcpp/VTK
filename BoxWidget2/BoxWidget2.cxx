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
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkPlanes.h>

namespace {
class vtkBoxCallback : public vtkCommand
{
public:
  static vtkBoxCallback* New()
  {
    return new vtkBoxCallback;
  }

  vtkSmartPointer<vtkActor> m_actor;

  void SetActor(vtkSmartPointer<vtkActor> actor)
  {
    m_actor = actor;
  }

  virtual void Execute(vtkObject* caller, unsigned long, void*)
  {
    vtkSmartPointer<vtkBoxWidget2> boxWidget =
        dynamic_cast<vtkBoxWidget2*>(caller);

    vtkNew<vtkTransform> t;

    dynamic_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation())
        ->GetTransform(t);
    this->m_actor->SetUserTransform(t);
  }

  vtkBoxCallback()
  {
  }
};
} // namespace

int main(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  vtkNew<vtkNamedColors> colors;

  // 显示坐标系的vtk组件
  vtkSmartPointer<vtkAxesActor> axes_actor = vtkSmartPointer<vtkAxesActor>::New();
  axes_actor->SetPosition(0, 0, 0);
  axes_actor->SetTotalLength(2, 2, 2);
  axes_actor->SetShaftType(0);
  axes_actor->SetCylinderRadius(0.02);



  vtkNew<vtkConeSource> coneSource;
  coneSource->SetHeight(1.5);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(coneSource->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("BurlyWood").GetData());

  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor);
  //renderer->SetBackground(colors->GetColor3d("Blue").GetData());
  renderer->SetBackground(colors->GetColor3d("Black").GetData());
  renderer->ResetCamera(); // Reposition camera so the whole scene is visible

  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("BoxWidget2");

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Use the "trackball camera" interactor style, rather than the default
  // "joystick camera"
  vtkNew<vtkInteractorStyleTrackballCamera> style;
  renderWindowInteractor->SetInteractorStyle(style);

  vtkNew<vtkBoxWidget2> boxWidget;
  boxWidget->SetInteractor(renderWindowInteractor);
  boxWidget->GetRepresentation()->SetPlaceFactor(1); // Default is 0.5
  boxWidget->GetRepresentation()->PlaceWidget(actor->GetBounds());
  vtkNew<vtkPlanes> planes;
  vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation())->GetPlanes(planes);

  // Set up a callback for the interactor to call so we can manipulate the actor
  vtkNew<vtkBoxCallback> boxCallback;
  boxCallback->SetActor(actor);
  boxWidget->AddObserver(vtkCommand::InteractionEvent, boxCallback);

  boxWidget->On();

  // 控制坐标系，使之随视角共同变化 
  vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  widget->SetOrientationMarker(axes_actor);
  widget->SetInteractor(renderWindowInteractor);
  widget->On();

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
