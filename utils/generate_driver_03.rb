require 'set'
POS_MAX = 38
O3_BASE = %Q{
eye #{3.times.map{POS_MAX + 10}.join(" ")}
look 0 0 0
up 0 1 0
d 4
bounds -2 -2 2 2
res 2000 2000
recursionLevel 10
ambient 0.2 0.2 0.2
}

def lights
  perms  = [
    [0, 0, 0],
    [100, 100, 100]
  ].to_set
  perms.merge([100, 0, 0].permutation.to_set)
  perms.merge([100, 100, 0].permutation.to_set)
  (perms.map do |l|
    "light #{l.join(" ")} 1 #{l.map{|x| x / 100.0}.join(" ")}"
  end).join("\n")
end

def spheres
  5.step(POS_MAX - 5, 5).map do |x|
    7.step(POS_MAX - 7, 7).map do |y|
      11.step(POS_MAX - 11, 11).map do |z|
        comps = [x, y, z]
        pos = comps.join(" ")
        color = comps.map{|x| x / 100.0 }.join(" ")
        rad = 1 # (1.0 - (x / POS_MAX.to_f)) * 2.0
        "sphere #{pos} #{rad} 0.2 0.2 0.2 #{color} #{color} #{color}"
      end.join("\n")
    end.join("\n")
  end.join("\n")
end

l = [O3_BASE,
     lights,
     spheres].join("\n")
puts l
IO.binwrite("driver03.txt", l)
