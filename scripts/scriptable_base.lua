print("hello from the base scriptable")

wilo_defs = {
 testspath = "path"
}
function help()
for k,v in pairs(_G) do
 print(k,v)
 end
end 
print(wilo_defs.testspath)


