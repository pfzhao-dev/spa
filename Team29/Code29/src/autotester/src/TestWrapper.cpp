#include "TestWrapper.h"

#include <fstream>
#include <streambuf>
#include <string>
#include <variant>

#include "ast/Node.h"
#include "ast/parser/Parser.h"
#include "opt/BushyTreesPlanner.h"
#include "opt/GreedyPlanner.h"
#include "opt/HeuristicPlanner.h"
#include "opt/SequentialPlanner.h"
#include "pkb/PKBRoot.h"
#include "pql/Parser.h"
#include "pql/QueryException.h"
#include "xtr/SourceExtractor.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
  std::ifstream t(filename);
  std::string s((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
  try {
    auto prog = spa::ast::Parser(s).Parse();

    spa::xtr::SourceExtractor extractor;
    extractor.Extract(prog);

  } catch (const std::exception& e) {
    printf("error: %s\n", e.what());
    // terminate autotester if parsing is not successful
    exit(0);
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  try {
    spa::pql::Parser parser;
    spa::pkb::PKBRoot::GetInstance()->ClearQueryCache();
    auto query_obj = parser.Parse(query);

    // spa::opt::SequentialPlanner planner;
    // spa::opt::BushyTreesPlanner planner;
    // spa::opt::GreedyPlanner planner;
    spa::opt::HeuristicPlanner planner;

    auto op = planner.Plan(query_obj, true);
    auto batch_result = op->Fetch();

    auto query_select_entities = query_obj.GetSelectEntities();
    auto boolean_query = query_select_entities.empty();

    if (boolean_query) {
      if (batch_result->Size() > 0) {
        results.push_back("TRUE");
      } else {
        results.push_back("FALSE");
      }
      return;
    }

    for (size_t i = 0; i < batch_result->Size(); ++i) {
      if (AbstractWrapper::GlobalStop) {
        return;
      }

      auto& tuple = batch_result->At(i);

      // Concatenate each element in the tuple to build a single string
      std::string s = "";
      for (size_t i = 0; i < tuple.size(); ++i) {
        if (i > 0) {
          s += " ";  // add white space between each element
        }

        auto elem = tuple[i];

        if (uint32_t* v = std::get_if<uint32_t>(&elem)) {
          s += std::to_string(*v);
        } else {
          s += std::get<std::string>(elem);
        }
      }

      results.push_back(s);
    }

  } catch (const std::exception& e) {
    // Return an empty result if there is any exception.
    printf("error: %s\n", e.what());
    results.clear();
    return;
  }
}
