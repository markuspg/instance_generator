#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(std::vector<unsigned int> &&argDurations,
                     const QString &argFilename, QWidget *const argParent) :
    QMainWindow{argParent},
    process_durations{std::move(argDurations)},
    scene{new QGraphicsScene},
    ui{std::make_unique<Ui::Histogram>()}
{
    // Create a QGraphicsScene and add the graphics to it
    ui->setupUi(this);
    setWindowTitle(argFilename);
    scene->setParent(this);
    ui->GVHistogram->setScene(scene);

    unsigned short int current_shown_process = 0;
    const auto vector_size = process_durations.size();
    for (auto rit = process_durations.rbegin();
         rit != process_durations.rend(); ++rit) {
        if (vector_size > 500) {
            scene->addRect(6 + current_shown_process,
                           process_durations.back() - (*rit), 1, (*rit),
                           QPen{QColor{0, 255, 0}});
        }
        else {
            scene->addRect(6 + current_shown_process * 3,
                           process_durations.back() - (*rit), 3, (*rit),
                           QPen{QColor{0, 255, 0}});
        }
        ++current_shown_process;
    }

    // Save the QGrapicsScene to a file
    scene->clearSelection(); // Selections would also render to the file
    // Re-shrink the scene to it's bounding contents
    scene->setSceneRect(scene->itemsBoundingRect());
    // Create the image with the exact size of the shrunk scene
    QImage image{scene->sceneRect().size().toSize(), QImage::Format_ARGB32};
    image.fill(Qt::white); // Start all pixels transparent

    QPainter painter{&image};
    scene->render(&painter);
    image.save(QString{argFilename}.append(".png"));
}

Histogram::~Histogram()
{
}
