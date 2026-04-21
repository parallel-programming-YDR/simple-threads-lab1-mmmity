#include "MonteCarloCounter.h"
#include <iostream>

MonteCarloCounter::MonteCarloCounter(size_t seed, double radius) : rand(seed), radius(radius) {}

double MonteCarloCounter::scale(size_t value, size_t min_in, size_t max_in, double min_out, double max_out)
{
  double relative_pos = static_cast<double>(value - min_in) / (max_in - min_in);
  return min_out + relative_pos * (max_out - min_out);
}

double MonteCarloCounter::get_current_area_estimation()
{
  return static_cast<double>(success_count) / static_cast<double>(tries_count) * radius * radius;
}

void MonteCarloCounter::add_try()
{
  size_t x_raw = rand();
  size_t y_raw = rand();

  double x = scale(x_raw, rand.min(), rand.max(), -radius, radius);
  double y = scale(y_raw, rand.min(), rand.max(), -radius, radius);

  ++tries_count;
  if (x * x + y * y <= radius * radius)
  {
    ++success_count;
  }
}

void MonteCarloCounter::add_tries(size_t tries)
{
  for (size_t i = 0; i < tries; ++i)
  {
    add_try();
  }
}

size_t MonteCarloCounter::get_success_count()
{
  return success_count;
}

size_t MonteCarloCounter::get_tries_count()
{
  return tries_count;
}