; ModuleID = 'llvm/output/double_free.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d \00", align 1

; Function Attrs: nounwind uwtable
define i32 @fib(i32 %num) #0 {
entry:
  %add = add nsw i32 %num, 1
  %conv = sext i32 %add to i64
  %mul = mul i64 %conv, 4
  %call = call noalias i8* @malloc(i64 %mul) #3
  %tmp = bitcast i8* %call to i32*
  %cmp = icmp eq i32* null, %tmp
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %return

if.end:                                           ; preds = %entry
  %cmp2 = icmp sle i32 %num, 1
  br i1 %cmp2, label %if.then4, label %if.else

if.then4:                                         ; preds = %if.end
  %tmp1 = bitcast i32* %tmp to i8*
  call void @free(i8* %tmp1) #3
  br label %if.end6

if.else:                                          ; preds = %if.end
  %arrayidx = getelementptr inbounds i32, i32* %tmp, i64 0
  store i32 1, i32* %arrayidx, align 4
  %arrayidx5 = getelementptr inbounds i32, i32* %tmp, i64 1
  store i32 1, i32* %arrayidx5, align 4
  br label %if.end6

if.end6:                                          ; preds = %if.else, %if.then4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end6
  %resp.1 = phi i32 [ 1, %if.end6 ], [ %tmp4, %for.inc ]
  %i.0 = phi i32 [ 2, %if.end6 ], [ %inc, %for.inc ]
  %cmp7 = icmp slt i32 %i.0, %num
  br i1 %cmp7, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %sub = sub nsw i32 %i.0, 1
  %idxprom = sext i32 %sub to i64
  %arrayidx9 = getelementptr inbounds i32, i32* %tmp, i64 %idxprom
  %tmp2 = load i32, i32* %arrayidx9, align 4
  %sub10 = sub nsw i32 %i.0, 2
  %idxprom11 = sext i32 %sub10 to i64
  %arrayidx12 = getelementptr inbounds i32, i32* %tmp, i64 %idxprom11
  %tmp3 = load i32, i32* %arrayidx12, align 4
  %add13 = add nsw i32 %tmp2, %tmp3
  %idxprom14 = sext i32 %i.0 to i64
  %arrayidx15 = getelementptr inbounds i32, i32* %tmp, i64 %idxprom14
  store i32 %add13, i32* %arrayidx15, align 4
  %idxprom16 = sext i32 %i.0 to i64
  %arrayidx17 = getelementptr inbounds i32, i32* %tmp, i64 %idxprom16
  %tmp4 = load i32, i32* %arrayidx17, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %tmp5 = bitcast i32* %tmp to i8*
  call void @free(i8* %tmp5) #3
  br label %return

return:                                           ; preds = %for.end, %if.then
  %retval.0 = phi i32 [ -1, %if.then ], [ %resp.1, %for.end ]
  ret i32 %retval.0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %call = call noalias i8* @malloc(i64 20) #3
  %tmp = bitcast i8* %call to i32*
  %cmp = icmp eq i32* null, %tmp
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  br label %return

if.end:                                           ; preds = %entry
  %call1 = call i32 @fib(i32 6)
  %arrayidx = getelementptr inbounds i32, i32* %tmp, i64 0
  store i32 %call1, i32* %arrayidx, align 4
  %call2 = call i32 @fib(i32 17)
  %arrayidx3 = getelementptr inbounds i32, i32* %tmp, i64 1
  store i32 %call2, i32* %arrayidx3, align 4
  %call4 = call i32 @fib(i32 10)
  %arrayidx5 = getelementptr inbounds i32, i32* %tmp, i64 2
  store i32 %call4, i32* %arrayidx5, align 4
  %call6 = call i32 @fib(i32 3)
  %arrayidx7 = getelementptr inbounds i32, i32* %tmp, i64 3
  store i32 %call6, i32* %arrayidx7, align 4
  %call8 = call i32 @fib(i32 19)
  %arrayidx9 = getelementptr inbounds i32, i32* %tmp, i64 4
  store i32 %call8, i32* %arrayidx9, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %i.0 = phi i32 [ 0, %if.end ], [ %inc, %for.inc ]
  %cmp10 = icmp slt i32 %i.0, 5
  br i1 %cmp10, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %i.0 to i64
  %arrayidx11 = getelementptr inbounds i32, i32* %tmp, i64 %idxprom
  %tmp1 = load i32, i32* %arrayidx11, align 4
  %call12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %tmp1)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  br label %return

return:                                           ; preds = %for.end, %if.then
  %retval.0 = phi i32 [ -1, %if.then ], [ 0, %for.end ]
  ret i32 %retval.0
}

declare i32 @printf(i8*, ...) #2

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (https://github.com/jeversmann/clang.git d61f6739d7a3bd637b85230eccf22a1072c62643) (https://github.com/llvm-mirror/llvm.git 039d60c2548f8f46d2051198cd5d803342d75e9f)"}
