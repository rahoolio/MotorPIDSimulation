#pragma once
#include <tuple>
#include <vector>

namespace ELCT350
{
  class Plotter
  {
  public:
    Plotter(unsigned short width = 60, unsigned short height = 20);

    void addPoint(double x, double y, unsigned short plot = 0);
    void plot();

  private:
    /*const unsigned short _WIDTH;
    const unsigned short _HEIGHT;*/
    double _minX;
    double _maxX;
    double _minY;
    double _maxY;
    std::vector< std::tuple<double, double, unsigned short> > _function;

    void init();
  };
}
