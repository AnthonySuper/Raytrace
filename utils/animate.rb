class Animator
  def self.parse(fname)
    File.readlines(fname).map do |line|
      self.new(*line.split(/\s+/))
    end
  end

  def initialize(replace, init, final)
    @replace = replace
    @init = init.to_f
    @final = final.to_f
  end

  def animate(str, percent)
    str.gsub(@replace, amount(percent).to_s)
  end


  def amount(percent)
    @init + (stride * percent)
  end

  def stride
    @final - @init
  end
end

class Animation
  def initialize(file, animators, frames)
    @file = file 
    @animators = animators
    @frames = frames
  end

  def animate
    `mkdir out/`
    1.upto(@frames).each do |n|
      out_str = make_frame(n)
      IO.binwrite("tmp.txt", out_str)
      b = Time.now.to_f
      puts `./raytrace tmp.txt #{out_title(n)}`
      e = Time.now.to_f
      puts "Traced frame #{n} of #@frames in #{e - b} seconds..."
    end
  end

  def make_frame(n)
    @animators.inject(@file) do |acc, animator|
      animator.animate(acc, n.to_f / @frames)
    end
  end

  def out_title(frame)
    "out/frame-#{frame.to_s.rjust(pad, "0")}.ppm"
  end

  def pad
    @pad ||= @frames.to_s.length
  end
end

input = IO.binread(ARGV[0])
animators = Animator.parse(ARGV[1])
frames = (ARGV[2] || 30).to_i

Animation.new(input, animators, frames).animate
