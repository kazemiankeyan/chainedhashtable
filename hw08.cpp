#include <iostream>
#include <fstream>
#include "ChainedHashTable.cpp"

void test_constructor(Hasher& hasher)
{
  ChainedHashTable *table = new ChainedHashTable(5000, hasher);

}

void test_copy_constructor(Hasher& hasher)
{

}

void test_hash(const char *file_name, Hasher& hasher)
{
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
}

int main()
{
  GeneralStringHasher h;
  test_hash("random.txt", h);
}
