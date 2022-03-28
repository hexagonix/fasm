# flat assembler Hexagonix edition (fasmX)

fasmX is a fork of the flat assembler 1 (fasm) aiming to be fully compatible with Hexagonix and upgradable alongside the current version of fasm.

For more information, read the [license file](https://github.com/hexagonix/fasm/blob/main/LICENSE.TXT).

This repository holds the system-dependent code that allows fasm to run on the Hexagonix operating system. These contributions are maintained by Hexagonix project.

With the fasm port to Hexagonix, the system became self-hosting, that is, it can be mounted on top of itself. This makes it possible for users to mount both the system and applications and utilities directly on Hexagonix, not depending on another operating system. For this, a code layer was added to fasm that allows fasm to run with all its functions on top of Hexagonix. Contributions that allow fasm to run are available in this repository.

You can report any performance issues, bugs or errors [here](https://github.com/hexagonix/fasm/issues).

## flat assembler 1

The flat assembler (fasm) is an open-source, general-purpose assembler used as a standard for developing Hexagonix and its applications. fasmX is based on code available in the official fasm repository, available here, and is always updated after an official fasm release. For more information about fasm, visit the [official repository](https://github.com/tgrysztar/fasm) or the [project's website](https://flatassembler.net/).