# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.18)
project(Plague)

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.1 REQUIRED)

set(SOURCES
  src/main.c
  src/core.c
  src/display_engine.c
  src/mutation_engine.c
  src/data.c
  src/epidemic_engine.c
  src/save.c
)

# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS

)
set(ASSETS_fx
  assets-fx/plague.png
  assets-fx/planes.png
  assets-fx/bground.png
  assets-fx/mutations.png
  assets-fx/cursor.png
  assets-fx/title.png
  assets-fx/dna.png
  assets-fx/explosion.png
  assets-fx/mutations_table/abilities_1.txt
  assets-fx/mutations_table/abilities_2.txt
  assets-fx/mutations_table/abilities_3.txt
  assets-fx/mutations_table/symptoms_1.txt
  assets-fx/mutations_table/symptoms_2.txt
  assets-fx/mutations_table/symptoms_3.txt
  assets-fx/mutations_table/symptoms_4.txt
  assets-fx/mutations_table/transmissions_1.txt
  assets-fx/mutations_table/transmissions_2.txt
  assets-fx/mutations_table/transmissions_3.txt
)
set(ASSETS_cg
  
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)
fxconv_declare_converters(assets-fx/converters.py)


add_executable(myaddin ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(myaddin PRIVATE -Wall -Wextra -Os)
target_link_libraries(myaddin Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET myaddin OUTPUT "Plague.g1a"
    NAME "Plague" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET myaddin OUTPUT "Plague.g3a"
    NAME "Plague" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
