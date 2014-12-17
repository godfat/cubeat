
local function power_factor(cubes, stepnum)

  if stepnum == 1 then
    if cubes >= 4 then
      return cubes - 1
    else
      return 0
    end
  else
    if cubes >= 3 then
      return (cubes - 1) * math.floor(stepnum/2) + (stepnum - 1)
    else
      return 0
    end
  end

end

local function print_power_table()

  for cubes = 3, 12 do
    io.write(string.format("c%-2d", cubes))
    for step = 1, 19 do
      io.write(string.format("%4d", power_factor(cubes, step)))
    end
    print()
  end

end

print_power_table()

