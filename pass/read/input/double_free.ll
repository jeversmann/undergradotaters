; ModuleID = 'llvm/input/double_free.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d \00", align 1

; Function Attrs: nounwind uwtable
define i32 @fib(i32 %num) #0 {
entry:
  %retval = alloca i32, align 4
  %num.addr = alloca i32, align 4
  %array = alloca i32*, align 8
  %resp = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %num, i32* %num.addr, align 4
  %0 = load i32, i32* %num.addr, align 4
  %add = add nsw i32 %0, 1
  %conv = sext i32 %add to i64
  %mul = mul i64 %conv, 4
  %call = call noalias i8* @malloc(i64 %mul) #3
  %1 = bitcast i8* %call to i32*
  store i32* %1, i32** %array, align 8
  %cmp = icmp eq i32* null, %1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 -1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %2 = load i32, i32* %num.addr, align 4
  %cmp2 = icmp sle i32 %2, 1
  br i1 %cmp2, label %if.then4, label %if.else

if.then4:                                         ; preds = %if.end
  store i32 1, i32* %resp, align 4
  %3 = load i32*, i32** %array, align 8
  %4 = bitcast i32* %3 to i8*
  call void @free(i8* %4) #3
  br label %if.end6

if.else:                                          ; preds = %if.end
  %5 = load i32*, i32** %array, align 8
  %arrayidx = getelementptr inbounds i32, i32* %5, i64 0
  store i32 1, i32* %arrayidx, align 4
  %6 = load i32*, i32** %array, align 8
  %arrayidx5 = getelementptr inbounds i32, i32* %6, i64 1
  store i32 1, i32* %arrayidx5, align 4
  br label %if.end6

if.end6:                                          ; preds = %if.else, %if.then4
  store i32 2, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end6
  %7 = load i32, i32* %i, align 4
  %8 = load i32, i32* %num.addr, align 4
  %cmp7 = icmp slt i32 %7, %8
  br i1 %cmp7, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %9 = load i32, i32* %i, align 4
  %sub = sub nsw i32 %9, 1
  %idxprom = sext i32 %sub to i64
  %10 = load i32*, i32** %array, align 8
  %arrayidx9 = getelementptr inbounds i32, i32* %10, i64 %idxprom
  %11 = load i32, i32* %arrayidx9, align 4
  %12 = load i32, i32* %i, align 4
  %sub10 = sub nsw i32 %12, 2
  %idxprom11 = sext i32 %sub10 to i64
  %13 = load i32*, i32** %array, align 8
  %arrayidx12 = getelementptr inbounds i32, i32* %13, i64 %idxprom11
  %14 = load i32, i32* %arrayidx12, align 4
  %add13 = add nsw i32 %11, %14
  %15 = load i32, i32* %i, align 4
  %idxprom14 = sext i32 %15 to i64
  %16 = load i32*, i32** %array, align 8
  %arrayidx15 = getelementptr inbounds i32, i32* %16, i64 %idxprom14
  store i32 %add13, i32* %arrayidx15, align 4
  %17 = load i32, i32* %i, align 4
  %idxprom16 = sext i32 %17 to i64
  %18 = load i32*, i32** %array, align 8
  %arrayidx17 = getelementptr inbounds i32, i32* %18, i64 %idxprom16
  %19 = load i32, i32* %arrayidx17, align 4
  store i32 %19, i32* %resp, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %20 = load i32, i32* %i, align 4
  %inc = add nsw i32 %20, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %21 = load i32*, i32** %array, align 8
  %22 = bitcast i32* %21 to i8*
  call void @free(i8* %22) #3
  %23 = load i32, i32* %resp, align 4
  store i32 %23, i32* %retval
  br label %return

return:                                           ; preds = %for.end, %if.then
  %24 = load i32, i32* %retval
  ret i32 %24
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %array = alloca i32*, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %retval
  %call = call noalias i8* @malloc(i64 20) #3
  %0 = bitcast i8* %call to i32*
  store i32* %0, i32** %array, align 8
  %cmp = icmp eq i32* null, %0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 -1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %call1 = call i32 @fib(i32 6)
  %1 = load i32*, i32** %array, align 8
  %arrayidx = getelementptr inbounds i32, i32* %1, i64 0
  store i32 %call1, i32* %arrayidx, align 4
  %call2 = call i32 @fib(i32 17)
  %2 = load i32*, i32** %array, align 8
  %arrayidx3 = getelementptr inbounds i32, i32* %2, i64 1
  store i32 %call2, i32* %arrayidx3, align 4
  %call4 = call i32 @fib(i32 10)
  %3 = load i32*, i32** %array, align 8
  %arrayidx5 = getelementptr inbounds i32, i32* %3, i64 2
  store i32 %call4, i32* %arrayidx5, align 4
  %call6 = call i32 @fib(i32 3)
  %4 = load i32*, i32** %array, align 8
  %arrayidx7 = getelementptr inbounds i32, i32* %4, i64 3
  store i32 %call6, i32* %arrayidx7, align 4
  %call8 = call i32 @fib(i32 19)
  %5 = load i32*, i32** %array, align 8
  %arrayidx9 = getelementptr inbounds i32, i32* %5, i64 4
  store i32 %call8, i32* %arrayidx9, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %6 = load i32, i32* %i, align 4
  %cmp10 = icmp slt i32 %6, 5
  br i1 %cmp10, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %7 = load i32, i32* %i, align 4
  %idxprom = sext i32 %7 to i64
  %8 = load i32*, i32** %array, align 8
  %arrayidx11 = getelementptr inbounds i32, i32* %8, i64 %idxprom
  %9 = load i32, i32* %arrayidx11, align 4
  %call12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %9)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %10 = load i32, i32* %i, align 4
  %inc = add nsw i32 %10, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 0, i32* %retval
  br label %return

return:                                           ; preds = %for.end, %if.then
  %11 = load i32, i32* %retval
  ret i32 %11
}

declare i32 @printf(i8*, ...) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (https://github.com/jeversmann/clang.git d61f6739d7a3bd637b85230eccf22a1072c62643) (https://github.com/llvm-mirror/llvm.git 039d60c2548f8f46d2051198cd5d803342d75e9f)"}
