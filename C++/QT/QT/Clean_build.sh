rm -rf build

source .venv/bin/activate

conan install . --output-folder=build --build=missing -s build_type=Debug

cmake -S . -B build  -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug

 cmake --build build