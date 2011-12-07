
-- temporary C interface header to LuaJIT FFI cdefs command script

local stub = io.open("stub.c", "w")
stub:write("#include <script/"..arg[1]..".h>")
stub:close()

local str2 = "gcc -I include -E stub.c | grep -v '^#' > rc/script/"..arg[1]..".ffi"
print(str2)

os.execute(str2)
os.execute"rm stub.c"
print 'done'
