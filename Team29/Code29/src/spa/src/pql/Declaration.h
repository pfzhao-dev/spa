#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace spa {

namespace pql {

class Declaration {
 public:
  // Declaration type according to the grammar of basic PQL
  enum class Type {
    kStmt = 0,
    kRead,
    kPrint,
    kCall,
    kWhile,
    kIf,
    kAssign,
    kVariable,
    kConstant,
    kProcedure,
  };

  // Value type of declaration
  enum class ValueType {
    kProcName = 0,
    kVarName,
    kValue,
    kStmtNum,
  };

  // Return type of declaration
  enum class ReturnType {
    kInteger = 0,
    kName,
  };

  // The `name` defaults to an empty string to better support wildcard type
  // that does not have any name.
  // std::optional<ValueType> will be an empty optional
  Declaration(Type type, std::string name = "");

  // The `name` defaults to an empty string to better support wildcard type
  // that does not have any name.
  Declaration(Type type, ValueType val_type, std::string name = "");

  bool operator==(const Declaration& other) const;
  bool operator!=(const Declaration& other) const;

  bool IsStatement() const;

  // returns true if  there is a declaration type in the allowed declaration
  // types that matches the declaration type of the declaration
  bool IsTypeIn(std::vector<Declaration::Type> allowed_decl_types) const;

  // returns true if there is a value type and there is a declaration value type
  // in the allowed declaration value types that matches the declaration value
  // type of the declaration
  bool IsValueTypeIn(
      std::vector<Declaration::ValueType> allowed_decl_val_types) const;

  bool HasValue() const;

  bool HasName() const;

  Type GetType() const;

  ValueType GetValueType() const;

  std::string GetName() const;

  ReturnType GetReturnType() const;
  // assumes that the declaration has a value and a return type
  ReturnType GetReturnTypeWithValue() const;

  friend std::ostream& operator<<(std::ostream& out, const Declaration& ref);

 private:
  Type type_;
  std::optional<ValueType> val_type_;
  std::string name_;
};

extern const std::unordered_map<Declaration::Type, std::string>
    kDeclTypeToString;
extern const std::unordered_map<std::string, Declaration::Type>
    kStringToDeclType;

extern const std::unordered_map<Declaration::ValueType, std::string>
    kDeclValTypeToString;
extern const std::unordered_map<std::string, Declaration::ValueType>
    kStringToDeclValType;

}  // namespace pql

}  // namespace spa
