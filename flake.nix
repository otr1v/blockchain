{
  description = "Portable blockhain framework with local discovery.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          name = "blockchain";
          src = ./.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja

            glibc.static
          ];
        };
      }
    );
}
