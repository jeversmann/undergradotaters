def run(name)
  target = Time.new.strftime(name + ".ll")
  `clang -emit-llvm -c samples/#{name}.c -o llvm/input/#{name}.bc`
  `opt -mem2reg llvm/input/#{name}.bc -o llvm/input/#{name}.bc`
  `llvm-dis llvm/input/#{name}.bc -o read/input/#{target}`
end

Dir.glob("samples/*.c") do |name|
  run(name.split("/")[1].split(".")[0])
end
