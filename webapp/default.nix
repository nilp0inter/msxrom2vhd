{ kapowbang, msxrom2vhd, zsh, writeShellApplication, lib, writeTextFile, coreutils }:
let
  kapowScript = name: text: writeShellApplication { inherit name text; runtimeInputs = [ coreutils kapowbang ]; };
  convertEndpoint = writeTextFile {
   name = "convert-endpoint";
   text = ''
   set -x
   set -o errexit
   set -o nounset
   set -o pipefail
   PATH=${lib.makeBinPath [ coreutils msxrom2vhd kapowbang ]}
   inputfilename=$(kapow get /request/files/romFile/filename)
   outputfilename="${"$"}{inputfilename:r}.vhd"
   tmpfile="$(mktemp /tmp/myoutputfile.XXXXXX.vhd)"
   export LIBGUESTFS_BACKEND=direct
   msxrom2vhd =(kapow get /request/files/romFile/content) "$tmpfile"
   if [[ $? -ne 0 ]]; then
     kapow set /response/status 500
     kapow set /response/body <(echo "Failed to convert ROM file")
     exit 1
   fi
   kapow set /response/headers/Content-Disposition "attachment; filename=${"$"}{outputfilename}"
   kapow set /response/body < "$tmpfile"
   rm -f "$tmpfile"
   '';
   executable = true;
   destination = "/bin/convert-endpoint";
 };

  init = kapowScript "init" ''
    mkdir -p /tmp
    mkdir -p /var/tmp
    kapow route add -X GET -e '${lib.getExe zsh} -c' -c 'kapow set /response/body < ${./index.html}' '/'
    kapow route add -X POST -e '${lib.getExe zsh} -c' '/convert' ${lib.getExe convertEndpoint}
  '';
in
  kapowScript "start-server" ''
    kapow server --bind "0.0.0.0:''\${PORT:-8080}" --debug "${lib.getExe init}"
  ''
