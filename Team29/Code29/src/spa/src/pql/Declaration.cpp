#include "Declaration.h"

#include <iostream>

#include "QueryException.h"

namespace spa {

namespace pql {

Declaration::Declaration(Type type, std::string name)
    : type_{type}, val_type_{std::nullopt}, name_{name} {};

Declaration::Declaration(Type type, ValueType val_type, std::string name)
    : type_{type},
      val_type_{std::make_optional<ValueType>(val_type)},
      name_{name} {};

bool Declaration::operator==(const Declaration& other) const {
  return this->type_ == other.type_ && this->val_type_ == other.val_type_ &&
         this->name_ == other.name_;
}

bool Declaration::operator!=(const Declaration& other) const {
  return !(*this == other);
}

bool Declaration::IsStatement() const {
  return type_ != Type::kVariable && type_ != Type::kConstant &&
         type_ != Type::kProcedure;
}

bool Declaration::IsTypeIn(
    std::vector<Declaration::Type> allowed_decl_types) const {
  return std::find(allowed_decl_types.begin(), allowed_decl_types.end(),
                   this->type_) != allowed_decl_types.end();
}

bool Declaration::IsValueTypeIn(
    std::vector<Declaration::ValueType> allowed_decl_val_types) const {
  if (this->HasValue()) {
    return std::find(allowed_decl_val_types.begin(),
                     allowed_decl_val_types.end(),
                     this->val_type_) != allowed_decl_val_types.end();
  }
  return false;
}

bool Declaration::HasValue() const { return this->val_type_.has_value(); }

bool Declaration::HasName() const { return !this->name_.empty(); }

Declaration::Type Declaration::GetType() const { return type_; }

Declaration::ValueType Declaration::GetValueType() const {
  return val_type_.value();
}

std::string Declaration::GetName() const { return name_; }

Declaration::ReturnType Declaration::GetReturnType() const {
  if (!this->HasValue()) {
    if (this->IsStatement() || this->IsTypeIn({Declaration::Type::kConstant})) {
      return ReturnType::kInteger;
    }

    return ReturnType::kName;
  }

  return this->GetReturnTypeWithValue();
}

Declaration::ReturnType Declaration::GetReturnTypeWithValue() const {
  if (this->HasValue()) {
    // procedure.procName, call.procName, variable.varName, read.varName,
    // print.varName : NAME
    bool is_proc_name =
        this->IsValueTypeIn({Declaration::ValueType::kProcName}) &&
        this->IsTypeIn(
            {Declaration::Type::kProcedure, Declaration::Type::kCall});
    if (is_proc_name) return ReturnType::kName;

    bool is_var_name =
        this->IsValueTypeIn({Declaration::ValueType::kVarName}) &&
        this->IsTypeIn({Declaration::Type::kVariable, Declaration::Type::kRead,
                        Declaration::Type::kPrint});
    if (is_var_name) return ReturnType::kName;

    // constant.value, stmt.stmt#, read.stmt#, print.stmt#,
    // call.stmt#, while.stmt#, if.stmt#, assign.stmt#: INTEGER
    bool is_const_val = this->IsValueTypeIn({Declaration::ValueType::kValue}) &&
                        this->IsTypeIn({Declaration::Type::kConstant});
    if (is_const_val) return ReturnType::kInteger;

    bool is_stmt_num =
        this->IsValueTypeIn({Declaration::ValueType::kStmtNum}) &&
        this->IsStatement();
    if (is_stmt_num) return ReturnType::kInteger;
  }
  throw QueryParseException("Value of declaration is invalid");
}

std::ostream& operator<<(std::ostream& out, const Declaration& decl) {
  out << "[Declaration: type: " << kDeclTypeToString.at(decl.type_)
      << " value_type: ";
  if (decl.val_type_.has_value()) {
    out << kDeclValTypeToString.at(decl.val_type_.value());
  } else {
    out << "empty";
  }
  out << " name: " << decl.name_ << "]";
  return out;
}

const std::unordered_map<Declaration::Type, std::string> kDeclTypeToString = {
    {Declaration::Type::kStmt, "stmt"},
    {Declaration::Type::kRead, "read"},
    {Declaration::Type::kPrint, "print"},
    {Declaration::Type::kCall, "call"},
    {Declaration::Type::kWhile, "while"},
    {Declaration::Type::kIf, "if"},
    {Declaration::Type::kAssign, "assign"},
    {Declaration::Type::kVariable, "variable"},
    {Declaration::Type::kConstant, "constant"},
    {Declaration::Type::kProcedure, "procedure"}};

const std::unordered_map<std::string, Declaration::Type> kStringToDeclType = {
    {"stmt", Declaration::Type::kStmt},
    {"read", Declaration::Type::kRead},
    {"print", Declaration::Type::kPrint},
    {"call", Declaration::Type::kCall},
    {"while", Declaration::Type::kWhile},
    {"if", Declaration::Type::kIf},
    {"assign", Declaration::Type::kAssign},
    {"variable", Declaration::Type::kVariable},
    {"constant", Declaration::Type::kConstant},
    {"procedure", Declaration::Type::kProcedure},
    {"prog_line", Declaration::Type::kStmt}};

const std::unordered_map<Declaration::ValueType, std::string>
    kDeclValTypeToString = {{Declaration::ValueType::kProcName, "procName"},
                            {Declaration::ValueType::kVarName, "varName"},
                            {Declaration::ValueType::kValue, "value"},
                            {Declaration::ValueType::kStmtNum, "stmt#"}};

const std::unordered_map<std::string, Declaration::ValueType>
    kStringToDeclValType = {{"procName", Declaration::ValueType::kProcName},
                            {"varName", Declaration::ValueType::kVarName},
                            {"value", Declaration::ValueType::kValue},
                            {"stmt#", Declaration::ValueType::kStmtNum}};

}  // namespace pql

}  // namespace spa
