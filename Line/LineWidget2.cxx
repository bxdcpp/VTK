#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkLineRepresentation.h>
#include <vtkLineWidget2.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkCursor3D.h>

namespace {
// This does the actual work.
// Callback for the interaction
class vtkLineCallback : public vtkCommand
{
public:
  static vtkLineCallback* New()
  {
    return new vtkLineCallback;
  }

  virtual void Execute(vtkObject* caller, unsigned long, void*)
  {

    vtkLineWidget2* lineWidget = reinterpret_cast<vtkLineWidget2*>(caller);

    // Get the actual box coordinates of the line
    vtkNew<vtkPolyData> polydata;
    static_cast<vtkLineRepresentation*>(lineWidget->GetRepresentation())
        ->GetPolyData(polydata);

    // Display one of the points, just so we know it's working
    double p[3];
    polydata->GetPoint(0, p);
    std::cout << "P: " << p[0] << " " << p[1] << " " << p[2] << std::endl;
  }
  vtkLineCallback()
  {
  }
};
} // namespace

int main(int, char*[])
{
  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->Update();

  // Create a mapper and actor
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

  // A renderer and render window
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("LineWidget2");


  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("MidnightBlue").GetData());

  // An interactor
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  vtkNew<vtkLineWidget2> lineWidget;
  lineWidget->ManagesCursorOn();
  //lineWidget->Setjo
  lineWidget->SetInteractor(renderWindowInteractor);
  //lineWidget->CreateDefaultRepresentation();
  //lineWidget->GetEventTranslator();


  // You could do this if you want to set properties at this point:
   vtkNew<vtkLineRepresentation> lineRepresentation;
 /*  lineRepresentation->SetLineColor(0, 1, 0);
   lineRepresentation->Se*/

   lineRepresentation->DistanceAnnotationVisibilityOn();//距离注释
   lineRepresentation->SetDistanceAnnotationFormat("%-#6.3g mm");
   vtkProperty* lineProp = lineRepresentation->GetLineProperty();
   vtkProperty* selectlineProp = lineRepresentation->GetSelectedLineProperty();
   lineProp->SetAmbientColor(.5, 0, 0);
   selectlineProp->SetColor(1, 0, 0);
   double a[] = { 1,0.2,0,1 };
   lineProp->SetSelectionColor(a);
   lineProp->SetSelectionLineWidth(5);
   lineWidget->SetRepresentation(lineRepresentation);



   vtkWidgetEventTranslator* eventTranslation = lineWidget->GetEventTranslator();
   eventTranslation->SetTranslation(
       vtkCommand::LeftButtonPressEvent,
       vtkWidgetEvent::SizeHandles);
   //eventTranslation->SetTranslation(
   //    vtkCommand::RightButtonPressEvent,
   //    vtkWidgetEvent::NoEvent);
   //eventTranslation->SetTranslation(
   //    vtkCommand::RightButtonReleaseEvent,
   //    vtkWidgetEvent::Select);

  vtkNew<vtkLineCallback> lineCallback;

  lineWidget->AddObserver(vtkCommand::InteractionEvent, lineCallback);

  // Render
  renderWindow->Render();

  renderWindowInteractor->Initialize();
  renderWindow->Render();
  lineWidget->On();

  // Begin mouse interaction
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}