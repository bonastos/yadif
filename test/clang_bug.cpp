#include <utility>
#include <iostream>

template <typename... Args>
struct IndexTest
{
  static std::size_t size() { return std::index_sequence_for<Args...>::size(); }  
};

int main()
{
  std::cout << std::index_sequence_for<int, int, int>::size() << " "
            << IndexTest<int, int, int>::size() << "\n";
}
