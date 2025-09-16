#include <azure/core/exception.hpp>
#include <iostream>
#include <exception>

int main() {
    try {
        // This should throw a RequestFailedException
        throw Azure::Core::RequestFailedException("Test exception for RTTI fix");
    } catch (const Azure::Core::RequestFailedException& e) {
        std::cout << "SUCCESS: Caught RequestFailedException correctly!" << std::endl;
        std::cout << "Message: " << e.what() << std::endl;
        std::cout << "RTTI fix is working - specific exception type was caught" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cout << "FAILED: Caught std::exception instead of RequestFailedException" << std::endl;
        std::cout << "Message: " << e.what() << std::endl;
        std::cout << "This indicates the RTTI fix did not work properly" << std::endl;
        return 1;
    } catch (...) {
        std::cout << "FAILED: Caught unknown exception" << std::endl;
        std::cout << "This indicates the RTTI fix did not work properly" << std::endl;
        return 1;
    }
    return 1;
}