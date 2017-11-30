#pragma once

#include <string>

#include "SQLParserResult.h"
#include "concurrency/transaction_context.hpp"
#include "logical_query_plan/abstract_lqp_node.hpp"
#include "sql/sql_query_plan.hpp"
#include "storage/table.hpp"

namespace opossum {

/**
 * This is the unified interface to handle SQL queries and related operations.
 *
 * The basic idea of the SQLPipeline is that it represents the flow from the basic SQL string to the result table with
 * all intermediate steps. These intermediate steps call the previous step that is required. The intermediate results
 * are all cached so calling a method twice will return the already existing value.
 *
 * The SQLPipeline holds all results and only hands them out as const references. If the SQLPipeline goes out of scope
 * while the results are still needed, the result references are invalid (except maybe the result_table).
 *
 * E.g: calling sql_pipeline.get_result_table() will result in the following "call stack"
 * get_result_table -> get_tasks -> get_query_plan -> get_optimized_logical_plan -> get_parsed_sql
 */
class SQLPipeline : public Noncopyable {
 public:
  // An explicitly passed TransactionContext will result in no auto-commit and LQP validation.
  SQLPipeline(const std::string& sql, std::shared_ptr<TransactionContext> transaction_context);

  // If use_mvcc is true, a Transaction context is created with auto-commit and LQP validation.
  // If false, no validation or transaction management is done.
  explicit SQLPipeline(const std::string& sql, bool use_mvcc = true);

  const std::string sql_string() { return _sql_string; }

  // Returns the parsed SQL string.
  const hsql::SQLParserResult& get_parsed_sql();

  // Returns all unoptimized LQP roots.
  const std::vector<std::shared_ptr<AbstractLQPNode>>& get_unoptimized_logical_plan();

  // Returns all optimized LQP roots.
  const std::vector<std::shared_ptr<AbstractLQPNode>>& get_optimized_logical_plan();

  // For now, this always uses the optimized LQP.
  const SQLQueryPlan& get_query_plan();

  // Returns all tasks tht need to be executed for this query.
  const std::vector<std::shared_ptr<OperatorTask>>& get_tasks();

  // Executes all tasks, waits for them to finish, and returns the resulting table.
  const std::shared_ptr<const Table>& get_result_table();

  // Returns the TransactionContext that was either passed to or created by the SQLPipeline.
  // This can be a nullptr if no transaction management is wanted.
  const std::shared_ptr<TransactionContext>& transaction_context();

  float parse_time_seconds();
  float compile_time_seconds();
  float execution_time_seconds();

 private:
  const std::string _sql_string;

  // Execution results
  std::unique_ptr<hsql::SQLParserResult> _parsed_sql;
  std::vector<std::shared_ptr<AbstractLQPNode>> _unopt_logical_plan;
  std::vector<std::shared_ptr<AbstractLQPNode>> _opt_logical_plan;
  std::unique_ptr<SQLQueryPlan> _query_plan;
  std::vector<std::shared_ptr<OperatorTask>> _op_tasks;
  std::shared_ptr<const Table> _result_table;
  // Assume there is an output table. Only change if nullptr is returned from execution.
  bool _query_has_output = true;

  // Execution times
  float _parse_time_sec;
  float _compile_time_sec;
  float _execution_time_sec;

  // Transaction related
  const bool _use_mvcc;
  const bool _auto_commit;
  std::shared_ptr<TransactionContext> _transaction_context;
};

}  // namespace opossum
