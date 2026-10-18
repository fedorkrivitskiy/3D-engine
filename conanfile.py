from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake

class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("nlohmann_json/3.12.0")
        self.requires("glm/1.0.3")
        self.requires("tinygltf/2.9.7")
        self.requires("glfw/3.4")
        self.requires("glad/2.0.8")
        self.requires("tinyobjloader/2.0.0-rc10")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()