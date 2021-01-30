local lfs = require "lfs"

function exists(path)
        if not lfs.chdir(path) then
                return false
        else
                lfs.chdir("-")
                return true
        end
end 
print("building project")
if not exists("build") then
        print("build directory not found, building clean")
        lfs.mkdir("build")
end


lfs.chdir("./build")
if os.execute("cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1") then 
        print("Project generated")
        end

Target = "CubeExample"
print("Building target: "..Target)
if os.execute("cmake --build . --target "..Target.." -j 16") then
        os.execute("./"..Target)
end
