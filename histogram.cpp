#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(std::vector<unsigned int> durations, QString filename, QWidget *parent) :
    QMainWindow(parent),
    process_durations(nullptr),
    ui(new Ui::Histogram)
{
    // Create a QGraphicsScene and add the graphics to it
    process_durations = new std::vector<unsigned int> (durations);
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->GVHistogram->setScene(scene);
    unsigned short int current_shown_process = 0, vector_size = process_durations->size();
    for (std::vector<unsigned int>::reverse_iterator rit = process_durations->rbegin(); rit != process_durations->rend(); ++rit) {
        if (vector_size > 500) {
            scene->addRect(6 + current_shown_process, process_durations->back() - (*rit), 1, (*rit), QPen(QColor(0, 255, 0)), QBrush());
        }
        else {
            scene->addRect(6 + current_shown_process * 3, process_durations->back() - (*rit), 3, (*rit), QPen(QColor(0, 255, 0)), QBrush());
        }
        current_shown_process += 1;
    }

    // Save the QGrapicsScene to a file
    scene->clearSelection(); // Selections would also render to the file
    scene->setSceneRect(scene->itemsBoundingRect()); // Re-shrink the scene to it's bounding contents
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32); // Create the image with the exact size of the shrunk scene
    image.fill(Qt::white); // Start all pixels transparent

    QPainter painter(&image);
    scene->render(&painter);
    image.save(filename.append(".png"));
}

Histogram::~Histogram()
{
    delete scene;
    delete ui;
    delete process_durations;
}
