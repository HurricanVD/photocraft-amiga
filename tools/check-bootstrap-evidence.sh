#!/bin/sh
# PF bootstrap evidence: read-only checks plus an isolated disposable bootstrap smoke.
set -eu

repo_root=$(CDPATH= cd "$(dirname "$0")/.." && pwd)
process_path="$repo_root/../../tools/vd-amiga-dev-process"

for required in \
  README.md docs/vision.md docs/roadmap.md docs/architecture.md \
  docs/project-metadata.env docs/bootstrap-report.md docs/development-process.md \
  docs/backlog.md docs/backlog-done.md docs/bootstrap/first-phase-backlog.md \
  tools/check-process-version.sh tools/check-process-version.ps1 \
  docs/stories/PF-SP-001.md docs/stories/PF-SP-002.md docs/stories/done/PF-SP-003.md \
  docs/tests/PF-TN-001.md docs/tests/PF-TN-002.md docs/tests/PF-TN-004.md \
  docs/tests/done/PF-TN-003.md \
  docs/reviews/PF-SP-002-atomization-report-2026-10-11.md \
  docs/stories/PF-SP-004.md docs/stories/PF-SP-005.md docs/stories/PF-SP-006.md \
  docs/stories/PF-SP-007.md docs/stories/PF-SP-008.md docs/stories/PF-SP-009.md \
  docs/stories/PF-TD-001.md
do
  if [ ! -f "$repo_root/$required" ]; then
    echo "FAIL missing consumer file: $required" >&2
    exit 1
  fi
done

grep -q '^VD_PROCESS_VERSION=0\.2\.1$' "$repo_root/docs/project-metadata.env" || { echo 'FAIL wrong process pin' >&2; exit 1; }
grep -q '^STORY_ID_PREFIX=PF$' "$repo_root/docs/project-metadata.env" || { echo 'FAIL wrong story prefix' >&2; exit 1; }
grep -q '^TEST_ID_PREFIX=PF$' "$repo_root/docs/project-metadata.env" || { echo 'FAIL wrong test prefix' >&2; exit 1; }
if grep -n 'VC-PO-001' "$repo_root/docs/backlog.md" "$repo_root/docs/backlog-done.md" "$repo_root/docs/bootstrap/first-phase-backlog.md"; then
  echo 'FAIL foreign prefix example in PF backlog' >&2
  exit 1
fi
for field in 'Prozessversion' 'Story-ID-Prefix' 'Test-ID-Prefix' 'Build' 'Windows-WSL2-Produktbuild' 'WSL-Distribution' 'Starter-App-Build' 'Starter-App-Build-Evidenz' 'Build-Artefakt' 'Full Check PowerShell' 'Full Check Bash/Make' 'Hosttest fuer OS-freie Logik' 'm68k-Test-Harness-Build' 'Vamos-Test' 'manueller WinUAE-/Hardwaretest' 'Test-/Build-Log-Verzeichnisse' 'Release-Build' 'Skill-/Tool-Freshness' 'Abgeschlossene Storys' 'WIP-Limit' 'Story-ID-Beispiel' 'Fastpath' 'Lokale Fastpath-Verschaerfungen'; do
  if grep -q -- "^- $field:[[:space:]]*$" "$repo_root/docs/development-process.md"; then
    echo "FAIL empty overlay field: $field" >&2
    exit 1
  fi
done
grep -q 'PF-SP-001.*blocked' "$repo_root/docs/backlog.md" || { echo 'FAIL PF-SP-001 status changed' >&2; exit 1; }
grep -q 'PF-SP-002.*blocked' "$repo_root/docs/backlog.md" || { echo 'FAIL PF-SP-002 split freeze missing' >&2; exit 1; }
grep -q '^- Status: blocked' "$repo_root/docs/stories/PF-SP-002.md" || { echo 'FAIL PF-SP-002 canonical freeze missing' >&2; exit 1; }
grep -q 'decision: `split_required`' "$repo_root/docs/reviews/PF-SP-002-atomization-report-2026-10-11.md" || { echo 'FAIL retrospective atomization decision missing' >&2; exit 1; }
for id in PF-SP-004 PF-SP-005 PF-SP-006 PF-SP-007 PF-SP-008 PF-SP-009 PF-TD-001; do
  grep -Eq '^- Status: `(draft|refining)`' "$repo_root/docs/stories/$id.md" || {
    echo "FAIL child story $id promoted without DoR" >&2
    exit 1
  }
  grep -Eq "$id.*(draft|refining)" "$repo_root/docs/backlog.md" || {
    echo "FAIL child story $id missing/refinement mismatch in backlog" >&2
    exit 1
  }
