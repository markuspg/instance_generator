#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(std::vector<unsigned int> durations, QWidget *parent) :
    QMainWindow(parent),
    process_durations(nullptr),
    ui(new Ui::Histogram)
{
    process_durations = new std::vector<unsigned int> (durations);
    ui->setupUi(this);
    QGraphicsScene *scene;
    scene = new QGraphicsScene();
    ui->GVHistogram->setScene(scene);
    for (unsigned short int i = 0; i < process_durations->size(); i++) {
        if (process_durations->size() > 500) {
            scene->addLine(QLine(i,0,i,process_durations->at(i)));
        }
        else {
            scene->addRect(i*3,0,3,process_durations->at(i),QPen(QColor(0,255,0)),QBrush());
        }
    }
}

Histogram::~Histogram()
{
    delete ui;
    delete process_durations;
}
