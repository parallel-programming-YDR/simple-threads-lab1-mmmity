#include "MonteCarloCounter.h"
#include <iomanip>
#include <iostream>
#include <time.h>

double calculate_with_monte_carlo(size_t tries, size_t seed, size_t radius, size_t thread_count)
{
  std::vector<std::thread>       threads;
  std::vector<MonteCarloCounter> counters;
  for (size_t i = 0; i < thread_count; ++i)
  {
    counters.emplace_back(seed + i, radius);
  }

  for (size_t i = 0; i < thread_count; ++i)
  {
    threads.push_back(std::thread(
      [&counters, i, tries, radius, thread_count]()
      {
        size_t local_tries = tries / thread_count;
        if (i < tries % thread_count)
        {
          ++local_tries; // if tries is not divisible by thread_count
        }

        counters[i].add_tries(local_tries);
      }));
  }

  size_t success_count = 0;
  for (size_t i = 0; i < thread_count; ++i)
  {
    threads[i].join();
    success_count += counters[i].get_success_count();
  }

  return static_cast<double>(success_count) / tries * 4 * radius * radius;
}

int main(int argc, char **argv)
{
  if (argc < 2 || argc > 3)
  {
    std::cerr << "usage: ./system-threads tries [seed]\n";
    return 1;
  }

  size_t tries, seed = 0;
  try
  {
    tries = std::stoull(argv[1]);
    if (argc == 3)
    {
      seed = std::stoull(argv[2]);
    }
  }
  catch (const std::exception &exp)
  {
    std::cerr << exp.what() << "\n";
    std::cerr << "usage: ./system-threads tries [seed]\n";
    return 1;
  }

  std::cout << std::setprecision(3) << std::fixed;

  int radius, thread_count;
  while (std::cin >> radius >> thread_count)
  {
    if (radius <= 0 || thread_count <= 0)
    {
      std::cout << "input should be positive\n";
      return 1;
    }

    auto                     start_time   = std::chrono::steady_clock::now();
    double                   area         = calculate_with_monte_carlo(tries, seed, radius, thread_count);
    std::chrono::nanoseconds time_elapsed = std::chrono::steady_clock::now() - start_time;
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(time_elapsed);
    std::cout << elapsed_ms.count() << " " << area << "\n";
  }
}
