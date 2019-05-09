struct list_node
{
  std::string key;
  int value;
  list_node *next;

  list_node(std::string new_key, int new_value, list_node *new_next)
  {
   key = new_key;
   value = new_value;
   next = new_next;
  }
};
