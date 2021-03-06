#include "storage_manager.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "operators/export_csv.hpp"
#include "operators/table_wrapper.hpp"
#include "optimizer/table_statistics.hpp"
#include "scheduler/job_task.hpp"
#include "utils/assert.hpp"

namespace opossum {

// singleton
StorageManager& StorageManager::get() {
  static StorageManager instance;
  return instance;
}

void StorageManager::add_table(const std::string& name, std::shared_ptr<Table> table) {
  Assert(_tables.find(name) == _tables.end(), "A table with the name " + name + " already exists");

  for (ChunkID chunk_id{0}; chunk_id < table->chunk_count(); chunk_id++) {
    Assert(table->get_chunk(chunk_id).has_mvcc_columns(), "Table must have MVCC columns.");
  }

  table->set_table_statistics(std::make_shared<TableStatistics>(table));
  _tables.insert(std::make_pair(name, std::move(table)));
}

void StorageManager::drop_table(const std::string& name) {
  auto num_deleted = _tables.erase(name);
  Assert(num_deleted == 1, "Error deleting table " + name + ": _erase() returned " + std::to_string(num_deleted) + ".");
}

std::shared_ptr<Table> StorageManager::get_table(const std::string& name) const {
  auto iter = _tables.find(name);
  Assert(iter != _tables.end(), "No such table named '" + name + "'");

  return iter->second;
}

bool StorageManager::has_table(const std::string& name) const { return _tables.count(name); }

std::vector<std::string> StorageManager::table_names() const {
  std::vector<std::string> table_names;
  table_names.reserve(_tables.size());

  for (const auto& table_item : _tables) {
    table_names.emplace_back(table_item.first);
  }

  return table_names;
}

void StorageManager::print(std::ostream& out) const {
  out << "==================" << std::endl;
  out << "===== Tables =====" << std::endl << std::endl;

  for (auto const& table : _tables) {
    out << "==== table >> " << table.first << " <<";
    out << " (" << table.second->column_count() << " columns, " << table.second->row_count() << " rows in "
        << table.second->chunk_count() << " chunks)";
    out << std::endl << std::endl;
  }
}

void StorageManager::reset() { get() = StorageManager(); }

void StorageManager::export_all_tables_as_csv(const std::string& path) {
  auto jobs = std::vector<std::shared_ptr<AbstractTask>>{};
  jobs.reserve(_tables.size());

  for (auto& pair : _tables) {
    auto job_task = std::make_shared<JobTask>([pair, &path]() {
      const auto& name = pair.first;
      auto& table = pair.second;

      auto table_wrapper = std::make_shared<TableWrapper>(table);
      table_wrapper->execute();

      auto export_csv = std::make_shared<ExportCsv>(table_wrapper, path + "/" + name + ".csv");
      export_csv->execute();
    });
    jobs.push_back(job_task);
    job_task->schedule();
  }

  for (auto& job : jobs) job->join();
}

}  // namespace opossum
