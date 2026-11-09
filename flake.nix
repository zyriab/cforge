{
  description = "cforge";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { nixpkgs, ... }:
    let
      platforms = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      forEachPlatform = nixpkgs.lib.genAttrs platforms;
    in
    {
      devShells = forEachPlatform (
        system:

        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              # C toolchain
              clang
              clang-tools # clangd, clang-format, clang-tidy
              llvm # llvm-symbolizer for ASan traces
              lldb
              gcc # cross-check builds
              gdb

              # Build system
              cmake
              ninja
              gnumake

              # Debugging / analysis
              valgrind

              # Version control
              git

              # Shell utilities
              coreutils
              which
            ];

            shellHook = ''
              export ASAN_SYMBOLIZER_PATH="${pkgs.llvm}/bin/llvm-symbolizer"
              echo ""
              echo "asciistl dev environment loaded"
              echo "  clang:  $(clang --version | head -n1)"
              echo "  cmake:  $(cmake --version | head -n1)"
              echo "  ninja:  $(ninja --version)"
              echo "  lldb:   $(lldb --version | head -n1)"
              echo ""
            '';
          };
        }
      );
    };
}
