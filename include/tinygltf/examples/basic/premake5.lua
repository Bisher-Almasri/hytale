solution("basic_viewer")
-- location ( "build" )
configurations({ "Debug", "Release" })
platforms({ "native", "x64", "x32" })

project("basic_viewer")

kind("ConsoleApp")
language("C++")
cppdialect("C++11")
files({ "main.cpp", "shaders.cpp", "window.cpp" })
includedirs({ "./" })
includedirs({ "../../" })
includedirs({ "../common/glm" })

linkoptions({ "`pkg-config --libs glfw3`" })
links({ "GL", "GLU", "m", "GLEW", "X11", "Xrandr", "Xinerama", "Xi", "Xxf86vm", "Xcursor", "dl" })

defines({ "DEBUG" })
symbols("On")
warnings("Extra")
