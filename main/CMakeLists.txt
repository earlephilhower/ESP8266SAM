idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "${include_dirs}")

component_compile_options(-Wno-error=format= -Wno-format)