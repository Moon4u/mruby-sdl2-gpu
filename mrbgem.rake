MRuby::Gem::Specification.new('mruby-sdl2-gpu') do |spec|
  spec.license = 'MIT'
  spec.authors = 'moon4u'

  spec.add_dependency('mruby-sdl2')
  spec.cc.flags << '`sdl2-config --cflags`'
  spec.cc.flags << '-I/usr/local/lib/include/'
  spec.cc.flags << '-I/usr/include/'
  spec.cc.flags << '-I/usr/include/x86_64-linux-gnu/'
  spec.linker.flags_before_libraries << '-L/usr/local/lib -lSDL2_gpu'
  spec.linker.flags_before_libraries << '-L/usr/lib/x86_64-linux-gnu/ -lGLEW'
  spec.linker.flags_before_libraries << '`sdl2-config --libs`'
end
