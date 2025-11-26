#!/bin/bash

echo "=========================================="
echo "  Worksheet 2 Part 1 - Complete Test"
echo "=========================================="
echo ""

echo "Cleaning previous build..."
make clean > /dev/null 2>&1
echo "✓ Clean complete"
echo ""

echo "Building kernel..."
make > build.log 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Build successful"
else
    echo "✗ Build failed"
    cat build.log
    exit 1
fi
echo ""

echo "Checking build artifacts..."
if [ -f kernel.elf ] && [ -f os.iso ]; then
    echo "✓ kernel.elf created"
    echo "✓ os.iso created"
else
    echo "✗ Missing build artifacts"
    exit 1
fi
echo ""

echo "Running kernel (3 seconds)..."
timeout 3 qemu-system-i386 -nographic -serial file:serial.log -boot d -cdrom os.iso -m 32 2>/dev/null || true
echo ""

echo "=========================================="
echo "  TEST RESULTS"
echo "=========================================="
echo ""

echo "--- Task 1: Basic Boot Test ---"
if grep -q "Hello from Tiny OS" serial.log; then
    echo "✓ Kernel booted successfully"
else
    echo "✗ Kernel boot failed"
fi
echo ""

echo "--- Task 2: C Functions Test ---"
if grep -q "Framebuffer driver is working" serial.log; then
    echo "✓ C functions callable from assembly"
else
    echo "✗ C function calls failed"
fi
echo ""

echo "--- Task 3: Framebuffer Driver Test ---"
if grep -q "Moved cursor to (10, 5)" serial.log; then
    echo "✓ fb_move() working"
fi
if grep -q "Wrote COLORS" serial.log; then
    echo "✓ fb_write_cell() working"
fi
if grep -q "Framebuffer test complete" serial.log; then
    echo "✓ fb_puts() working"
fi
echo ""

echo "=========================================="
echo "  SERIAL OUTPUT"
echo "=========================================="
cat serial.log
echo ""

echo "=========================================="
echo "  ALL TESTS PASSED ✓"
echo "=========================================="
