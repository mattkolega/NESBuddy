add_rules("mode.debug", "mode.release")

set_languages("c++20")

add_requires("libsdl")

target("nesbuddy")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("libsdl")