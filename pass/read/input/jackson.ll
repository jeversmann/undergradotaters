; ModuleID = 'llvm/input/jackson.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @dance(i32* %name) #0 {
entry:
  %name.addr = alloca i32*, align 8
  %bad = alloca i32, align 4
  %terri = alloca i32*, align 8
  %only = alloca i32*, align 8
  store i32* %name, i32** %name.addr, align 8
  store i32 5, i32* %bad, align 4
  store i32* %bad, i32** %terri, align 8
  %0 = load i32*, i32** %name.addr, align 8
  store i32* %0, i32** %only, align 8
  %1 = load i32*, i32** %terri, align 8
  %2 = load i32, i32* %1, align 4
  %3 = load i32*, i32** %only, align 8
  %4 = load i32, i32* %3, align 4
  %mul = mul nsw i32 %2, %4
  ret i32 %mul
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %thriller = alloca i32, align 4
  %name = alloca i32*, align 8
  %mj = alloca i32, align 4
  %ptr = alloca i32*, align 8
  store i32 0, i32* %retval
  store i32 4, i32* %thriller, align 4
  store i32* %thriller, i32** %name, align 8
  %0 = load i32*, i32** %name, align 8
  %call = call i32 @dance(i32* %0)
  store i32 %call, i32* %mj, align 4
  store i32* %mj, i32** %ptr, align 8
  %1 = load i32*, i32** %ptr, align 8
  %2 = load i32, i32* %1, align 4
  ret i32 %2
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (https://github.com/jeversmann/clang.git d61f6739d7a3bd637b85230eccf22a1072c62643) (https://github.com/llvm-mirror/llvm.git 039d60c2548f8f46d2051198cd5d803342d75e9f)"}
