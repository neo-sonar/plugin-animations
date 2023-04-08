cmake_minimum_required(VERSION 3.23)
project(animation-demo-dev VERSION 0.1.0 LANGUAGES C CXX)

find_program(CCACHE ccache)
if (CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif ()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(3rd_party/JUCE)
set_directory_properties(PROPERTIES JUCE_COMPANY_COPYRIGHT "Copyright 2023 digitaldreams.audio")
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME "digitaldreams.audio")

juce_add_gui_app(AnimationDemo PRODUCT_NAME "Animation Demo")

target_sources(AnimationDemo
    PRIVATE
        "src/AnimatedButton.cpp"
        "src/AnimatedButton.hpp"
        "src/AnimatedProperty.hpp"
        "src/AnimationTimer.hpp"
        "src/BannerCarousel.hpp"
        "src/CubicInterpolation.hpp"
        "src/Main.cpp"
        "src/MainComponent.hpp"
        "src/MainComponent.cpp"
        "src/TabSelector.hpp"
        "src/TabSelector.cpp"
        "src/Time.hpp"
        "src/TrimPathEffect.hpp"
        "src/TrimPathEffect.cpp"
)

target_compile_definitions(AnimationDemo
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:AnimationDemo,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:AnimationDemo,JUCE_VERSION>"
)


target_link_libraries(AnimationDemo
    PRIVATE
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)