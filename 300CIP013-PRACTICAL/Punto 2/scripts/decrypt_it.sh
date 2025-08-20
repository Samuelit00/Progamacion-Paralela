#!/usr/bin/env bash
# scripts/decrypt_it.sh
# Usage: ./decrypt_it.sh <rotation> <phrase...>
set -euo pipefail

if [[ $# -lt 2 ]]; then
  echo "Usage: $0 <rotation> <phrase...>" >&2
  exit 1
fi

k="$1"; shift
phrase="$*"

# decrypt = rotate by negative k
mod() { local a=$1 b=$2; local r=$((a % b)); [[ $r -lt 0 ]] && r=$((r + b)); echo $r; }
k=$(( -1 * k ))
k=$(mod "$k" 26)

awk -v K="$k" '
function mod(a,b){r=a%b; if(r<0) r+=b; return r}
function rot(c,k,   o){
  o = ord(c)
  if (o>=65 && o<=90) return sprintf("%c", 65 + mod((o-65)+k,26))
  if (o>=97 && o<=122) return sprintf("%c", 97 + mod((o-97)+k,26))
  return c
}
function ord(c){ return index("\0\1\2\3\4\5\6\7\10\11\12\13\14\15\16\17\20\21\22\23\24\25\26\27\30\31\32\33\34\35\36\37 !\"#$%&'\''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", c)-1 }
BEGIN{
  s = ARGV[1]
  for(i=1;i<=length(s);i++){ ch = substr(s,i,1); printf "%s", rot(ch,K) }
  printf "\n"
}' "$phrase"
