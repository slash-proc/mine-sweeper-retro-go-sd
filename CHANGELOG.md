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
