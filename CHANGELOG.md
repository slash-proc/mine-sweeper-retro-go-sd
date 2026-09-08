# Changelog

This file follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). Release tags must
match a section heading exactly (for example `v1.0.0`); CI reads the matching
section and uses it as the GitHub Release notes.

When you cut a release:

1. Move items from `[Unreleased]` into a new `## [vX.Y.Z] - YYYY-MM-DD` section.
2. Commit the changelog update.
3. Push the tag: `git tag vX.Y.Z && git push origin vX.Y.Z`

## [Unreleased]

## [v0.2.0] - 2026-09-08

### Changed

- The SDK moves to the new packed-header model: `gwhb_meta_t` now carries
  `segments_count` and a `segments[]` array instead of one code/bss pair, and
  the firmware ABI grows to 844 bytes. Taken from upstream.
- The shared dist scripts are current: the manifest generator reads either
  header layout, CI reads the sidecar list from the Makefile rather than one
  hardcoded slot, and `project` is derived as an identifier instead of copying
  a display name.

### Added

- The full-size cover art is published with the release, beside the smaller
  one packed into the binary.

### Changed (device files)

- The installed binary is now `MineSweeper.bin`, upstream's spelling. A card
  that already holds `minesweeper.bin` keeps it, and the launcher lists both
  until the old one is deleted.


## [v0.1.3] - 2026-09-05

Tooling and packaging only. The binary is the same game; what changed is what
the release says about itself.

### Fixed

- The packed GWHB header carried the literal version `1.0.0` while the project
  was at v0.1.2, so every device has shown the wrong number since the first
  release. `CORE_VERSION` now comes from `git describe --tags --dirty`, and an
  untagged build stamps `0.0.0` rather than claiming a release number it does
  not have. This needs the newer `pack_homebrew.py`, whose `parse_version`
  accepts a describe string; the previous one required a bare `X.Y.Z` and died
  on everything else.
- `stage_release.py` read the Makefile's `print-` targets with stderr folded
  into stdout and then split the result into values, so a `$(warning)` emitted
  at parse time became a value and failed the release. Nothing warns in this
  Makefile, which is the only reason it never fired here.

### Added

- `docs` in the manifest, pointing at this repository's README. It is the one
  absolute URL the [GWRG distribution spec](https://github.com/slash-proc/gwrg-dist-spec)
  allows: somewhere a UI can send a human, as opposed to the plain filenames an
  installer resolves.
- A debug symbols archive (`minesweeper-<tag>-debug.zip`) holding the ELF and
  the linker map, published for the first time.

### Changed

- The release tooling is now byte-identical to snake's and smw's:
  `build_dist.py`, `make_bundle.py`, `stage_release.py`, `scripts/DEBUG_README.md`
  and `sdk/tools/pack_homebrew.py` are the same files in all three projects, so
  a project can vendor them without editing anything. Everything
  project-specific they need, they ask the Makefile for.
- Release assets: the tag-stamped copy of the binary (`minesweeper-<tag>.bin`)
  is gone, replaced by the debug archive. The loose `minesweeper.bin` the
  manifest names and the SD install zip are unchanged.

## [v0.1.2] - 2026-09-05

### Fixed

- The dist mirror no longer publishes a retained release whose manifest cannot
  be validated against the current spec. v0.1.0 predates the removal of `role`
  and `format`, and mirroring it made the whole project read as non-conformant
  over one old version nobody installs. It stays attached to its GitHub
  release, which is the archival record, and is simply absent from the mirror.

## [v0.1.1] - 2026-09-05

### Changed

- The manifest no longer emits `role` or `format` on artifacts. Both were
  removed from the [GWRG distribution spec](https://github.com/slash-proc/gwrg-dist-spec):
  `format` was read only by the conformance checker's own UI, and `role` was
  redundant because the firmware finds what to launch by scanning for `.bin`
  rather than by consulting a manifest. Since artifacts are
  `additionalProperties: false`, v0.1.0's published manifest no longer
  validates, so this release republishes it.

  Nothing else changes: the same binary, the same hash, the same firmware ABI
  requirement.

## [v0.1.0] - 2026-09-04

First release of Minesweeper as a Retro-Go SD homebrew, and the first release
published for the GWRG distribution model.

### Added

- Minesweeper ported from
  [slipperstree/game-and-watch-mine-sweeper](https://github.com/slipperstree/game-and-watch-mine-sweeper)
  as a GWHB homebrew, packed by `pack_homebrew.py` with an embedded cover.
- `manifest.json` release asset describing the build per the
  [GWRG distribution spec](https://github.com/slash-proc/gwrg-dist-spec). The
  firmware ABI requirement and display name are read out of the packed GWHB
  header, so the manifest cannot drift from the binary.
- Offline bundle (`minesweeper-<tag>.zip`) holding the manifest and the files it
  names, so the release stays installable if this repository disappears. Every
  file is checked against the manifest's size and hash before it is added.
- GitHub Pages mirror of the newest 5 releases, rebuilt from the releases on
  every tag, so a web installer can read them (browsers cannot fetch release
  assets cross-origin).

### Install

- Copy `minesweeper.bin` to `/homebrews/` on the SD card.
- Optional coverflow override: `/covers/homebrew/minesweeper.img`
  (JPEG ≤186×100, ≤10 KiB). The binary already embeds a cover.
- Requires firmware whose ABI matches `SDK_VERSION` in this repository.

No ROM or asset conversion is needed, so an installer never asks for a file.
