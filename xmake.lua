set_project("NESBuddy")
set_version("0.1.0")

set_languages("c++20")

add_rules("mode.debug", "mode.release")

-- Dependencies --
add_requires(
    "catch2",
    "libsdl",
    "nativefiledialog-extended",
    "nlohmann_json"
)

target("nesbuddy")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("libsdl", "nativefiledialog-extended")
    if is_mode("debug") then
        add_ldflags("/subsystem:console")  -- Needed for stdout and stderr to console 
    end

target("cputest")
    set_kind("binary")
    set_default(false)
    add_files("test/test_CPU.cpp")
    add_files("src/NES.cpp", "src/CPU/**.cpp", "src/Cartridge/**.cpp")
    add_packages("catch2", "nlohmann_json", "nativefiledialog-extended")