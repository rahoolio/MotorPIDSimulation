#include "Plotter/rang.h"
#include "Plotter/Plotter.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace ELCT350;
using namespace std;

const unsigned short WIDTH = 80;
const unsigned short HEIGHT = 20;

Plotter::Plotter(unsigned short width, unsigned short height)
       : //_WIDTH(width), _HEIGHT(height),
         _minX((numeric_limits<double>::max)()), _maxX(-(numeric_limits<double>::max)()),
         _minY((numeric_limits<double>::max)()), _maxY(-(numeric_limits<double>::max)())
{

}

void Plotter::addPoint(double x, double y, unsigned short plotIndex)
{
  _function.push_back({x,y, plotIndex});

  if(x < _minX)
    _minX = x;
  if(x > _maxX)
    _maxX = x;

  if(y < _minY)
    _minY = y;
  if(y > _maxY)
    _maxY = y;
}

void Plotter::plot()
{  
  unsigned short halfHeight = HEIGHT / 2;
  char grid[WIDTH][HEIGHT];
  for(unsigned short row = 0; row < WIDTH; ++row)
  {
    for(unsigned short column = 0; column < HEIGHT; ++ column)
    {
      if(row == 0)
        grid[row][column] = '|';
      else if(column == halfHeight)
        grid[row][column] = '-';
      else
        grid[row][column] = ' ';
    }
  }

  double domainRatio = static_cast<double>(WIDTH - 1) / (_maxX - _minX);
  double rangeRatio = 1;
  if(fabs(_maxY) >= fabs(_minY))
    rangeRatio = -static_cast<double>(HEIGHT - 1) / (2 * fabs(_maxY));
  else
    rangeRatio = -static_cast<double>(HEIGHT - 1) / (2 * fabs(_minY));

  if(isinf(rangeRatio))
    rangeRatio = 0.0;

  for(const auto& point : _function)
  {
    unsigned short x = static_cast<unsigned short>(get<0>(point) * domainRatio);
    unsigned short y = static_cast<unsigned short>(halfHeight + get<1>(point) * rangeRatio);
    switch(get<2>(point))
    {
    case 0:      
      grid[x][y] = '.';
      break;
    case 1:
      grid[x][y] = '*';
      break;
    default:
      grid[x][y] = '!';
    }
  }

  for(unsigned short column = 0; column < HEIGHT; ++ column)
  {
    for(unsigned short row = 0; row < WIDTH; ++row)
    {
      if(grid[row][column] == '|')
        cout << rang::fg::gray << '|';
      else if(grid[row][column] == '-')
        cout << rang::fg::gray << '-';
      else if(grid[row][column] == '.')
        cout << rang::fg::yellow << '.';
      else if(grid[row][column] == '*')
        cout << rang::fg::cyan << '.';
      else if(grid[row][column] == '!')
        cout << rang::fg::red << '.';
      else
        cout << ' ';
    }
    cout << endl;
  }
  cout << rang::fg::gray;
}
