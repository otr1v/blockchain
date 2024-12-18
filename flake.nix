{
  description = "Portable blockhain framework with local discovery.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        nativePkgs = import nixpkgs { inherit system; };

        crossSystems = {
          arm64-linux = { config = "aarch64-linux"; };
        };

        buildInputs = pkgs: [
          pkgs.glibc.static
        ];

        nativeInputs = pkgs: [
          pkgs.buildPackages.cmake
        ];

        shellInputs = pkgs: [
          pkgs.clang-tools
          pkgs.mold
          pkgs.ninja
        ];

        mkDerivation = pkgs: pkgs.stdenv.mkDerivation {
          name = "blockchain";
          src = ./.;

          nativeBuildInputs = nativeInputs pkgs;
          buildInputs = buildInputs pkgs;

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=MinSizeRel"
            "-DCMAKE_C_FLAGS=-static"
            "-DCMAKE_CXX_FLAGS=-static"
          ];

          postInstall = ''
            ${pkgs.buildPackages.binutils}/bin/${pkgs.stdenv.cc.targetPrefix}strip "$out/bin/blockchain"
          '';
        };
      in
      {
        packages = {
          default = mkDerivation nativePkgs;

          cross = builtins.mapAttrs (crossName: crossSystem:
            mkDerivation (import nixpkgs {
              inherit system;
              inherit crossSystem;
            })
          ) crossSystems;
        };

        devShell = nativePkgs.mkShell {
          nativeBuildInputs =
            builtins.concatLists
              (builtins.map (input: input nativePkgs)
                [shellInputs buildInputs nativeInputs]
              );
        };
      }
    );
}
