# Mine Sweeper — Retro-Go SD Homebrew

GWHB port of [slipperstree/game-and-watch-mine-sweeper](https://github.com/slipperstree/game-and-watch-mine-sweeper)
for [Game & Watch Retro-Go SD](https://github.com/sylverb/game-and-watch-retro-go-sd).

Drop `MineSweeper.bin` on the SD card under `/roms/homebrew/`.

## Build

```bash
make                    # PROJECT_KIND=homebrew is the default
# or without a local ARM toolchain:
make docker
```

Produces `MineSweeper.bin` → copy to `/homebrews/` (or unzip a release archive
onto the SD root). Optional cover override: `/covers/homebrew/MineSweeper.img`.

Tagged releases ship two zips only: install (`MineSweeper-<tag>.zip`) and
debug symbols (`MineSweeper-<tag>-debug.zip`, ELF + map).

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
