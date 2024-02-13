set_project("NESBuddy")
set_version("0.1.0")

add_rules("mode.debug", "mode.release")

set_languages("c++20")

-- Dependencies --
add_requires(
    "catch2",
    "libsdl",
    "simdjson"
)

target("nesbuddy")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("libsdl")

target("cputest")
    set_kind("binary")
    set_default(false)
    add_files("test/test_CPU.cpp")
    add_packages("catch2", "simdjson")