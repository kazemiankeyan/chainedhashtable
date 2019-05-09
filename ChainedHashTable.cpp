#include <iostream>
#include "list_node.cpp"

struct Hasher
{
  virtual int hash(std::string, int N) = 0;
};

struct GeneralStringHasher : Hasher
{
  int hash(std::string key, int N)
  {
    unsigned hash_val = 0;
    for(int i = 0; i < key.size(); i++)
      hash_val = (127 * hash_val + key[i]) % 16908799;
    return hash_val % N;
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

    int find(std::string word)
    {
      return operator[](word);
    }

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
        while(temp->next)
        {
          if(temp->next->key == word)
          {
            list_node *t = temp->next->next;
            delete temp->next;
            temp->next = t;
          }
          temp = temp->next;
        }
      }
    }

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
