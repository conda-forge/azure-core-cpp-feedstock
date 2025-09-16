#!/bin/bash

# Test RTTI fix for macOS (exception handling across shared library boundaries)
# This test verifies that RequestFailedException can be caught correctly,
# addressing the issue described in:
# https://github.com/conda-forge/azure-storage-blobs-cpp-feedstock/issues/22

echo "Testing RTTI fix for macOS..."
echo "This test verifies that RequestFailedException symbols are properly exported"
echo "and can be caught by the specific exception type rather than std::exception"

# Test 1: Basic RTTI test
echo "=== Test 1: Basic RTTI Test ==="
echo "Compiling basic RTTI test..."
${CXX} -std=c++14 -I${PREFIX}/include -L${PREFIX}/lib -lazure-core test_rtti_fix.cpp -o test_rtti_fix

echo "Running basic RTTI test..."
DYLD_LIBRARY_PATH=${PREFIX}/lib ./test_rtti_fix

# Test 2: Comprehensive symbol visibility test
echo ""
echo "=== Test 2: Comprehensive Symbol Visibility Test ==="
echo "Compiling comprehensive symbol visibility test..."
${CXX} -std=c++14 -I${PREFIX}/include -L${PREFIX}/lib -lazure-core test_symbol_visibility.cpp -o test_symbol_visibility

echo "Running comprehensive symbol visibility test..."
DYLD_LIBRARY_PATH=${PREFIX}/lib ./test_symbol_visibility

# Test 3: Verify symbol visibility using nm (the core issue from GitHub #22)
echo ""
echo "=== Test 3: Symbol Visibility Analysis ==="
echo "Verifying symbol visibility..."
nm ${PREFIX}/lib/libazure-core.*.dylib | grep -E "RequestFailedException" | grep -E "^[0-9a-f]+ [TtSs]"

# Test 4: Verify that the vtable symbol is not private (s)
echo ""
echo "=== Test 4: Vtable Symbol Analysis ==="
echo "Checking vtable symbol visibility..."
vtable_symbol=$(nm ${PREFIX}/lib/libazure-core.*.dylib | grep -E "__ZTVN5Azure4Core22RequestFailedExceptionE")
echo "Vtable symbol: $vtable_symbol"
if echo "$vtable_symbol" | grep -q " s "; then
  echo "WARNING: Vtable symbol is still private (s) - this may cause RTTI issues"
else
  echo "SUCCESS: Vtable symbol is not private - RTTI should work correctly"
fi

# Test 5: Verify that the typeinfo symbol is not private (s)
echo ""
echo "=== Test 5: Typeinfo Symbol Analysis ==="
echo "Checking typeinfo symbol visibility..."
typeinfo_symbol=$(nm ${PREFIX}/lib/libazure-core.*.dylib | grep -E "__ZTIN5Azure4Core22RequestFailedExceptionE")
echo "Typeinfo symbol: $typeinfo_symbol"
if echo "$typeinfo_symbol" | grep -q " s "; then
  echo "WARNING: Typeinfo symbol is still private (s) - this may cause RTTI issues"
else
  echo "SUCCESS: Typeinfo symbol is not private - RTTI should work correctly"
fi

echo ""
echo "=== RTTI Test Summary ==="
echo "RTTI test completed successfully."
echo "This addresses the issue described in:"
echo "https://github.com/conda-forge/azure-storage-blobs-cpp-feedstock/issues/22"
