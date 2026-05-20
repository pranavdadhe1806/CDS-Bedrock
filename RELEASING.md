# Release Guide for CDS-Bedrock

This document explains the complete release process — from making a code change to seeing a versioned GitHub Release with downloadable binaries appear automatically.

---

## Overview: The Three-Workflow Pipeline

```
Your commit to main
        │
        ▼
┌───────────────────────┐
│  auto-tag.yml         │  Reads VERSION from CMakeLists.txt.
│  (triggers on push    │  If the version is new, pushes tag v1.2.3.
│   to main)            │
└──────────┬────────────┘
           │  tag push triggers ↓
           ▼
┌───────────────────────┐
│  release.yml          │  Builds on Linux + Windows + macOS.
│  (triggers on v* tag) │  Runs tests. Installs. Packages binaries.
│                       │  Creates GitHub Release. Uploads all assets.
└───────────────────────┘
```

Also running in parallel on every push/PR:

```
push / pull_request
        │
        ├──▶  build.yml   (matrix: Ubuntu / Windows / macOS)
        └──▶  asan.yml    (Ubuntu + AddressSanitizer)
```

---

## How to Ship a New Release (The Full Process)

### Step 1 — Bump the version in CMakeLists.txt

Open `CMakeLists.txt` and update the `VERSION` field:

```cmake
# Before
project(cds-bedrock
    VERSION 0.1.0
    ...
)

# After — bumping to 0.2.0
project(cds-bedrock
    VERSION 0.2.0
    ...
)
```

> **Semantic Versioning rules**:
> - `PATCH` (0.1.0 → 0.1.1): Bug fixes only. No API changes.
> - `MINOR` (0.1.0 → 0.2.0): New features, backward-compatible.
> - `MAJOR` (0.1.0 → 1.0.0): Breaking API changes.

---

### Step 2 — Update CHANGELOG.md

Add a new section at the top of `CHANGELOG.md`:

```markdown
## [v0.2.0] - 2026-06-01

### Added
- New feature X

### Fixed
- Bug Y in the hash map

### Changed
- Refactored Z for clarity

[v0.2.0]: https://github.com/pranavdadhe1806/CDS-Bedrock/releases/tag/v0.2.0
```

The release workflow automatically extracts this section and uses it as the GitHub Release description.

---

### Step 3 — Commit and push to main

```bash
git add CMakeLists.txt CHANGELOG.md
git commit -m "chore: release v0.2.0"
git push origin main
```

That's it. The automation takes over:

1. `auto-tag.yml` sees `CMakeLists.txt` changed.
2. It reads `VERSION 0.2.0` → constructs tag `v0.2.0`.
3. Checks: does tag `v0.2.0` already exist? **No.**
4. Pushes tag `v0.2.0` to GitHub.
5. Tag push triggers `release.yml`.
6. `release.yml` builds on all three platforms, packages binaries, and publishes the GitHub Release.

---

## Manual Tagging (Optional — bypasses auto-tag)

If you want to tag manually without bumping `CMakeLists.txt`:

```bash
# Create an annotated tag
git tag -a v0.2.0 -m "Release v0.2.0"

# Push the tag to GitHub (triggers release.yml)
git push origin v0.2.0
```

---

## Pre-Release Versions

Tags containing a `-` are automatically marked as **pre-release** by the workflow:

```bash
git tag -a v1.0.0-rc.1 -m "Release candidate 1"
git push origin v1.0.0-rc.1
```

This creates a GitHub Release marked "Pre-release" — visible to users who browse releases, but not shown as the "Latest Release".

---

## What Gets Uploaded to the Release

Every release automatically attaches:

| File | Platform |
|------|----------|
| `cds-bedrock-linux-x86_64-vX.Y.Z.tar.gz` | Ubuntu (GCC) |
| `cds-bedrock-macos-arm64-vX.Y.Z.tar.gz` | macOS (Apple Clang) |
| `cds-bedrock-windows-x86_64-vX.Y.Z.zip` | Windows (MSVC/MinGW) |
| `cds-bedrock-src-vX.Y.Z.tar.gz` | Source archive (all platforms) |

Each binary package contains:
```
include/        ← All public headers
lib/            ← Static library (.a or .lib)
share/          ← CMake config files for find_package()
```

---

## Downstream Consumption After a Release

Once a release is published, users can consume it via CMake:

```bash
# Download and extract the archive for their platform, then:
cmake -S myapp -B build \
  -DCMAKE_PREFIX_PATH=/path/to/extracted/cds-bedrock
```

Or in their `CMakeLists.txt`:

```cmake
find_package(cds-bedrock CONFIG REQUIRED)
target_link_libraries(my_target PRIVATE cds-bedrock::cds-bedrock)
```

---

## Workflow File Reference

| File | Trigger | Purpose |
|------|---------|---------|
| `.github/workflows/build.yml` | push/PR to `main` | Cross-platform build + test matrix |
| `.github/workflows/asan.yml` | push/PR to `main` | Memory safety checks (ASan + UBSan) |
| `.github/workflows/auto-tag.yml` | push to `main` (CMakeLists.txt) | Auto-create git tag when VERSION bumps |
| `.github/workflows/release.yml` | push of `v*` tag | Build, package, and publish GitHub Release |

---

## First-Time Setup Checklist

Before your first release, verify the following on GitHub:

- [ ] **Actions are enabled**: `Settings → Actions → General → Allow all actions`
- [ ] **Workflow permissions**: `Settings → Actions → General → Workflow permissions → Read and write permissions` ✓
- [ ] **No branch protection blocking tag pushes**: `Settings → Branches` — ensure `auto-tag.yml` can push tags (the `GITHUB_TOKEN` permission in the workflow handles this automatically)
- [ ] Push your code to `main` at least once so all workflows appear in the Actions tab

---

## Troubleshooting

**Tag was not created automatically**
- Check the `auto-tag.yml` run in `Actions → Auto-Tag on Version Bump`.
- The most common reason: `CMakeLists.txt` was not part of the commit (only changed files trigger the workflow).

**Release workflow failed on Windows packaging**
- The release preset uses `cmake --install`. Make sure your `CMakeLists.txt` install targets are correct (they are — verified during Phase 4/5).

**Release body is empty**
- The workflow looks for a heading matching the exact tag name in `CHANGELOG.md`. Ensure the heading is `## [v0.2.0]` or `## v0.2.0` — not `## Version 0.2.0`.
