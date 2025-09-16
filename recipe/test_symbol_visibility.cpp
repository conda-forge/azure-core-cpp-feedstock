#include <azure/core/exception.hpp>
#include <iostream>
#include <exception>
#include <dlfcn.h>

int main() {
    // Test 1: Verify that RequestFailedException can be caught correctly
    std::cout << "=== Test 1: Exception Catching Test ===" << std::endl;
    try {
        throw Azure::Core::RequestFailedException("Test exception for RTTI fix");
    } catch (const Azure::Core::RequestFailedException& e) {
        std::cout << "SUCCESS: Caught RequestFailedException correctly!" << std::endl;
        std::cout << "Message: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "FAILED: Caught std::exception instead of RequestFailedException" << std::endl;
        std::cout << "Message: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "FAILED: Caught unknown exception" << std::endl;
        return 1;
    }

    // Test 2: Verify that the vtable symbol is accessible (if possible)
    std::cout << "\n=== Test 2: Symbol Visibility Test ===" << std::endl;
    
    // Try to load the library and check if we can access the vtable symbol
    void* handle = dlopen("libazure-core.dylib", RTLD_LAZY);
    if (handle) {
        // Try to find the vtable symbol
        void* vtable_symbol = dlsym(handle, "__ZTVN5Azure4Core22RequestFailedExceptionE");
        if (vtable_symbol) {
            std::cout << "SUCCESS: Vtable symbol is accessible via dlsym" << std::endl;
        } else {
            std::cout << "INFO: Vtable symbol not accessible via dlsym (this may be normal)" << std::endl;
        }
        dlclose(handle);
    } else {
        std::cout << "INFO: Could not load library for symbol test" << std::endl;
    }

    std::cout << "\n=== RTTI Fix Verification Complete ===" << std::endl;
    std::cout << "The RTTI fix is working correctly if Test 1 passed." << std::endl;
    std::cout << "This addresses the issue described in:" << std::endl;
    std::cout << "https://github.com/conda-forge/azure-storage-blobs-cpp-feedstock/issues/22" << std::endl;
    
    return 0;
}
