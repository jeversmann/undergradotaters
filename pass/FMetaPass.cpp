#define DEBUG_TYPE "FMetaPass"

#include "FMetaPass.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>

#include <llvm/IR/Function.h>
#include <llvm/IR/Constant.h>
#include <llvm/Support/raw_ostream.h>

using namespace cs380c;
using namespace llvm;
using namespace rapidjson;

bool FMetaPass::runOnFunction(Function &f) {
    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    rapidjson::Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;
    return 0; 
  errs() << f.hasMetadata() << f;
  return true;
}

void FMetaPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
}

char FMetaPass::ID = 0;
static RegisterPass<FMetaPass>
    X("fmeta-pass",
      "Messes with Function metadata");
