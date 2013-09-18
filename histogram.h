#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <vector>

namespace Ui {
class Histogram;
}

class Histogram : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Histogram(std::vector<unsigned int> durations, QString filename, QWidget *parent = 0);
    ~Histogram();
    
private:
    QGraphicsScene *scene;
    Ui::Histogram *ui;
    std::vector<unsigned int> *process_durations;
};

#endif // HISTOGRAM_H
