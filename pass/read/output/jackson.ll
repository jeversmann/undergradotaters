; ModuleID = 'llvm/output/jackson.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32* @dance(i32* %name) #0 {
entry:
  %conv = sext i32 5 to i64
  %0 = inttoptr i64 %conv to i32*
  ret i32* %0
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %thriller = alloca i32, align 4
  store i32 4, i32* %thriller, align 4
  %call = call i32* @dance(i32* %thriller)
  %0 = load i32, i32* %call, align 4
  ret i32 %0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (https://github.com/jeversmann/clang.git d61f6739d7a3bd637b85230eccf22a1072c62643) (https://github.com/llvm-mirror/llvm.git 039d60c2548f8f46d2051198cd5d803342d75e9f)"}
