#pragma once

#include <string>

#include "abstract_lqp_node.hpp"

namespace opossum {

/**
 * This node type represents the CREATE VIEW management command.
 */
class CreateViewNode : public AbstractLQPNode {
 public:
  explicit CreateViewNode(const std::string& view_name, std::shared_ptr<const AbstractLQPNode> lqp);

  std::string description() const override;

  std::string view_name() const;
  std::shared_ptr<const AbstractLQPNode> lqp() const;

 protected:
  std::shared_ptr<AbstractLQPNode> _clone_impl() const override;
  const std::string _view_name;
  const std::shared_ptr<const AbstractLQPNode> _lqp;
};

}  // namespace opossum
