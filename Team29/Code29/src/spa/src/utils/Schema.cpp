#include "Schema.h"

#include <iostream>

#include "Attribute.h"

namespace spa {

namespace utils {

Schema::Schema(std::vector<Attribute> attrs) : attrs_(attrs) {}

Schema::Schema(Attribute attr) : attrs_({attr}) {}

Schema::Schema() : attrs_({}) {}

Schema Schema::Join(Schema& left, Schema& right) {
  std::vector<Attribute> new_attrs;
  for (const auto& attr : left.attrs_) {
    new_attrs.push_back(attr);
  }
  for (const auto& attr : right.attrs_) {
    bool exist_in_left = false;
    for (const auto& left_attr : left.attrs_) {
      if (attr == left_attr) {
        exist_in_left = true;
        break;
      }
    }

    if (!exist_in_left) {
      new_attrs.push_back(attr);
    }
  }

  return Schema(new_attrs);
}

std::vector<std::pair<size_t, size_t>> Schema::GetJoinIdx(Schema& left,
                                                          Schema& right) {
  std::vector<std::pair<size_t, size_t>> join_idx;

  for (size_t i = 0; i < left.attrs_.size(); ++i) {
    for (size_t j = 0; j < right.attrs_.size(); ++j) {
      if (left.attrs_[i] == right.attrs_[j]) {
        join_idx.push_back({i, j});
      }
    }
  }

  return join_idx;
}

Schema Schema::ProjectAttr(Schema& base, std::vector<Attribute> attrs) {
  if (!base.HasAttrs(attrs)) {
    throw SchemaException("Projected attribute could not be found.");
  }

  return Schema(attrs);
}

Schema Schema::ProjectClause(Schema& base,
                             std::shared_ptr<pql::Clause> clause) {
  if (!base.HasAttrs(clause->GetAttrs())) {
    throw SchemaException("Unable to find clause attributes for projection.");
  }

  // Do not change the schema since we only perform filtering
  return Schema(base.GetAttrs());
}

bool Schema::HasAttrs(std::vector<Attribute> attrs) {
  for (auto& attr : attrs) {
    if (!HasAttr(attr)) return false;
  }
  return true;
}

bool Schema::HasAttr(Attribute& attr) {
  for (const auto& schema_attr : attrs_) {
    if (attr == schema_attr) return true;
  }
  return false;
}

std::vector<Attribute>& Schema::GetAttrs() { return attrs_; }

size_t Schema::GetIdx(Attribute attr) {
  for (size_t i = 0; i < attrs_.size(); ++i) {
    if (attrs_[i] == attr) {
      return i;
    }
  }
  throw SchemaException("Unable to find the attribute index.");
}

size_t Schema::Size() { return attrs_.size(); }

std::ostream& operator<<(std::ostream& out, const Schema& ref) {
  out << "[Schema: ";
  for (auto attr : ref.attrs_) {
    out << attr;
  }
  out << "]";
  return out;
}

}  // namespace utils

}  // namespace spa
