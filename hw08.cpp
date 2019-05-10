#include <iostream>
#include <fstream>
#include <ctime>
#include "ChainedHashTable.cpp"

// tests copy constructor
void test_copy_constructor(const char *file_name, Hasher& hasher)
{
  ChainedHashTable *table = new ChainedHashTable(5000, hasher);
  std::ifstream file;
  file.open(file_name);
  std::string word;

  while(file >> word)
  {
    table->insert(word);
  }

  ChainedHashTable *table_copy = table;

  //table_copy->print();

  delete table_copy;
}

// tests constructor, insertion, printing, and destructor
void test_hash(const char *file_name, Hasher& hasher)
{
  std::cout << "-------------------" << std::endl;
  std::cout << "TEST GENERAL HASH" << std::endl << std::endl;

  ChainedHashTable *table = new ChainedHashTable(5000, hasher);
  std::ifstream file;
  file.open(file_name);
  std::string word;

  while(file >> word)
  {
    table->insert(word);
  }

  table->print();

  delete table;
  std::cout << "-------------------" << std::endl;
}

// tests find, remove, and array operator
void test_others(const char *file_name, Hasher& hasher)
{
  ChainedHashTable *table = new ChainedHashTable(5000, hasher);
  std::ifstream file;
  file.open(file_name);
  std::string word;

  while(file >> word)
  {
    table->insert(word);
  }

  word = "earth";
  int count = table->find(word);

  std::cout << "-------------------" << std::endl;
  std::cout << "TEST OTHERS" << std::endl << std::endl;
  std::cout << "Before array bracket increment, find(\"earth\") = " << count << std::endl;

  (*table)[word]++;
  count = table->find(word);

  std::cout << "After table[\"earth\"]++, find(\"earth\") = " << count << std::endl;

  table->remove(word);
  std::cout << std::endl;
  std::cout << "After remove(\"earth\"): " << std::endl;
  try
  {
    table->find(word);
  }
  catch(const std::out_of_range& error)
  {
    std::cerr << std::endl << error.what() << std::endl;
  }

  std::cout << "-------------------" << std::endl << std::endl;

  delete table;
}

// Big O Average: O(N) Worst Case: O(N^2)
double insertAll(const char *file_name, ChainedHashTable& h, int n = 45000)
{
  std::ifstream file;
  file.open(file_name);
  std::string word;

  std::clock_t start = std::clock();

  for(int i = 0; file >> word && i < n; i++)
  {
    h.insert(word);
  }

  return (std::clock() - start) / (float) CLOCKS_PER_SEC;
}

// Big O Average: O(N) Worst Case: O(N^2)
double findAll(const char *file_name, ChainedHashTable& h, int n = 45000)
{
  std::ifstream file;
  file.open(file_name);
  std::string word;

  std::clock_t start = std::clock();

  for(int i = 0; file >> word && i < n; i++)
  {
    h.find(word);
  }

  return (std::clock() - start) / (float) CLOCKS_PER_SEC;
}

// Big O Average: O(N) Worst Case: O(N^2)
double removeAll(const char *file_name, ChainedHashTable& h, int n = 45000)
{
  std::ifstream file;
  file.open(file_name);
  std::string word;

  std::clock_t start = std::clock();

  for(int i = 0; file >> word && i < n; i++)
  {
    h.remove(word);
  }

  return (std::clock() - start) / (float) CLOCKS_PER_SEC;
}

void partition_test(Hasher& hasher)
{
  ChainedHashTable *table = new ChainedHashTable(5000, hasher);
  std::cout << "-------------------" << std::endl;
  std::cout << "RANDOM.TXT" << std::endl << std::endl;
  const char *file_one = "random.txt";
  int n = 4500;

  for(int i = 1; i <= 10; i++)
  {
    double duration_insert = insertAll(file_one, *table, n*i);
    std::cout << std::endl << " partition " << i << "/10 insertAll Time: " << duration_insert << "s" << std::endl << std::endl;
    double duration_find = findAll(file_one, *table, n*i);
    std::cout << std::endl << " partition " << i << "/10 findAll Time: " << duration_find << "s" << std::endl << std::endl;
    double duration_remove = removeAll(file_one, *table, n*i);
    std::cout << std::endl << " partition " << i << "/10 removeAll Time: " << duration_remove << "s" << std::endl << std::endl;
  }

  std::cout << "-------------------" << std::endl;
  std::cout << "-------------------" << std::endl;
  std::cout << "WORDS.TXT" << std::endl << std::endl;
  const char *file_two = "words.txt";
  int m = 4500;

  for(int j = 1; j <= 10; j++)
  {
    double duration_insert = insertAll(file_two, *table, m*j);
    std::cout << std::endl << " partition " << j << "/10 insertAll Time: " << duration_insert << "s" << std::endl << std::endl;
    double duration_find = findAll(file_two, *table, m*j);
    std::cout << std::endl << " partition " << j << "/10 findAll Time: " << duration_find << "s" << std::endl << std::endl;
    double duration_remove = removeAll(file_two, *table, m*j);
    std::cout << std::endl << " partition " << j << "/10 removeAll Time: " << duration_remove << "s" << std::endl << std::endl;
  }
  std::cout << "-------------------" << std::endl;

  delete table;
}

void hash_function_compare(Hasher& hasher, const char *hash_type)
{
  const char *file = "random.txt";

  ChainedHashTable *table = new ChainedHashTable(5000, hasher);

  double duration_insert = insertAll(file, *table);
  std::cout << hash_type << " CHAIN LENGTH STATS: " << std::endl << std::endl;
  std::cout << "min: " << table->min_chain_length() << " | max: " << table->max_chain_length() << " | avg: " << table->avg_chain_length() << " | st_dev: " << table->std_dev_chain() << std::endl;
  std::cout << std::endl << "insertAll Time: " << duration_insert << "s" << std::endl << std::endl;
  double duration_find = findAll(file, *table);
  std::cout << std::endl << "findAll Time: " << duration_find << "s" << std::endl << std::endl;
  double duration_remove = removeAll(file, *table);
  std::cout << std::endl << "removeAll Time: " << duration_remove << "s" << std::endl << std::endl;

  delete table;
}

int main()
{
  GeneralStringHasher h;

  test_hash("random.txt", h);
  test_copy_constructor("random.txt", h);
  test_others("random.txt", h);

  partition_test(h);

  BitShiftHasher h1;
  SumHasher h2;
  ProdHasher h3;

  hash_function_compare(h1, "BITSHIFTHASHER");
  hash_function_compare(h2, "SUMHASHER");
  hash_function_compare(h3, "PRODHASHER");
}
