// Copyright 2018 <Raymond Chan>

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono> // NOLINT (build/c++11)
#include <algorithm>

class ct;
class positional_vector;

int do_searches(const std::vector<int>& magnitudes,
                std::vector<positional_vector>* positional_vectors,
                ct* ct);
void commandline_error_check(int argc);

std::vector<positional_vector> read_vector_file(char filename[]);
std::vector<int> read_magnitude_file(char filename[]);
void write_file(char filename[], int count);

bool l_search_1(int magnitude,
                const std::vector<positional_vector>& positional_vectors);
int linear_search(const std::vector<int>& magnitudes,
                  std::vector<positional_vector>* positional_vectors,
                  ct* ct);

int b_search_1(int magnitude,
               const std::vector<positional_vector>& positional_vectors);
int binary_search(const std::vector<int>& magnitudes,
                  std::vector<positional_vector>* positional_vectors,
                  ct* ct);

class ct {
 public:
  ct();
  ~ct();

  void reset();
  double current_time();
  void stop_time();

 private:
  std::chrono::high_resolution_clock::time_point start;
  double end;
};
ct::ct() = default;
ct::~ct() = default;
void ct::reset() {
  start = std::chrono::high_resolution_clock::now();
}
void ct::stop_time() {
  auto now = std::chrono::high_resolution_clock::now();
  end =  std::chrono::duration<double, std::micro>(now - start).count();
}
double ct::current_time() {
  return end;
}

class positional_vector {
 public:
  positional_vector();
  explicit positional_vector(int x_coord, int y_coord);

  ~positional_vector();

  const int x() const;
  const int y() const;
  const int magnitude() const;

  bool operator <(const positional_vector& rhs) const;
  bool operator >(const positional_vector& rhs) const;

 private:
  int x_coord = 0;
  int y_coord = 0;

  int _magnitude = 0;
};
positional_vector::positional_vector() = default;
positional_vector::~positional_vector() = default;

positional_vector::positional_vector(int x_coord, int y_coord) :
    x_coord(x_coord), y_coord(y_coord),
    _magnitude(static_cast<int>(sqrt(pow(x_coord, 2) + pow(y_coord, 2)))) {}

bool positional_vector::operator<(const positional_vector &rhs) const {
  if (_magnitude == rhs._magnitude) {
    return y_coord < rhs.y_coord;
  }

  return _magnitude < rhs._magnitude;
}
bool positional_vector::operator>(const positional_vector &rhs) const {
  if (_magnitude == rhs._magnitude) {
    return y_coord < rhs.y_coord;
  }

  return _magnitude > rhs._magnitude;
}

const int positional_vector::x() const {
  return x_coord;
}
const int positional_vector::y() const {
  return y_coord;
}
const int positional_vector::magnitude() const {
  return _magnitude;
}


// std::ostream &operator <<(std::ostream& out,
//                           const std::vector<positional_vector>& vector) {
//   for(auto& element : vector) {
//     out << element.x() << ' ' << element.y() << std::endl;
//   }
//   return out;
// }
// std::ostream &operator <<(std::ostream& out,
//                           const std::vector<int>& vector) {
//   for(auto& element : vector) {
//     out << element << std::endl;
//   }
//   return out;
// }


int do_searches(const std::vector<int>& magnitudes,
                std::vector<positional_vector>* positional_vectors,
                ct* ct) {
  char input;

  do {
    std::cin >> input;

    switch (input) {
      case 'l':
        return linear_search(magnitudes, positional_vectors, ct);
      case 'b':
        return binary_search(magnitudes, positional_vectors, ct);
      default:
        std::cerr << "Incorrect choice" << std::endl;
        break;
    }
  } while (!(input == 'l' || input == 'b'));

  return -1;
}
void commandline_error_check(int argc) {
  if (argc < 4) {
    std::cerr << "Usage: ./vector_search "
        "<vector_file.dat> "
        "<magnitude_file.dat> "
        "<result_file.dat>";
    exit(0);
  }
}

// FIXME how to write these vectors more genetic?
std::vector<positional_vector> read_vector_file(char filename[]) {
  std::ifstream myfile;
  myfile.open(filename);
  if (!myfile.is_open()) {
    std::cerr << "Error: cannot open file " << filename;
    exit(0);
  }

  std::vector<positional_vector> vector;

  while (!myfile.eof()) {
    int x_coords;
    int y_coords;
    myfile >> x_coords;
    myfile >> y_coords;
    vector.emplace_back(x_coords, y_coords);
  }
  // FIXME Problem with eof() double reading file values
  vector.pop_back();

  myfile.close();
  return vector;
}
std::vector<int> read_magnitude_file(char filename[]) {
  std::ifstream myfile;
  myfile.open(filename);
  if (!myfile.is_open()) {
    std::cerr << "Error: cannot open file " << filename;
    exit(0);
  }

  std::vector<int> vector;

  while (!myfile.eof()) {
    int coords;
    myfile >> coords;
    vector.push_back(coords);
  }
  // FIXME Problem with eof() double reading file values
  vector.pop_back();

  myfile.close();
  return vector;
}
void write_file(char filename[], int count) {
  std::ofstream myfile;
  myfile.open(filename);
  if (!myfile.is_open()) {
    std::cerr << "Error: cannot open file " << filename;
    exit(0);
  }

  myfile << count;
  myfile.close();
}

bool l_search_1(int magnitude,
                const std::vector<positional_vector>& positional_vectors) {
  for (auto& result : positional_vectors) {
    if (result.magnitude() == magnitude) {
      return true;
    }
  }
  return false;
}
int linear_search(const std::vector<int>& magnitudes,
                  std::vector<positional_vector>* positional_vectors,
                  ct* ct) {
  ct->reset();
  int count = 0;

  for (int magnitude : magnitudes) {
    if (l_search_1(magnitude, *positional_vectors)) {
      count += 1;
    }
  }
  ct->stop_time();
  return count;
}

// From Lecture Slide #02, Page 27
int b_search_1(int magnitude,
               const std::vector<positional_vector>& positional_vectors) {
  int low = 0;
  int high = static_cast<int>(positional_vectors.size() - 1);

  while (low <= high) {
    int medium = low + (high - low)/2;
    int result = positional_vectors.at(medium).magnitude();

    if (result == magnitude) {
      return medium;
    }
    if (result < magnitude) {
      low = medium + 1;
    } else {
      high = medium - 1;
    }
  }

  return -1;
}
int binary_search(const std::vector<int>& magnitudes,
                  std::vector<positional_vector>* positional_vectors,
                  ct* ct) {
  ct->reset();

  std::sort(positional_vectors->begin(), positional_vectors->end());

  int count = 0;

  for (int magnitude : magnitudes) {
    if (b_search_1(magnitude, *positional_vectors) != -1) {
      count += 1;
    }
  }
  ct->stop_time();
  return count;
}

int main(int argc, char *argv[]) {
  commandline_error_check(argc);
  ct ct;

  std::vector<positional_vector> positional_vectors = read_vector_file(argv[1]);
  std::vector<int> magnitudes = read_magnitude_file(argv[2]);

//  std::cout << magnitudes << std::endl;
//  std::cout << positional_vectors << std::endl;

  std::cout << "Choice of search method ([l]inear, [b]inary)?"
            << std::endl;
  int count = do_searches(magnitudes, &positional_vectors, &ct);

  write_file(argv[3], count);
  std::cout << "CPU time: "
            << ct.current_time()
            << " microseconds"
            << std::endl;

  return 0;
}
