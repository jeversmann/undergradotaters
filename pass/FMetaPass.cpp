#define DEBUG_TYPE "FMetaPass"

#include "FMetaPass.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <cstdio>

#include <llvm/IR/Function.h>
#include <llvm/IR/Constant.h>
#include <llvm/Support/raw_ostream.h>

using namespace cs380c;
using namespace llvm;
using namespace rapidjson;

bool FMetaPass::runOnFunction(Function &f) {
  outs() << f.getName() << "\n";
  FILE *fp = fopen("out.txt", "r"); // non-Windows use "r"
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  Document d;
  d.ParseStream(is);
  fclose(fp);

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  StringBuffer buffer2;
  Writer<StringBuffer> writer2(buffer2);

  d["annotations"][0].Accept(writer);
  std::cout << buffer.GetString() << std::endl;
  d["annotations"][1].Accept(writer2);
  std::cout << buffer2.GetString() << std::endl;
  return 0;
}

void FMetaPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesCFG();
}

char FMetaPass::ID = 0;
static RegisterPass<FMetaPass> X("fmeta-pass", "Messes with Function metadata");
