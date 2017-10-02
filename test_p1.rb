#!/usr/bin/env ruby
require 'fileutils'
require 'set'

`make`
FileUtils.cp("modeltoworld", "outputtest/modeltoworld")

Dir.chdir 'outputtest'

Dir["*driver**.txt"].each do |x| 
  puts "Processing #{x}..."
  `./modeltoworld #{x}`
end

Dir.chdir ".."

Dir["outputtest/driver**/"].each do |dir|
  Dir[dir + "*.obj"].each do |out|
    next unless /04/ =~ out
    nothing = dir.gsub("outputtest/", "")
    o2 = "./outputtest/test_transformed_models/#{nothing}" + File.basename(out)
    puts "FOR FILE #{out}"
    puts `./cbuild/model_compare #{o2} #{out}`
    puts "---\n"
  end
end
