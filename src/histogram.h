#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QMainWindow>

#include <memory>
#include <vector>

class QGraphicsScene;

namespace Ui {
class Histogram;
} // namespace Ui

class Histogram : public QMainWindow
{
    Q_OBJECT
    
public:
    Histogram(std::vector<unsigned int> &&argDurations,
              const QString &argFilename, QWidget *const argParent = nullptr);
    ~Histogram() override;
    
private:
    std::vector<unsigned int> process_durations;
    QGraphicsScene *const scene = nullptr;
    const std::unique_ptr<Ui::Histogram> ui;
};

#endif // HISTOGRAM_H
