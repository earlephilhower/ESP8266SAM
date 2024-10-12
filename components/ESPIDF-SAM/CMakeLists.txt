set(PROJECT_SOURCE_DIR src)

set(include_dirs
    ${PROJECT_SOURCE_DIR}
)

file(GLOB SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/*.c"
        "${PROJECT_SOURCE_DIR}/*.cpp"
        )

idf_component_register(SRCS "${SOURCE_FILES}"
                    INCLUDE_DIRS "${include_dirs}")

component_compile_options(-Wno-error=format= -Wno-format)