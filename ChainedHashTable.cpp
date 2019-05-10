#include <iostream>
#include <cmath>
#include "list_node.cpp"

struct Hasher
{
  // Big O: O(N) where N is size of string
  virtual int hash(std::string, int N) = 0;
};

struct GeneralStringHasher : Hasher
{
  // Big O: O(N) where N is size of string
  int hash(std::string key, int N)
  {
    unsigned hash_val = 0;
    for(int i = 0; i < key.size(); i++)
      hash_val = (127 * hash_val + key[i]) % 16908799;
    return hash_val % N;
  }
};

struct BitShiftHasher : Hasher
{
  // Big O: O(N) where N is size of string
  int hash(std::string key , int N)
  {
    const unsigned shift = 6;
    const unsigned zero = 0;
    unsigned mask = ~zero >> (32- shift ); // low 6 bits on
    unsigned result = 0;

    for (int i = 0; i < key.size (); i++)
      result = (result << shift) | (key[i] & mask );

    return result % N;
  }
};

struct SumHasher : Hasher
{
  // Big O: O(N) where N is size of string
  int hash(std::string s, int N)
  {
    int result = 0;
    for (int i=0; i<s.size (); ++i)
      result += s[i];
    return abs(result) % N;
  }
};

struct ProdHasher : Hasher
{
  // Big O: O(N) where N is size of string
  int hash(std::string s, int N)
  {
    int result = 1;
    for (int i=0; i<s.size (); ++i)
      result *= s[i];
    return abs(result) % N;
  }
};

class ChainedHashTable
{
  private:
    list_node **elements;
    Hasher *hasher;
    int capacity;

  public:
    ChainedHashTable(int new_capacity, Hasher& my_hasher)
    {
      elements = new list_node*[new_capacity];
      hasher = &my_hasher;
      capacity = new_capacity;

      for(int i = 0; i < capacity; i++)
        elements[i] = nullptr;
    }

    ChainedHashTable(const ChainedHashTable& table)
    {
      this->capacity = table.capacity;
      for(int i = 0; i < capacity; i++)
        elements[i] = nullptr;

      for(int i = 0; i < capacity; i++)
      {
        list_node *other_node = table.elements[i];
        list_node *this_node = elements[i];
        while(other_node)
        {
          this_node = new list_node(other_node->key, other_node->value, nullptr);
          other_node = other_node->next;
          this_node = this_node->next;
        }
      }
    }

    // Big O average: O(1) worst case: O(N)
    void insert(std::string word)
    {
      int index = hasher->hash(word, capacity);
      list_node *temp = elements[index];
      if(!temp)
        elements[index] = new list_node(word, 1, nullptr);
      else
      {
        while(temp->next && temp->next->key != word)
          temp = temp->next;
        if(!temp->next)
          temp->next = new list_node(word, 1, nullptr);
        else
          temp->next->value++;
      }
    }

    // Big O average: O(1) worst case: O(N)
    int find(std::string word)
    {
      return operator[](word);
    }

    // Big O average: O(1) worst case: O(N)
    void remove(std::string word)
    {
      int index = hasher->hash(word, capacity);
      list_node *temp  = elements[index];
      if(temp)
      {
        if(temp->key == word)
        {
          list_node *t = temp->next;
          delete temp;
          elements[index] = t;
        }
        else
        {
          while(temp->next && temp->next->key != word)
          {
            temp = temp->next;
          }
          if(temp->next->key == word)
          {
            list_node *t = temp->next->next;
            delete temp->next;
            temp->next = t;
          }
        }
      }
    }

    int chain_length(int index)
    {
      list_node *temp = elements[index];
      int count = 0;

      while(temp)
      {
        count++;
        temp = temp->next;
      }

      return count;
    }

    int min_chain_length()
    {
      int min = chain_length(0);

      for(int i = 1; i < capacity; i++)
      {
        int current_length = chain_length(i);

        if(current_length < min)
          min = current_length;
      }

      return min;
    }

    int max_chain_length()
    {
      int max = 0;

      for(int i = 0; i < capacity; i++)
      {
        int current_length = chain_length(i);

        if(current_length > max)
          max = current_length;
      }

      return max;
    }

    double avg_chain_length()
    {
      double sum = 0;

      for(int i = 0; i < capacity; i++)
        sum += static_cast<double>(chain_length(i));

      return sum / double(capacity);
    }

    double std_dev_chain()
    {
      double avg = avg_chain_length();
      double sum = 0;

      for(int i = 0; i < capacity; i++)
      {
        double current_length = static_cast<double>(chain_length(i));
        sum += ((current_length - avg ) * (current_length - avg));
      }

      return sqrt((sum / double(capacity)));
    }

    // Big O average: O(1) worst case: O(N)
    int& operator[](std::string word)
    {
      int index = hasher->hash(word, capacity);
      list_node *temp = elements[index];
      while(temp)
      {
        if(temp->key == word)
          return temp->value;

        temp = temp->next;
      }

      throw std::out_of_range("value not found");
    }

    void print()
    {
      for(int i = 0; i < capacity; i++)
      {
        std::cout << "[" << i << "]" << " |";
        list_node *temp = elements[i];
        while(temp)
        {
          std::cout << " -> " << temp->key << "(" << temp->value << ")";
          temp = temp->next;
        }
        std::cout << std::endl;
      }
    }

    ~ChainedHashTable()
    {
      for(int i = 0; i < capacity; i++)
      {
        list_node *t = elements[i];
        while(t)
        {
          list_node *next = t->next;
          delete t;
          t = next;
        }
      }
      delete[] elements;
    }
};
