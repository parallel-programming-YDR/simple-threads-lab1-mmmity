#pragma once

#include <random>
#include <thread>

class MonteCarloCounter
{
  std::mt19937 rand;
  double       radius;

  size_t tries_count{0};
  size_t success_count{0};

  static double scale(size_t value, size_t min_in, size_t max_in, double min_out, double max_out);

public:
  MonteCarloCounter(size_t seed, double radius);
  double get_current_area_estimation();
  void   add_try();
  void   add_tries(size_t count);
  size_t get_tries_count();
  size_t get_success_count();
};