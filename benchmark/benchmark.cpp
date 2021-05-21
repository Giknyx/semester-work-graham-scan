#include <fstream>      // ifstream
#include <iostream>     // cout
#include <string>       // string, stoi
#include <string_view>  // string_view
#include <chrono>       // high_resolution_clock, duration_cast, nanoseconds
#include <sstream>      // stringstream
#include <errno.h>      // for errno
#include <stdlib.h>     // for strtol

// подключаем алгоритм
#include "algorithm.hpp"

using namespace std;
using namespace itis;

// абсолютный путь до набора данных и папки проекта
static constexpr auto kDatasetPath = string_view{PROJECT_DATASET_DIR};
static constexpr auto kProjectPath = string_view{PROJECT_SOURCE_DIR};

int main(int argc, char **argv) {

  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " NUMBER_OF_ELEMENTS" << " NUMBER_OF_DATASETS" << " NUMBER_OF_TRIALS" << std::endl;
  } else {
    // Строки для проверки ошибок
    char *elements_err;
    char *datasets_err;
    char *trials_err;
    int num_elem;
    int num_datasets;
    int num_trials;
    errno = 0;
    long dataset_elements = strtol(argv[1], &elements_err, 10);
    long datasets = strtol(argv[2], &datasets_err, 10);
    long trials = strtol(argv[3], &trials_err, 10);

    // Валидация аргументов
    if (errno != 0 || *elements_err != '\0' || dataset_elements > 1000000 || dataset_elements <= 0) {
      throw std::invalid_argument("Number of elements must be greater than zero and not greater than 1000000");
    } else {
      num_elem = dataset_elements;
    }

    if (errno != 0 || *datasets_err != '\0' || datasets > 5 || datasets <= 0) {
      throw std::invalid_argument("Number of datasets must be greater than zero and not greater than 5");
    } else {
      num_datasets = datasets;
    }

    if (errno != 0 || *trials_err != '\0' || trials <= 0) {
      throw std::invalid_argument("Number of trials must be greater than zero");
    } else {
      num_trials = trials;
    }

    const auto path = string(kDatasetPath);
    const auto path_project = string(kProjectPath);
    ofstream output_file;
    ifstream input_file;
    string str_num_elem;
    str_num_elem = to_string(num_elem);
    Graham* graham;

    // Тестируем каждый набор данных
    for (int i = 1; i <= num_datasets; i++) {
      // Открываем файлы
      output_file = ofstream(path_project + "/benchmark/test_results/" + str_num_elem + "/" + to_string(i) + ".csv");
      input_file = ifstream(path + "/" + str_num_elem + "/" + to_string(i) + ".csv");

      if (!input_file) {
        throw std::invalid_argument("No matching dataset.");
      }

      // Добавляем все точки из набора данных в структуру
      graham = new Graham();
      string join_line;
      for (int j = 0; j < num_elem; j++) {
        std::getline(input_file, join_line);
        int num1 = 0;
        int num2 = 0;
        auto create_set_ss = stringstream(join_line);
        create_set_ss >> num1;
        create_set_ss >> num2;
        graham->add_point(num1, num2);
      }

      double result;
      double average = 0;

      // Делаем нужное количество попыток
      for (int j = 0; j < num_trials; j++) {
        const auto time_point_before = chrono::high_resolution_clock::now();
        graham->convex_hull();
        const auto time_point_after = chrono::high_resolution_clock::now();
        const auto time_diff = time_point_after - time_point_before;
        const long time_elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(time_diff).count();
        result = time_elapsed_ns;
        result = result/1000000;
        average += result;
        output_file << result << endl;
      }

      // Выводим среднее значение, очищаем структуру, готовимся к следующему набору данных
      output_file << "Average: " << average/num_trials;
      delete graham;
      output_file.close();
      input_file.close();
    }
  }

  return 0;
}
