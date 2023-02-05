{
  description = "msxrom2vhd";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system}; in
      rec {
        packages = rec {
          msxrom2vhd = with pkgs; stdenv.mkDerivation {
            name = "msxrom2vhd";
            version = "1.1";
            src = ./.;
            nativeBuildInputs = [ automake autoconf xxd ];
            buildInputs = [ libguestfs-with-appliance ];
            preConfigure = ''
              ./autoconf.sh
            '';
          };
        };
        defaultPackage = packages.msxrom2vhd;
#        apps.msxrom2vhd = flake-utils.lib.mkApp { drv = packages.msxrom2vhd; };
#        apps.default = apps.msxrom2vhd;
      }
    );
}
