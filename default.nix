{ pkgs ? import <nixpkgs> {} }:

with pkgs;
with stdenv.lib;

let
    makeSDLCFlags = lib.concatMap (p: [ "-I${getDev p}/include/SDL2" "-I${getDev p}/include" ]);
    makeSDLDFlags = lib.concatMap (p: [ "-L${getLib p}/lib" ]);
    SDLlibs = [ SDL2 SDL2_mixer ];

in pkgs.stdenv.mkDerivation {
    name = "systemshock";
    src = ./.;

    NIX_CFLAGS_COMPILE = (makeSDLCFlags SDLlibs);
    NIX_CFLAGS_LINK    = (makeSDLDFlags SDLlibs);

    installPhase = ''
        install -Dsm 644 systemshock $out/bin/systemshock
    '';
}
