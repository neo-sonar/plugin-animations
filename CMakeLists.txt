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

add_subdirectory(3rd_party/sml)
add_subdirectory(3rd_party/JUCE)
set_directory_properties(PROPERTIES JUCE_COMPANY_COPYRIGHT "Copyright 2023 digitaldreams.audio")
set_directory_properties(PROPERTIES JUCE_COMPANY_NAME "digitaldreams.audio")

juce_add_gui_app(AnimationDemo PRODUCT_NAME "Animation Demo")

target_sources(AnimationDemo
    PRIVATE
        "src/neo/Main.cpp"
        "src/neo/MainComponent.hpp"
        "src/neo/MainComponent.cpp"

        "src/neo/animations/Animation.hpp"
        "src/neo/animations/AnimationDirection.hpp"
        "src/neo/animations/AnimationTimer.hpp"
        "src/neo/animations/AnimationTimer.cpp"
        "src/neo/animations/Keyframe.hpp"
        "src/neo/animations/KeyframeProperty.hpp"
        "src/neo/animations/KeyframeTraits.hpp"
        "src/neo/animations/TimingFunction.hpp"
        "src/neo/animations/Transition.hpp"
        "src/neo/animations/TransitionProperty.hpp"

        "src/neo/components/AnimatedButton.cpp"
        "src/neo/components/AnimatedButton.hpp"
        "src/neo/components/BannerCarousel.hpp"
        "src/neo/components/LoaderCarousel.hpp"
        "src/neo/components/MouseHoverListener.hpp"
        "src/neo/components/TabSelector.hpp"
        "src/neo/components/TabSelector.cpp"

        "src/neo/graphics/TrimPathEffect.hpp"
        "src/neo/graphics/TrimPathEffect.cpp"
)

target_compile_definitions(AnimationDemo
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:AnimationDemo,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:AnimationDemo,JUCE_VERSION>"
)

target_include_directories(AnimationDemo
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)


target_link_libraries(AnimationDemo
    PRIVATE
        juce::juce_gui_extra
    PUBLIC
        boost::sml
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
