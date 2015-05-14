def run(name)
  target = Time.new.strftime(name + ".ll")
  `clang -emit-llvm -c sample/#{name}.c -o llvm/input/#{name}.bc`
  `opt -mem2reg llvm/input/#{name}.bc -o llvm/input/#{name}.bc`
  `opt -instnamer llvm/input/#{name}.bc -o llvm/input/#{name}.bc`
  `llvm-dis llvm/output/#{name}.bc -o read/output/#{target}`
  `llvm-dis llvm/input/#{name}.bc -o read/input/#{target}`
end

`make`

Dir.glob("sample/*.c") do |name|
  run(name.split("/")[1].split(".")[0])
end