done
grep -q 'PF-SP-003.*2026-10-09' "$repo_root/docs/backlog-done.md" || { echo 'FAIL PF-SP-003 missing from done' >&2; exit 1; }
echo 'STATIC_BOOTSTRAP_DOCS=PASS'

# Public fork CI may run only the static gate; full VD checks run in the
# private shared-process repository because its sources are not public.
if [ "${1:-}" = '--static-only' ]; then
  exit 0
fi

if [ ! -f "$process_path/VERSION" ]; then
  echo "POSIX_DRIFT=RUNTIME_BLOCKED (missing process checkout: $process_path)" >&2
  exit 2
fi
process_root=$(CDPATH= cd "$process_path" && pwd)
[ "$(tr -d '\r\n' < "$process_root/VERSION")" = '0.2.1' ] || { echo 'FAIL process VERSION is not 0.2.1' >&2; exit 1; }
# Both states are valid: reserved before formal activation, active afterward.
# A missing/unallocated PF entry must always fail.
grep -Eq '^\|[[:space:]]*`PF`[[:space:]]*\|[[:space:]]*`photocraft-amiga`[[:space:]]*\|.*\|[[:space:]]*(reserved|active)[[:space:]]*\|' "$process_root/docs/process/workspace-id-prefixes.md" || { echo 'FAIL PF registry is not reserved or active for photocraft-amiga' >&2; exit 1; }

sh "$repo_root/tools/check-process-version.sh" "$repo_root"
echo 'POSIX_DRIFT=PASS'

if command -v pwsh >/dev/null 2>&1; then
  pwsh -NoProfile -File "$repo_root/tools/check-process-version.ps1" -RepoRoot "$repo_root"
  echo 'POWERSHELL_DRIFT=PASS'
  pwsh -NoProfile -File "$process_root/tools/test-bootstrap-repo.ps1"
  echo 'POWERSHELL_BOOTSTRAP_FIXTURE=PASS'
else
  echo 'POWERSHELL_DRIFT=RUNTIME_BLOCKED (pwsh not installed)' >&2
  powershell_missing=1
fi

sh "$process_root/tools/test-bootstrap-repo.sh" --exhaustive
echo 'BOOTSTRAP_FIXTURE=PASS'
make -C "$process_root" check
echo 'PROCESS_FULL_CHECK=PASS'

# Official bootstrap is only executed on a temporary four-input snapshot.
tmp_root=$(mktemp -d "${TMPDIR:-/tmp}/pf-bootstrap-smoke.XXXXXX")
trap 'rm -rf "$tmp_root"' EXIT HUP INT TERM
mkdir -p "$tmp_root/consumer/docs"
cp "$repo_root/README.md" "$tmp_root/consumer/README.md"
for name in vision roadmap architecture; do
  cp "$repo_root/docs/$name.md" "$tmp_root/consumer/docs/$name.md"
done
sh "$process_root/tools/bootstrap-repo.sh" \
  --repo-root "$tmp_root/consumer" \
  --project-id photocraft-amiga \
  --project-name 'PhotoCraft AmigaOS 3.2 Port' \
  --story-id-prefix PF \
  --test-id-prefix PF \
  --starter-app-profile process-overlay \
  --skip-starter-build
test -f "$tmp_root/consumer/docs/bootstrap-report.md" || { echo 'FAIL official smoke did not generate report' >&2; exit 1; }
echo 'CONSUMER_INPUT_SMOKE=PASS (disposable fixture; real checkout not modified)'

if [ "${powershell_missing:-0}" -eq 1 ]; then
  exit 2
fi
echo 'PF_BOOTSTRAP_VALIDATION=PASS (registry activation remains separate)'
