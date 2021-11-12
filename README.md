# CSE 471 Project 1
A Multi-Component Music Synthesizer


## Group 14
Anthony Rodeman (rodeman2@msu.edu)

Cameron Hurley (hurleyc6@msu.edu)

## Musical Selection: *Whisked to the Shire after Gameday*
Another victory for the Spartans wraps up, and you decide to relax by the Red Cedar. You have a nice talk with an old friend, and suddenly you find yourself in Middle Earth!

[Score File](/Scores/music-selection.score)

[Audio File](/Scores/music-selection.wav)


## Components
- [Effects](Pages/effects.md), Cameron Hurley
- [Additive Synthesizer](/Pages/additivesynth.md), Cameron Hurley
- [Wavetable Synthesizer](/Pages/wavetable.md), Anthony Rodeman


## XML Format
Extension: `.score`

A `score` head node which has attributes:
- `bpm`: beats per minute
- `beatspermeasure`: how many beats occur in a measure

The score node then has as many `instrument` notes as desired. Each `instrument` node has one attribute:
- `instrument`: the name of the instrument to use.

"Chorus", "Flange", "NoiseGate", and "Limiter" options are effects descriptors. 
These instruments must appear once to use effects on sound instruments and one `note` node is used to input values through attributes. 
The settings for each effect can be seen under the Effects component page. If no effects are used, the effect-named `instrument` tags are optional.

The AdditiveSynth, Wavetable, and ToneInstrument all can generate sound. Each can have a `chorus`, `flange`, `limiting`, and `noisegating` attribute, which corresponds to the use of that effect for child notes of the `instrument`. Accepted values are "0" (do not use effect) or "1" (use effect), with default of "0".

`note` nodes are always leaf nodes which are children of `instrument` nodes. For instruments that can generate sound, the following attributes must be defined:
- `measure`: which measure the note is played in (1-indexed integer)
- `beat`: which beat in the measure the note is played on (1-indexed floating point up to `beatspermeasure`, exclusive)
- `dur`: the length in beats which the note is played for (floating point)
- `note`: string which names the note in scientific pitch notation to be played (use 'b' for the flat symbol). Ex: "Ab4"

Each instrument may have more possible attributes. These attributes must be given on a per-`note` basis. Information about these attributes is found under the component pages.
