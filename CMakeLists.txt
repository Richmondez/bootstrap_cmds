cmake_minimum_required(VERSION 3.20)

# Default to release for single target generators if nothing specified.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
endif()

project(migcom VERSION 1.22.0 LANGUAGES C)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser migcom.tproj/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(lexxer migcom.tproj/lexxer.l ${CMAKE_CURRENT_BINARY_DIR}/lexxer.c)
ADD_FLEX_BISON_DEPENDENCY(lexxer parser)

# flex runs in the binary dir, so these two includes need copying there to be found.
file(COPY migcom.tproj/lexxer.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY migcom.tproj/strdefs.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(migcom)
target_include_directories(migcom PRIVATE migcom.tproj)
target_compile_definitions(migcom PRIVATE MIG_VERSION="\\\"migcom ${migcom_VERSION_MAJOR}${migcom_VERSION_MINOR}\\\"")

target_sources(migcom PRIVATE
    migcom.tproj/alloc.h
    migcom.tproj/error.c
    migcom.tproj/error.h
    migcom.tproj/global.c
    migcom.tproj/global.h
    migcom.tproj/header.c
    migcom.tproj/lexxer.h
    migcom.tproj/mig.c
    migcom.tproj/mig_errors.h
    migcom.tproj/mig_machine.h
    migcom.tproj/routine.c
    migcom.tproj/routine.h
    migcom.tproj/server.c
    migcom.tproj/statement.c
    migcom.tproj/statement.h
    migcom.tproj/strdefs.h
    migcom.tproj/string.c
    migcom.tproj/type.c
    migcom.tproj/type.h
    migcom.tproj/user.c
    migcom.tproj/utils.c
    migcom.tproj/utils.h
    migcom.tproj/write.h
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexxer_OUTPUTS}
)

if(NOT APPLE)
    target_include_directories(migcom PRIVATE compat)
endif()

# Workaround in case the repo was cloned on windows for use in wsl
configure_file(migcom.tproj/mig.sh mig @ONLY NEWLINE_STYLE UNIX)

# Install/package the target
include(GNUInstallDirs)
install(TARGETS migcom DESTINATION ${CMAKE_INSTALL_BINDIR}/../libexec)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/mig RENAME mig TYPE BIN)
install(FILES migcom.tproj/mig.1 migcom.tproj/migcom.1 TYPE MAN)
