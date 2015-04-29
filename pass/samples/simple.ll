; ModuleID = 'simple.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @simple(i32 %arg) #0 {
entry:
  %arg.addr = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %arg, i32* %arg.addr, align 4
  %0 = load i32* %arg.addr, align 4
  %add = add nsw i32 %0, 4
  store i32 %add, i32* %b, align 4
  %1 = load i32* %b, align 4
  call void @simple(i32 %1)
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.1 (tags/RELEASE_351/final)"}
