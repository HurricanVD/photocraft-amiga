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
  docs/reviews/PF-SP-002-atomization-report-2026-10-11.md
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
# Check the actual Status column and the lifecycle backlog section.
# This is a cheap fail-closed evidence guard, not a replacement for a full DoR review.
pf_backlog_row() {
  awk -F'|' -v target="$1" -v want="$2" -v category="$3" '
    /^## / { section=$0 }
    /^\|/ {
      id=$2; state=$6
      gsub(/^[[:space:]]+|[[:space:]]+$/, "", id)
      gsub(/^[[:space:]]+|[[:space:]]+$/, "", state)
      if (id==target) {
        count++
        if (state==want && section==category) correct++
      }
    }
    END { exit !(count==1 && correct==1) }
  ' "$repo_root/docs/backlog.md"
}
pf_active_count() {
  awk -F'|' -v target="$1" '
    /^\\|/ {
      id=$2
      gsub(/^[[:space:]]+|[[:space:]]+$/, "", id)
      if (id==target) count++
    }
    END { print count+0 }
  ' "$repo_root/docs/backlog.md"
}
pf_done_count() {
  awk -F'|' -v target="$1" '
    /^\|/ {
      id=$2
      gsub(/^[[:space:]]+|[[:space:]]+$/, "", id)
      if (id==target) count++
    }
    END { print count+0 }
  ' "$repo_root/docs/backlog-done.md"
}
pf_backlog_row PF-SP-001 blocked '## Blockiert' || { echo 'FAIL PF-SP-001 not in blocked section' >&2; exit 1; }
pf_backlog_row PF-SP-002 blocked '## Blockiert' || { echo 'FAIL PF-SP-002 is not blocked in canonical backlog section' >&2; exit 1; }
grep -Eq '^- Status: [`]?blocked[`]?([[:space:]]|$)' "$repo_root/docs/stories/PF-SP-002.md" || { echo 'FAIL canonical parent is not blocked' >&2; exit 1; }
[ ! -e "$repo_root/docs/stories/done/PF-SP-002.md" ] || { echo 'FAIL blocked parent duplicated in done archive' >&2; exit 1; }
[ "$(pf_done_count PF-SP-002)" -eq 0 ] || { echo 'FAIL blocked parent present in backlog-done' >&2; exit 1; }
grep -q 'decision: `split_required`' "$repo_root/docs/reviews/PF-SP-002-atomization-report-2026-10-11.md" || { echo 'FAIL retrospective atomization decision missing' >&2; exit 1; }
# Only ONE canonical status per gate is valid: prevent appended `pass` lines
# from overriding older pending/failing lines in a promoted child story.
pf_gate_marker() {
  key=$1
  expected=$2
  story_file=$3
  awk -v key="$key" -v expected="$expected" '
    /^- `/ {
      line=$0; sub(/^- `/, "", line)
      split(line, tokens, "`")
      if (tokens[1]==key) {
        count++
        if ($0=="- `" key "`: `" expected "`") match_count++
      }
    }
    END { exit !(count==1 && match_count==1) }
  ' "$story_file"
}

# Child IDs may advance through any *valid* VD lifecycle stage. Status
# transitions must include their separate DoR/implementation/review markers.
for id in PF-SP-004 PF-SP-005 PF-SP-006 PF-SP-007 PF-SP-008 PF-SP-009 PF-TD-001; do
  active="$repo_root/docs/stories/$id.md"
  archived="$repo_root/docs/stories/done/$id.md"
  if [ -f "$active" ] && [ -f "$archived" ]; then echo "FAIL duplicated child $id" >&2; exit 1; fi
  if [ -f "$active" ]; then story="$active"
  elif [ -f "$archived" ]; then story="$archived"
  else echo "FAIL missing child $id" >&2; exit 1; fi
  status=$(sed -n 's/^- Status: [`]*\([a-z_]*\)[`]*.*/\1/p' "$story" | head -n 1)
  case "$status" in
    draft|refining) section='## Offen' ;;
    ready) section='## Bereit' ;;
    in_progress|review) section='## Aktive PF-Stories' ;;
    blocked|rejected) section='## Blockiert' ;;
    done) section='' ;;
    *) echo "FAIL invalid child status $id: $status" >&2; exit 1 ;;
  esac
  if [ "$status" = done ] || { [ "$status" = rejected ] && [ "$story" = "$archived" ]; }; then
    [ "$story" = "$archived" ] || { echo "FAIL terminal child not archived $id" >&2; exit 1; }
    [ "$(pf_done_count "$id")" -eq 1 ] || { echo "FAIL missing/duplicate done row $id" >&2; exit 1; }
    [ "$(pf_active_count "$id")" -eq 0 ] || { echo "FAIL stale or duplicate active row $id" >&2; exit 1; }
  else
    [ "$story" = "$active" ] || { echo "FAIL non-done child archived $id" >&2; exit 1; }
    pf_backlog_row "$id" "$status" "$section" || { echo "FAIL backlog status/section $id: $status" >&2; exit 1; }
    [ "$(pf_done_count "$id")" -eq 0 ] || { echo "FAIL stale done entry $id" >&2; exit 1; }
  fi
  case "$status" in
    ready|in_progress|review|done)
      pf_gate_marker "refinement_triage" pass "$story" &&
      pf_gate_marker "arch_review.initial" pass "$story" &&
      pf_gate_marker "arch_review.pre_ready_final" pass "$story" &&
      pf_gate_marker "dor_check" pass "$story" || {
        echo "FAIL missing pre-ready/DoR proof on $id" >&2; exit 1
      } ;;
  esac
  case "$status" in
    in_progress|review|done)
      pf_gate_marker "implementation_gate_report" pass "$story" || {
        echo "FAIL missing implementation gate $id" >&2; exit 1
      } ;;
  esac
  if [ "$status" = done ]; then
    pf_gate_marker review_report approve "$story" || {
      echo "FAIL missing final review approval $id" >&2; exit 1
    }
  fi
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
