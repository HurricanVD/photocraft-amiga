#!/bin/sh
set -eu

repo_root=${1:-.}
metadata="$repo_root/docs/project-metadata.env"

if [ ! -f "$metadata" ]; then
	echo "Missing docs/project-metadata.env" >&2
	exit 1
fi

read_metadata() {
	awk '
		function store(line) {
			sub(/^[[:space:]]*/, "", line)
			name = line
			sub(/[[:space:]]*=.*/, "", name)
			sub(/^[^=]*=/, "", line)
			sub(/^[[:space:]]*/, "", line)
			sub(/[[:space:]]*$/, "", line)
			sub(/^"/, "", line)
			sub(/"$/, "", line)
			if (name == "VD_PROCESS_REPO") {
				process_repo = line
			} else if (name == "VD_PROCESS_VERSION") {
				expected_version = line
			}
		}
		$0 ~ /^[[:space:]]*VD_PROCESS_REPO[[:space:]]*=[[:space:]]*"?[^"]*"?[[:space:]]*$/ {
			store($0)
			next
		}
		$0 ~ /^[[:space:]]*VD_PROCESS_VERSION[[:space:]]*=[[:space:]]*"?[^"]*"?[[:space:]]*$/ {
			store($0)
			next
		}
		END {
			printf "%s\n%s\n", process_repo, expected_version
		}
	' "$metadata"
}

metadata_values=$(read_metadata)
nl='
'
process_repo=${metadata_values%%"$nl"*}
if [ "$process_repo" = "$metadata_values" ]; then
	expected_version=""
else
	expected_version=${metadata_values#*"$nl"}
fi

if [ -z "$process_repo" ]; then
	echo "VD_PROCESS_REPO is not set" >&2
	exit 1
fi
if [ -z "$expected_version" ]; then
	echo "VD_PROCESS_VERSION is not set" >&2
	exit 1
fi

lower_drive() {
	case "$1" in
		A|a) printf 'a' ;;
		B|b) printf 'b' ;;
		C|c) printf 'c' ;;
		D|d) printf 'd' ;;
		E|e) printf 'e' ;;
		F|f) printf 'f' ;;
		G|g) printf 'g' ;;
		H|h) printf 'h' ;;
		I|i) printf 'i' ;;
		J|j) printf 'j' ;;
		K|k) printf 'k' ;;
		L|l) printf 'l' ;;
		M|m) printf 'm' ;;
		N|n) printf 'n' ;;
		O|o) printf 'o' ;;
		P|p) printf 'p' ;;
		Q|q) printf 'q' ;;
		R|r) printf 'r' ;;
		S|s) printf 's' ;;
		T|t) printf 't' ;;
		U|u) printf 'u' ;;
		V|v) printf 'v' ;;
		W|w) printf 'w' ;;
		X|x) printf 'x' ;;
		Y|y) printf 'y' ;;
		Z|z) printf 'z' ;;
		*) printf '%s' "$1" ;;
	esac
}

normalize_windows_drive_path() {
	value=$1
	drive=${value%%:*}
	rest=${value#?:}
	case "$rest" in
		*\\*)
			rest=$(printf '%s' "$rest" | tr '\\' '/')
			;;
	esac
	printf '/%s%s\n' "$(lower_drive "$drive")" "$rest"
}

case "$process_repo" in
	/*)
		process_root=$process_repo
		;;
	?:/*|?:\\*)
		process_root=$process_repo
		if [ ! -d "$process_root" ]; then
			process_root=$(normalize_windows_drive_path "$process_repo")
		fi
		;;
	*)
		process_root="$repo_root/$process_repo"
		if [ ! -d "$process_root" ]; then
			process_root="$(dirname "$repo_root")/$process_repo"
		fi
		;;
esac

version_file="$process_root/VERSION"
if [ ! -f "$version_file" ]; then
	echo "Process VERSION not found: $version_file" >&2
	exit 1
fi

actual_version=$(tr -d '\r\n' < "$version_file")
if [ "$actual_version" != "$expected_version" ]; then
	echo "Process version drift: expected $expected_version, got $actual_version from $version_file" >&2
	exit 1
fi

echo "process version check: PASS"
echo "version: $actual_version"
