#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "types.hpp"

namespace opossum {

class Table;

// The StorageManager is a singleton that maintains all tables
// by mapping table names to table instances.
class StorageManager : private Noncopyable {
 public:
  static StorageManager& get();

  // adds a table to the storage manager
  void add_table(const std::string& name, std::shared_ptr<Table> table);

  // removes the table from the storage manger
  void drop_table(const std::string& name);

  // returns the table instance with the given name
  std::shared_ptr<Table> get_table(const std::string& name) const;

  // returns whether the storage manager holds a table with the given name
  bool has_table(const std::string& name) const;

  // returns a list of all table names
  std::vector<std::string> table_names() const;

  // prints information about all tables in the storage manager (name, #columns, #rows, #chunks)
  void print(std::ostream& out = std::cout) const;

  // deletes the entire StorageManager and creates a new one, used especially in tests
  // This can lead to a lot of issues if there are still running tasks / threads that
  // want to access a resource. You should be very sure that this is what you want.
  // Have a look at base_test.hpp to see the correct order of resetting things.
  static void reset();

  // For debugging purposes mostly, dump all tables as csv
  void export_all_tables_as_csv(const std::string& path);

  StorageManager(StorageManager&&) = delete;

 protected:
  StorageManager() {}
  StorageManager& operator=(StorageManager&&) = default;

  std::map<std::string, std::shared_ptr<Table>> _tables;
};
}  // namespace opossum
