conan install .\conanfile.txt -g MSBuildDeps --output-folder ".\x64\Release\.conan" -s build_type=Release -s compiler.runtime_type=Release -s compiler.cppstd=20 --build=missing --update
conan install .\conanfile.txt -g MSBuildDeps --output-folder ".\x64\Debug\.conan" -s build_type=Debug -s compiler.runtime_type=Debug -s compiler.cppstd=20 --build=missing --update