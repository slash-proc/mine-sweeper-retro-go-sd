# Mine Sweeper — Retro-Go SD Homebrew

GWHB port of [slipperstree/game-and-watch-mine-sweeper](https://github.com/slipperstree/game-and-watch-mine-sweeper)
for [Game & Watch Retro-Go SD](https://github.com/sylverb/game-and-watch-retro-go-sd).

Drop `minesweeper.bin` on the SD card under `/roms/homebrew/`.

## Build

```bash
make                    # PROJECT_KIND=homebrew is the default
# or without a local ARM toolchain:
make docker
```

Produces `minesweeper.bin` (optional cover override: `/covers/homebrew/minesweeper.img`).

## Controls

| Screen | Controls |
|--------|----------|
| Home | **Up / Down** — difficulty · **A** — start |
| In game | **D-pad** — cursor · **A** — dig · **B** — flag |
| Game over | Any button — home · **TIME / SELECT / Y** — info |

Chord-clear: dig (**A**) on a revealed number that already has enough neighbouring flags.

Pause / power stay with Retro-Go (system menu / sleep).

## Credits

- Game logic & UI: MANDA ([slipperstree](https://github.com/slipperstree/game-and-watch-mine-sweeper)), BSD-3-Clause
- Base hardware bring-up in upstream: [game-and-watch-base](https://github.com/ghidraninja/game-and-watch-base)
- This tree: Retro-Go SD SDK template (MIT) + homebrew glue

## License

- Homebrew glue / SDK template: MIT (see `LICENSE`)
- Ported `src/mine/` sources retain upstream BSD-3-Clause (see upstream `COPYING`)
- Vendored files under `sdk/include/` keep their upstream licenses

## Distribution

Tagged releases carry a `manifest.json` describing the build per the
[GWRG distribution spec](https://github.com/slash-proc/gwrg-dist-spec), and CI
mirrors the newest 5 releases to GitHub Pages so a web installer can read them
(browsers cannot fetch release assets cross-origin).

```
https://slash-proc.github.io/mine-sweeper-retro-go-sd/dist/versions.json
https://slash-proc.github.io/mine-sweeper-retro-go-sd/dist/<tag>/manifest.json
https://slash-proc.github.io/mine-sweeper-retro-go-sd/dist/<tag>/minesweeper.bin
```

Each version is also published as an offline bundle — `dist/minesweeper-<tag>-bundle.zip`,
containing the manifest and the files it names — so a release stays installable
if this repository disappears.

GitHub Pages must be enabled once on the repository (Settings -> Pages ->
Source: GitHub Actions) before the mirror can deploy.

Releases remain the source of truth; `scripts/build_dist.py` rebuilds the whole
Pages tree from them on every tag, so the mirror is disposable.

`scripts/make_manifest.py` reads the firmware ABI requirement and display name
out of the packed GWHB header — never from a constant — so the manifest cannot
drift from the binary it describes.

Minesweeper needs no proprietary assets, so its manifest declares `"tools": []`
and an installer never asks the user for a file.
