//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// group.h
//
// Identification: src/include/optimizer/group.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <unordered_map>
#include <vector>

#include "optimizer/group_expression.h"
#include "optimizer/operator_node.h"
#include "optimizer/property.h"

namespace peloton {
namespace optimizer {

using GroupID = int32_t;

const GroupID UNDEFINED_GROUP = -1;

//===--------------------------------------------------------------------===//
// Group
//===--------------------------------------------------------------------===//
class Group {
 public:
  Group(GroupID id, std::unordered_set<std::string> table_alias);
  void add_item(Operator op);

  // If the GroupExpression is generated by applying a
  // property enforcer, we add them to enforced_exprs_
  // which will not be enumerated during OptimizeExpression
  void AddExpression(std::shared_ptr<GroupExpression> expr, bool enforced);

  void SetExpressionCost(std::shared_ptr<GroupExpression> expr, double cost,
                         PropertySet properties);

  std::shared_ptr<GroupExpression> GetBestExpression(PropertySet properties);

  const std::vector<std::shared_ptr<GroupExpression>> &GetExpressions() const;

  inline const std::unordered_set<std::string> &GetTableAliases() const {
    return table_aliases_;
  }

  inline void SetExplorationFlag() { has_explored_ = true; }
  inline bool HasExplored() { return has_explored_; }

  inline void SetImplementationFlag() { has_implemented_ = true; }
  inline bool HasImplemented() { return has_implemented_; }

 private:
  GroupID id_;
  // All the table alias this group represents. This will not change once create
  std::unordered_set<std::string> table_aliases_;
  std::vector<Operator> items_;
  std::vector<std::shared_ptr<GroupExpression>> expressions_;
  std::vector<std::shared_ptr<GroupExpression>> enforced_exprs_;
  std::unordered_map<PropertySet,
                     std::tuple<double, std::shared_ptr<GroupExpression>>>
      lowest_cost_expressions_;

  // Whether equivalent logical expressions have been explored for this group
  bool has_explored_;

  // Whether physical operators have been implemented for this group
  bool has_implemented_;
};

} // namespace optimizer
} // namespace peloton
