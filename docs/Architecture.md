# TASOS Architecture

## Target Platform

TASOS initially targets x86_64 computers.

## Initial Boot Architecture

The first versions will use GRUB to load the kernel.

This allows early development to focus on kernel architecture rather than firmware and bootloader implementation.

A custom bootloader may be implemented later.

## Planned High-Level Components

1. Boot interface
2. Kernel entry layer
3. Terminal and logging
4. CPU exception handling
5. Hardware interrupt handling
6. Physical memory manager
7. Virtual memory manager
8. Kernel heap
9. Process and thread system
10. Scheduler
11. System-call interface
12. User-space runtime
13. Virtual filesystem
14. Storage drivers
15. Networking stack
16. Graphics and window management
17. Package and application system

## Design Principles

- Prefer simple and explicit implementations
- Document architectural decisions
- Separate architecture-specific and architecture-independent code
- Avoid unnecessary dependencies
- Build reusable subsystems
- Test components independently where practical
