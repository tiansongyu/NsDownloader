
set_languages("c++17")

add_requires("cpr", "openssl")

add_packages("cpr")
add_links("pthread")
add_includedirs("include")

target("tn_test")
    set_kind("binary")
    add_files("tests/TinyNetdisk.cpp")